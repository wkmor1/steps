#' Create a dynamics object to run in an experiment
#'
#' @description A dynamics object is a group of modules (functions) used modify habitat, population, and demography in a timestep.
#' It modifies a state object in each timestep of an experiment.
#' 
#' @rdname dynamics
#' 
#' @param population_dynamics A module to alter the population object in an experiment
#' @param habitat_dynamics A module to alter the habitat object in an experiment
#' @param demography_dynamics A module to alter the habitat object in an experiment
#' @param order The order to apply the dynamics at each timestep in an experiment 
#' @param x an object to print or test as a dynamics object
#' @param ... further arguments passed to or from other methods
#'
#' @return An object of class \code{dynamics}
#' 
#' @export
#'
#' @examples
#' 
#' library(steps)
#' library(raster)
#' 
#' test_dynamics <- build_dynamics(no_habitat_dynamics,
#'                                 no_demography_dynamics,
#'                                 fast_population_dynamics)

build_dynamics <- function (habitat_dynamics,
                            demography_dynamics,
                            population_dynamics,
                            order = c("demography_dynamics",
                                      "habitat_dynamics",
                                      "population_dynamics")
                            ) {
  #INSERT CHECKS FOR OBJECT CLASSES
  dynamics <- list(habitat_dynamics = habitat_dynamics,
                   demography_dynamics = demography_dynamics,
                   population_dynamics = population_dynamics)

  # get all the functions in a list, in the required order
  check_dynamics_order(order)
  dynamics <- lapply(order, get, envir = environment())
  set_class(dynamics, "dynamics")
}

#' @rdname dynamics
#'
#' @export
#' 
#' @examples
#'
#' # Test if object is of the type 'dynamics'
#'   
#' is.dynamics(test_dynamics)

is.dynamics <- function (x) {
  inherits(x, 'dynamics')
}

#' @rdname dynamics
#'
#' @export
#'
#' @examples
#'
#' print(test_dynamics)

print.dynamics <- function(x, ...) {
  cat("This is a dynamics object")
}


##########################
### internal functions ###
##########################

check_dynamics_order <- function (order) {
  sorted_order <- sort(order)
  expected <- c("demography_dynamics",
                "habitat_dynamics",
                "population_dynamics")
  if (!identical(sorted_order, expected)) {
    msg <- paste0("order must be a length-4 character vector giving the order ",
                  "in which to run the dynamic functions. It must contain each ",
                  "of the following strings once and only once:\n",
                  "'", paste(expected, collapse = "', '"), "'")
    stop (msg, call. = FALSE)
  }
}