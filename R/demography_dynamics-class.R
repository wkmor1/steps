#' Modify the demography in a state object.
#' 
#' A \code{demography_dynamics} object is used to modify life-stage transition
#' matrices - for example, adding stochasticity.
#' 
#' A \code{demography_dynamics} object is a sub-component of a \link[steps]{dynamics}
#' object and is executed in each timestep of a simulation. Note, some dynamics
#' functions can be executed at non-regular intervals (i.e. only timesteps
#' explicitly defined by the user). The \code{build_demography_dynamics} function is
#' used to construct a demography dynamics object consisting of several demographic
#' dynamics functions and their associated parameters. These functions specify how
#' the demography in the state object will be modified throughout a simulation.
#'
#' @rdname demography_dynamics
#'
#' @param ... Functions that operates on a state object to change demography
#' at specified timesteps. A user may enter custom functions or select
#' pre-defined modules - see examples. 
#' @param object A \code{demography_dynamics} object to print or test.
#'
#' @return An object of class \code{demography_dynamics}
#' 
#' @export
#'
#' @examples
#' 
#' library(steps)
#' library(raster)
#' 
#' # Import a raster layer for habitat
#' r <- raster(system.file("external/test.grd", package="raster"))
#' 
#' # Create a life-stage matrix
#' mat <- matrix(c(0.000,0.000,0.302,0.302,
#'                 0.940,0.000,0.000,0.000,
#'                 0.000,0.884,0.000,0.000,
#'                 0.000,0.000,0.793,0.793),
#'               nrow = 4, ncol = 4, byrow = TRUE)
#' colnames(mat) <- rownames(mat) <- c('Stage_1','Stage_2','Stage_3','Stage_4')
#' 
#' # Create a matrix with standard deviations for environmental stochasticity
#' mat_sd <- matrix(c(0.000,0.00,0.010,0.010,
#'                 0.010,0.000,0.000,0.000,
#'                 0.000,0.010,0.000,0.000,
#'                 0.000,0.000,0.010,0.010),
#'               nrow = 4, ncol = 4, byrow = TRUE)
#' colnames(mat_sd) <- rownames(mat_sd) <- c('Stage_1','Stage_2','Stage_3','Stage_4')
#' 
#' # Create a stack of raster layers to represent each
#' # life-stage of a population structure (four in this case)
#' pop <- stack(replicate(4, ceiling(r * 0.2)))
#'
#' # Create raster and shuffle values (omit NAs)
#' r2 <- r
#' r2[na.omit(r2)] <- sample(r[na.omit(r)])
#' 
#' # Create raster and shuffle values (omit NAs)
#' r3 <- r
#' r3[na.omit(r3)] <- sample(r[na.omit(r)])
#' 
#' # Create a list of rasters stacks for all life stages
#' surv <- list(stack(r2, r2, r2),
#'              stack(r2, r2, r2),
#'              stack(r2, r2, r2),
#'              stack(r2, r2, r2))
#'
#' # Create a list of raster stacks when the first two stages are NULL            
#' fec <- list(NULL,
#'             NULL,
#'             stack(r3, r3, r3),
#'             stack(r3, r3, r3))
#' 
#' # Construct habitat, demography, and population objects.
#' test_habitat <- build_habitat(habitat_suitability = r / cellStats(r, "max"),
#'                               carrying_capacity = ceiling(r * 0.1))
#' test_demography <- build_demography(transition_matrix = mat)
#' test_population <- build_population(pop)
#' 
#' # Construct a state object
#' test_state <- build_state(test_habitat, test_demography, test_population)
#' 
#' # Select existing dynamic functions to be run on the demography
#' # in a simulation and specify input parameters: 
#' env_stoch <- demo_environmental_stochasticity(transition_matrix = mat,
#'                                               stochasticity = mat_sd)
#'                                               
#' demo_dens <- demo_density_dependence(transition_matrix = mat)
#' 
#' # Construct a demography dynamics object
#' test_demo_dynamics <- build_demography_dynamics(env_stoch, demo_dens)

build_demography_dynamics <- function (...) {
  
  dots <- list(...)
  
  # run checks on the functions passed in to make sure they are legit
  
  demo_dynamics <- function (state, timestep) {
    
    if (!is.null(unlist(dots))){
      for (fun in dots) {
        state <- fun(state, timestep)
      }
    }
    
    state
    
  }
  
  as.demography_dynamics(demo_dynamics)
  
}

as.demography_dynamics <- function (demography_dynamics_function) {
  as_class(demography_dynamics_function, "demography_dynamics", "function")
}

#' @rdname demography_dynamics
#'
#' @export
#' 
#' @examples
#'
#' # Test if object is of the type 'demography_dynamics'
#' is.demography_dynamics(test_demo_dynamics)

is.demography_dynamics <- function (object) {
  inherits(object, 'demography_dynamics')
}

#' @rdname demography_dynamics
#'
#' @export
#'
#' @examples
#' 
#' # Print details about the 'demography_dynamics' object
#' print(test_demo_dynamics)

print.demography_dynamics <- function (object) {
  cat("This is a demography_dynamics object")
}