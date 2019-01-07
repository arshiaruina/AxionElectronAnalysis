#include <vector>
#include <iterator>
#include <algorithm>
#include <list>
#include <utility>
#include <TTree.h>
// this is the header for the linear interpolation function, we use to interpolate
// the transmission curves to the axion flux array 

// helper function lerp, which gets the interpolated y value for a relative distance t 
// between two points for two y values v0 and v1
double lerp(double v0, double v1, double t);

// the actual linear interpolation function acting on three vectors (x and y input vectors, which
// are to be interpolated) and the x output vector for which to calculate the interpolated points
// if x_vec_out spans a larger range than x_vec_in, we will still only interpolate the range
// of x_vec_in, but for the steps x_vec_out which are in x_vec_in
// thus we return a pair of an x and y vector (use pair.first and pair.second to get 
// back the vectors). the returned x vector is simply the vector containing all x points
// which were actually used of x_vec_out (only different from x_vec_out, if x_vec_out
// spans larger range than x_vec_in). returned y vector is interpolated y vector for these
// x elements
std::pair<std::vector<double>, std::vector<double>> linear_interpolation_function(std::vector<double> x_vec_in, 
										  std::vector<double> y_vec_in, 
										  std::vector<double> x_vec_out);


// function which receives two lists: list of paths to data files and list of names for
// trees. Also an output vector we would like to interpolate to
// creates new trees, performs interpolation by using linear_interpolation_function and
// then adds tree to list. List of trees is returned
std::list<TTree *> read_interpolate_and_create_trees(std::list<const char *> path_list,
						     std::list<const char *> tree_names_list,
						     std::pair<std::vector<double>, std::vector<double> > vecPair);
