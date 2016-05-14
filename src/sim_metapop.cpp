#include "RcppArmadillo.h"
using namespace Rcpp;

//'C++ dispersal function
//' @param dist Distances between patches (symetrical matrix)
//' @param alpha Exponential decay rate of patch connectivity (dispersion parameter)
//' @param beta double parameter that represents the shape of the dispersal kernel.
//' @param hanski_dispersal_kernal bool if true uses hanski(1994), if false uses shaw(1995).
//' @export
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::export]]
NumericMatrix meta_dispersal_fun(NumericMatrix dist, double alpha, 
                                 double  beta=1, bool hanski_dispersal_kernal = true) {
  arma::mat dist1 = as<arma::mat>(dist);
  arma::mat disp_mat(dist1.n_rows,dist1.n_cols);
  if(hanski_dispersal_kernal == true) disp_mat = exp(-alpha * dist1);
  if(hanski_dispersal_kernal == false) disp_mat = 1/(1+(alpha * arma::pow(dist1,beta)));
  return(wrap(disp_mat));
}


//'C++ metapopulation function for a single timestep.
//' @param presence NumericVector Initial occupancy of each patch
//' @param dist_mat Exponential decay rate of patch connectivity (dispersion parameter)
//' @param Ei patch extinction rate at time i. note: In the future I need to pull this from demographic model.
//' @param y incidence function parameters.
//' @export
// [[Rcpp::export]]
NumericVector metapop(NumericVector presence, NumericMatrix dist_mat, NumericVector Ei, double y){
  int presences = presence.length();
  NumericVector s(presences);
  for(int i=0; i < presences; i++) {
    if(presence[i]>0) s[i] = sum(dist_mat(_,i));
    else s[i] = NA_REAL;
  }
  NumericVector si = Rcpp::na_omit(s);
  NumericVector pa(presences); 
  NumericVector c = Rcpp::pow(si,2)/(Rcpp::pow(si,2) + (y*y));
  for (int j=0; j < presences; j++) {
    if (presence[j] == 0 && R::runif(0,1)  < c[j])
      presence[j] = 1;
    else if (presence[j] == 1 && R::runif(0,1) < ((1 - c[j]) * Ei[j]))
      presence[j] = 0;
  }
  return(presence);
}

//'Simulate a metapopulation system in C++
//'this function is not complete.
//' @param time Number of time steps
//' @param dist Distances between patches (symetrical matrix)
//' @param area Area of patches - This needs to be calculated somehow - using occupancy models?
//' @param presence Initial occupancies of patches. Must be presence 1 or absence 0.
//' @param y incidence function parameters
//' @param x incidence function parameters
//' @param e Minimum area of patches
//' @param alpha Exponential decay rate of patch connectivity (dispersion parameter)
//' @param beta double parameter that represents the shape of the dispersal kernel.
//' @param hanski_dispersal_kernal bool if true uses hanski(1994), if false uses shaw(1995).
//' @param locations NULL or NumericMatrix Longitudes and latitudes of coordinates of the patches
//' @export
// [[Rcpp::export]]
NumericMatrix metapop_n(int time, NumericMatrix dist, NumericVector area, NumericVector presence, 
                 int y = 1, int x = 1, int e=1, double alpha = 1, double beta = 1, bool hanski_dispersal_kernal = true,
                 Rcpp::Nullable<Rcpp::NumericMatrix> locations = R_NilValue){
 arma::mat dist_mat = as<arma::mat>(meta_dispersal_fun(dist,alpha,beta,hanski_dispersal_kernal));
 dist_mat.diag().zeros();
 NumericMatrix dist_mat2 = wrap(dist_mat);
 int presences = presence.length();
 NumericMatrix presence_mat(presences,time+1);
 NumericVector E = e/pow(area,x);
 // Rcpp::Rcout << E << std::endl;
 NumericVector Ei = ifelse(E > 1, 1, E);
 // Rcpp::Rcout << Ei << std::endl;
 presence_mat(_,0) = presence;
 for (int i=0; i<time;i++){
   presence_mat(_,i + 1) = metapop(presence_mat(_,i),dist_mat2, Ei, y);
 }
 return(presence_mat);
}
