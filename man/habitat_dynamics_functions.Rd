% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/habitat_dynamics-functions.R
\name{habitat_dynamics_functions}
\alias{habitat_dynamics_functions}
\alias{disturbance_fires}
\title{Functions to modify the habitat in a state object.}
\usage{
disturbance_fires(habitat_suitability, disturbance_layers,
  effect_time = 1)
}
\arguments{
\item{habitat_suitability}{a raster layer or stack containing habitat suitability for each cell}

\item{disturbance_layers}{a raster stack with fire disturbances used to alter the habitat object in the experiment (number of layers must match the intended timesteps in the experiment)}

\item{effect_time}{the number of timesteps that the disturbance layer will act on the habitat object}
}
\description{
Pre-defined functions to operate on a habitat and carrying capacity
during a simulation.
}
\examples{

library(steps)


# Use the disturbance_fires function to modify the habitat using spatial
# fire history layers:

test_fires <- disturbance_fires(habitat_suitability = r / cellStats(r, "max"),
                                    disturbance_layers = dist,
                                    effect_time = 1)
}
