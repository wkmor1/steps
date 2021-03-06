#include <Rcpp.h>
using namespace Rcpp;


/*
** can_source_cell_disperse: This function will search, for a given input location, for a
**            suitable "source" pixel that can lead to the colonization of
**            the input location (sink pixel).
*/

// [[Rcpp::export]]
bool barrier_to_dispersal(int sink_x, int sink_y, int source_x, int source_y, NumericMatrix barriers_map, int barrier_type){
  int  dist_x, dist_y, i, pixel_x, pixel_y, distance_max, barrier_counter;
  bool barrier_found;
  barrier_found = false;

  /*
  ** Calculate the distance in both dimensions between the source and sink
  ** pixels and take the largest of the two.
  */
  dist_x = source_x - sink_x;
  dist_y = source_y - sink_y;
  if (abs (dist_x) >= abs (dist_y)){
    distance_max = abs(dist_x);
  } else {
    distance_max = abs(dist_y);
  }

  /*
  ** Check the possible paths from source to sink and see if there is a path
  ** without barriers.
  */
  if (barrier_type == 0){
    /*
    ** Weak barrier: If there is at least one free path we're good.
    **
    ** BARRIER MIDDLE
    */
    barrier_found = false;
    for (i = 1; i <= distance_max; i++){
      pixel_x = round(sink_x + (1.0 * i / distance_max * dist_x));
      pixel_y = round(sink_y + (1.0 * i / distance_max * dist_y));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1) {
		  barrier_found = true;
		  break;
      }
    }
    if (!barrier_found){
      return(barrier_found);
    }
    /*
    ** BARRIER TOP_LEFT
    */
    barrier_found = false;
    for (i = 1; i <= distance_max; i++){
      pixel_x = round(sink_x - 0.49 + (1.0 * i / distance_max * dist_x));
      pixel_y = round(sink_y - 0.49 + (1.0 * i / distance_max * dist_y));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
		barrier_found = true;
		break;
      }
    }
    if (!barrier_found){
      return(barrier_found);
    }
    /*
    ** BARRIER TOP_RIGHT
    */
    barrier_found = false;
    for (i = 1; i <= distance_max; i++){
      pixel_x = round(sink_x + 0.49 + (1.0 * i / distance_max * dist_x));
      pixel_y = round(sink_y - 0.49 + (1.0 * i / distance_max * dist_y));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
	barrier_found = true;
	break;
      }
    }
    if (!barrier_found)
    {
     return(barrier_found);
    }
    /*
    ** Barrier DOWN_LEFT
    */
    barrier_found = false;
    for (i = 1; i <= distance_max; i++){
      pixel_x = round(sink_x - 0.49 + (1.0 * i / distance_max * dist_x));
      pixel_y = round(sink_y + 0.49 + (1.0 * i / distance_max * dist_y));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
	barrier_found = true;
	break;
      }
    }
    if (!barrier_found){
     return(barrier_found);
    }
    /*
    ** Barrier DOWN_RIGHT
    */
    barrier_found = false;
    for (i = 1; i <= distance_max; i++){
      pixel_x = round(sink_x + 0.49 + (1.0 * i / distance_max * dist_x));
      pixel_y = round(sink_y + 0.49 + (1.0 * i / distance_max * dist_y));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
	barrier_found = true;
	break;
      }
    }
    if (!barrier_found){
     return(barrier_found);
         }
  }
  else if (barrier_type == 1){
    /*
    ** Strong barrier: If more than one way is blocked by a barrier then
    **                 colonization fails.
    */
    barrier_counter = 0;
    barrier_found = false;
    /*
    ** BARRIER MIDDLE
    */
    for (i = 1; i <= distance_max; i++){
      pixel_x = round(sink_x + (1.0 * i / distance_max * dist_x));
      pixel_y = round(sink_y + (1.0 * i / distance_max * dist_y));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
	barrier_counter++;
	break;
      }
    }
    /*
    ** BARRIER TOP_LEFT
    */
    for (i = 1; i <= distance_max; i++){
	  pixel_x = round(sink_x - 0.49 + (((i-1.0) / distance_max * dist_x) +
					((1.0 / distance_max * dist_x) / 2.0)));
      pixel_y = round(sink_y - 0.49 + (((i-1.0) / distance_max * dist_y) +
					((1.0 / distance_max * dist_y) / 2.0)));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
	barrier_counter++;
	break;
      }
    }
    if (barrier_counter > 1){
      barrier_found = true;
      return(barrier_found);
    }
    /*
    ** BARRIER TOP_RIGHT
    */
    for (i = 1; i <= distance_max; i++){
      pixel_x = round(sink_x + 0.49 + (((i-1.0) / distance_max * dist_x) +
					((1.0 / distance_max * dist_x) / 2.0)));
      pixel_y = round(sink_y - 0.49 + (((i-1.0) / distance_max * dist_y) +
					((1.0 / distance_max * dist_y) / 2.0)));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
	barrier_counter++;
	break;
      }
    }
    if (barrier_counter > 1){
      barrier_found = true;
     return(barrier_found);
    }
    /*
    ** BARRIER DOWN_LEFT
    */
    for (i = 1; i <= distance_max; i++){
      pixel_x = round (sink_x - 0.49 + (((i-1.0) / distance_max * dist_x) +
					((1.0 / distance_max * dist_x) / 2.0)));
      pixel_y = round (sink_y + 0.49 + (((i-1.0) / distance_max * dist_y) +
					((1.0 / distance_max * dist_y) / 2.0)));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
		barrier_counter++;
		break;
      }
    }
    if (barrier_counter > 1){
      barrier_found = true;
     return(barrier_found);
    }
    /*
    ** BARRIER DOWN_RIGHT
    */
    for (i = 1; i <= distance_max; i++){
      pixel_x = round (sink_x + 0.49 + (((i-1.0) / distance_max * dist_x) +
					((1.0 / distance_max * dist_x) / 2.0)));
      pixel_y = round (sink_y + 0.49 + (((i-1.0) / distance_max * dist_y) +
					((1.0 / distance_max * dist_y) / 2.0)));
      if (R::rbinom(1, barriers_map(pixel_x,pixel_y)) == 1){
	barrier_counter++;
	break;
      }
    }
    if (barrier_counter > 1){
      barrier_found = true;
      return(barrier_found);
    }
  }
}

// [[Rcpp::export]]
IntegerVector can_source_cell_disperse(int i, int j, NumericMatrix carrying_capacity_available, 
                                       NumericMatrix tracking_population_state, NumericMatrix habitat_suitability_map,
                                       NumericMatrix barriers_map, bool use_barrier, int barrier_type, int loopID, 
                                       int dispersal_distance, NumericVector dispersal_kernel){

 	int ncols = carrying_capacity_available.ncol();
  int nrows = carrying_capacity_available.nrow();
	int    k, l, real_distance;
	double prob_colonisation, rnd;
	IntegerVector source_found(2,-9999);

  /*
  ** Search for a potential source cell. i and j are the coordinates of the
  ** sink cell. k and l are the coordinates of the potential source cell.
  */
  for (k = i - dispersal_distance; k <= i + dispersal_distance; k++){
    for (l = j - dispersal_distance; l <= j + dispersal_distance; l++){

      /*
      ** 1. Test of basic conditions to see if a pixel could be a potential
      **    source cell:
      **    - The pixel must be within the limits of the matrix's extent.
      **    - The pixel must be colonized, but not during the current loop, and is not NA.
      ** 	  - The pixel must have avaliable carrying capacity to allow recruitment.
      */
      if ((k >= 0) && (k < nrows) && (l >= 0) && (l < ncols)){
		    if (carrying_capacity_available(k,l) > 0){
		        if (tracking_population_state(k,l) != loopID){
		            if(!R_IsNA(tracking_population_state(k,l))){
          	    /*
          	    ** 2. Compute the distance between sink and (potential) source pixel
          	    **    and check if it is <= maximum dispersal distance. The distance
          	    **    is computed in pixel units.
          	    */
          	    real_distance = round(sqrt((k-i)*(k-i) + (l-j)*(l-j)));
	              if ((real_distance > 0) && (real_distance <= dispersal_distance)){
        	      /*
        	      ** 3. Compute the probability of colonisation of the sink pixel.
        	      **    This probability depends on several factors:
        	      **    - Disance between source and sink cells.
        	      */
          		  prob_colonisation = dispersal_kernel[real_distance-1] * (habitat_suitability_map(k,l));
        	      rnd = as<double>(Rcpp::runif(1));
	              if (rnd < prob_colonisation || prob_colonisation == 1.0){
              				/*
              				** When we reach this stage, the last thing we need to check for
              				** is whether there is a "barrier" obstacle between the source
              				** and sink pixel. We check this last as it requires significant
              				** computing time.
              				*/
              				if (use_barrier){
              				  if (!barrier_to_dispersal(i, j, k, l, barriers_map, barrier_type)){
              					source_found[0] = k;
              					source_found[1] = l;
              				  }
              				}
              				else {
              				source_found[0] = k;
              				source_found[1] = l;
          					}
	                }
	              }
		          }
		        }
					}
  			}
      }
		}
  return(source_found);
}

// This function cleans up the habitat matrix before dispersal.
// [[Rcpp::export]]
NumericMatrix clean_matrix(NumericMatrix in_matrix,
						   NumericMatrix barriers_map,
						   bool filter_na_data = true,
						   bool filter_barriers = true,
						   bool insert_na_data = true){
	int i, j;
	int ncols = in_matrix.ncol();
	int nrows = in_matrix.nrow();
	  // set any value < 0 to 0, removes nan data and data where carrying  capacity is */
	  if(filter_na_data){
		for (i = 0; i < nrows; i++){
			for (j = 0; j < ncols; j++){
				if (in_matrix(i,j) < 0) in_matrix(i,j) = 0;
			}
		 }
	  }

	  /* Turn barrier cells into zero. */
	  if(filter_barriers){
		for (i = 0; i < nrows; i++){
			for (j = 0; j < ncols; j++){
				if (barriers_map(i,j) == 1) in_matrix(i,j) = 0;
			}
		 }
	  }

	  /* turn NA in barrier_matrix into NA in active matrix. */
	  if(insert_na_data){
		for (i = 0; i < nrows; i++){
			for (j = 0; j < ncols; j++){
				if (barriers_map(i,j) == NA_REAL) in_matrix(i,j) = NA_REAL;
			}
		 }
	  }
     return(in_matrix);
}

// [[Rcpp::export]]
int proportion_of_population_to_disperse(int source_x, int source_y, int sink_x, int sink_y, 
                                         NumericMatrix starting_population_state,
                                         NumericMatrix current_carrying_capacity, 
                                         double dispersal_proportion){
  	        double source_pop, source_pop_dispersed;
            source_pop = round(starting_population_state(source_x,source_y));
            if(source_pop<1)source_pop=0;
            source_pop_dispersed = R::rbinom(source_pop,dispersal_proportion);
            // Rcpp::Rcout << source_pop_dispersed << ' ' << source_pop << std::endl;
            if (current_carrying_capacity(sink_x,sink_y) < source_pop_dispersed){
                // could include this function to allow a smaller proportion of the population to disperse.
                // dispersal_proportion = dispersal_proportion - 0.01;
                // source_pop_dispersed = R::rbinom(source_pop,dispersal_proportion);
                // if(dispersal_proportion <= 0) 
                source_pop_dispersed = 0;
            }
  return(source_pop_dispersed);
}

// [[Rcpp::export]]
NumericMatrix na_matrix(int nr, int nc){
  NumericMatrix m(nr,nc) ;
  std::fill( m.begin(), m.end(), NumericVector::get_na() ) ;
  return m ;
}

// //' dispersal function for dynamic metapopulation models
// //' @param current_distribution raster of current population distribution.
// //' @param habitat_suitability raster of habitat suitability that has been converted to carrying capacity
// //' @param barrier_map raster of barriers to the dispersal, 1=barrier; 0=no barrier.
// //' @param barrier_type if 0 weak barrier, if 1 strong barriers.
// //' @param use_barrier if true use barriers in dispersal analysis.
// //' @param dispersal_steps The number of dispersal iterations per C++ call.
// //' @param dispersal_distance The maximum number of cells the species can disperse.
// //' @param dispersal_kernal a numeric vector of probabilites of dispersing from one to n cells, where n is the dispersal distance.
// //' @param dispersal_proportion the proportion of species that will disperse from source cell, needs to be between 0 and 1. e.g 0.2 means that 20% of the cell's population disperses. 
// [[Rcpp::export]]
List rcpp_dispersal(NumericMatrix starting_population_state, NumericMatrix potential_carrying_capacity,
  NumericMatrix habitat_suitability_map,NumericMatrix barriers_map, int barrier_type, bool use_barrier, int dispersal_steps,
  int dispersal_distance, NumericVector dispersal_kernel, double dispersal_proportion){

	  int ncols = starting_population_state.ncol();
    int nrows = starting_population_state.nrow();
    NumericMatrix carrying_capacity_available = na_matrix(nrows,ncols); // carrying capacity avaliable.
    NumericMatrix tracking_population_state = na_matrix(nrows,ncols); // tracking population state.
    NumericMatrix future_population_state = na_matrix(nrows,ncols); // future population size (after dispersal).
    int loopID, dispersal_step, i, j;
    bool habitat_is_suitable;//, cell_in_dispersal_distance;

	// check how much carrying capacity is free per-cell - this will enable dispersal to these cells if needed.
     for(i = 0; i < nrows; i++){
         for(j = 0; j < ncols; j++){
      			 if(!R_IsNA(starting_population_state(i,j))){
	              carrying_capacity_available(i,j) = potential_carrying_capacity(i,j) - starting_population_state(i,j); // carrying capacity available = potential carrying capacity - current population state.
                tracking_population_state(i,j) = starting_population_state(i,j);			   
	          }
	      }
     }

      /* Filter the habitat suitability matrix in three ways:
      **  1. replace any value < 0 by 0 (this removes NoData).
      **  2. set habitat suitability to 0 where barrier = 1.
      **  3. set habitat suitability values to NoData where barrier = NoData.
      ** and also  */
      // if(use_barrier){ 
      NumericMatrix carrying_capacity_available_cleaned = clean_matrix(carrying_capacity_available, barriers_map, true, true, true);
      // }
      // else {
        // NumericMatrix carrying_capacity_available_cleaned = clean_matrix(carrying_capacity_available, barriers_map, true, false, false);
      // }
      NumericMatrix tracking_population_state_cleaned = clean_matrix(tracking_population_state, barriers_map, true, true, true);
      // int n_dispersal_cells = total_dispersal_cells(carrying_capacity_available_cleaned);

      /* *********************** */
      /* Dispersal starts here.  */
      /* *********************** */
      loopID = 0;
      for(dispersal_step = 1; dispersal_step <= dispersal_steps; dispersal_step++){

	    /* Set the value of "loopID" for the current iteration of the dispersal loop. */
	    loopID = loopID + 1;

	    /* Source cell search: Can the sink pixel be colonized? There are four
	    ** conditions to be met for a sink pixel to become colonized:
	    **   1. Sink pixel is currently suitable.
	    **	 2. Sink pixel has available carrying capacity.
	    **   3. Sink pixel is within dispersal distance of an already colonised cell.
	    **   4. There is no obstacle (barrier) between the pixel to be colonised
	    **      (sink pixel) and the pixel that is already colonised (source
	    **      pixel).
	    **
	    ** Loop through the cellular automaton. */
	    for(i = 0; i < nrows; i++){
	      for(j = 0; j < ncols; j++){

		    //The are boolean calls which indicate if habitat is suitable and if cell can disperse
	        habitat_is_suitable = false;
	        // cell_in_dispersal_distance = false;

	        /* 1. Test whether the pixel is a suitable sink (i.e., its habitat
	        **    is suitable, it has avaliable carrying capacity, it's not NA and is not on a barrier). */
	        if((habitat_suitability_map(i,j) > 0) && (carrying_capacity_available_cleaned(i,j) > 0) && !R_IsNA(carrying_capacity_available_cleaned(i,j))) habitat_is_suitable = true;

	        /* 2. Test whether there is a source cell within the dispersal
	        **    distance. To be more time efficient, this code runs only if
	        **    the answer to the first question is positive. This is a bit slower,
	        **	  especially if you include barriers in dispersal step.
	        **/
	        if(habitat_is_suitable){
		      /* Now we search if there is a suitable source cell to colonize the sink cell. */
	            IntegerVector cell_in_dispersal_distance = can_source_cell_disperse(i, j, starting_population_state, tracking_population_state_cleaned,
	            habitat_suitability_map, barriers_map, use_barrier, barrier_type, loopID, dispersal_distance, dispersal_kernel);
	            // Rcpp::Rcout << cell_in_dispersal_distance << std::endl;
	 	        /* Update sink cell status. */
    	        if(cell_in_dispersal_distance[0] >= 0 && cell_in_dispersal_distance[1] >= 0){
    		        /* Only if the 2 conditions are fullfilled the cell's is there dispersal to this cell and the population size is changed. */
    		        int source_x = cell_in_dispersal_distance[0];
    		        int source_y = cell_in_dispersal_distance[1];
    		        // Rcpp::Rcout << source_x << std::endl;
    		        int source_pop_dispersed = proportion_of_population_to_disperse(source_x, source_y, i, j, starting_population_state,
    		                                                                        carrying_capacity_available_cleaned, dispersal_proportion);
    		        future_population_state(i,j) = starting_population_state(i,j) + source_pop_dispersed;
    		        // future_population_state(source_x,source_y) = starting_population_state(source_x,source_y) - source_pop_dispersed;
     		        starting_population_state(source_x,source_y) = starting_population_state(source_x,source_y) - source_pop_dispersed;
    	          if(starting_population_state(source_x,source_y)<0)starting_population_state(source_x,source_y)=0;
    	          tracking_population_state_cleaned(i,j) = loopID;
    	          // tracking_population_state_cleaned(source_x,source_y) = loopID;
    	        }
	         }
	      }
	   }
	for(i = 0; i < nrows; i++){
	    for(j = 0; j < ncols; j++){
	        if(R_IsNA(future_population_state(i,j))){
	          future_population_state(i,j)=starting_population_state(i,j);
	          if(future_population_state(i,j)<0)future_population_state(i,j)=0;
	      }
	    }
	  }
	}
  return(List::create(Named("dispersed_population") = future_population_state,
                      Named("tracked_population") = tracking_population_state_cleaned));/* end of dispersal */
}