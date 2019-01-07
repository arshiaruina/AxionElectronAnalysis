#include "interpolation.h"
#include <iostream>
#include <algorithm>
// implementation of the interpolation function and convenience helper for ROOT trees


// Precise method which guarantees v = v1 when t = 1.
double lerp(double v0, double v1, double t) {
    return (1-t)*v0 + t*v1;
}


std::pair<std::vector<double>, std::vector<double>> linear_interpolation_function(std::vector<double> x_vec_in, 
										  std::vector<double> y_vec_in, 
										  std::vector<double> x_vec_out){
    // this function performs a linear interpolation of two vectors (double vals)
    // for a given output x vector
    // std::vector<double> x_vec_in: input vector containing x input values
    // std::vector<double> y_vec_in: input vector containing y input values
    // std::vector<double> x_vec_out: vector containing x values for desired y output vector
    // interpolation is done by a lerp
    // returns a pair of the interpolated y vector and the corresponding x vector (if ranges of x_in and x_out are
    // different)
    std::vector<double> y_vec_out;
    std::vector<double> x_vec_out_actual;
    // first sort the input vectors
    std::sort(x_vec_in.begin(), x_vec_in.end());
    std::sort(x_vec_out.begin(), x_vec_out.end());

    // first we get the iterators for the elements of x_vec_out, which are
    // within x_vec_in
    std::vector<double>::iterator x_vec_out_lower_iter;
    std::vector<double>::iterator x_vec_out_upper_iter;
    std::vector<double>::iterator x_vec_in_low_bin_iter;
    std::vector<double>::iterator x_vec_in_up_bin_iter;
    std::vector<double>::iterator y_vec_in_low_bin_iter;
    std::vector<double>::iterator y_vec_in_up_bin_iter;
    x_vec_out_lower_iter = std::lower_bound(x_vec_out.begin(), x_vec_out.end(), x_vec_in.front());
    x_vec_out_upper_iter = std::upper_bound(x_vec_out.begin(), x_vec_out.end(), x_vec_in.back());
    // these iterators will now allow us to run over the interesting part of x_vec_out,
    // for which we want to interpolate
	
    // now get iterators of first element of x_vec_in and second (first bin)
    x_vec_in_low_bin_iter = x_vec_in.begin();
    x_vec_in_up_bin_iter  = std::next(x_vec_in_low_bin_iter);
    y_vec_in_low_bin_iter = y_vec_in.begin();
    y_vec_in_up_bin_iter  = std::next(y_vec_in_low_bin_iter);

    // variables needed for actual for loop
    double x_s, x_e, y_s, y_e;
    double t;
    double x_current;
    double y_current;
    double bin_size;

    // set variables corresponding to the values of the x and y bins for the input vectors
    x_s = *x_vec_in_low_bin_iter;
    x_e = *x_vec_in_up_bin_iter;
    y_s = *y_vec_in_low_bin_iter;
    y_e = *y_vec_in_up_bin_iter;

    std::vector<double>::iterator it;
    for(it = x_vec_out_lower_iter; it != x_vec_out_upper_iter; ++it){
	// these are all interesting elements of x_vec_out
	// set x_current variable to current iterator value
	x_current = *it;

	if( (x_current > x_s) &&
	    (x_current < x_e) ){
	    // in this case all is good and we calculate the next value
	    // get x and y values for current bin and interpolate for *it
	    bin_size = x_e - x_s;
	    t = (x_current - x_s) / bin_size;
	    y_current = lerp(y_s, y_e, t);
	    y_vec_out.push_back(y_current);
	    x_vec_out_actual.push_back(x_current);
	}
	else if( (x_current > x_s) &&
		 (x_current >= x_e) ){
	    // in this case, we need to move both x_vec_in iterators one step and y_vec_in iters
	    std::advance(x_vec_in_low_bin_iter, 1);
	    std::advance(x_vec_in_up_bin_iter, 1);
	    x_s = *x_vec_in_low_bin_iter;
	    x_e = *x_vec_in_up_bin_iter;
	    std::advance(y_vec_in_low_bin_iter, 1);
	    std::advance(y_vec_in_up_bin_iter, 1);
	    y_s = *y_vec_in_low_bin_iter;
	    y_e = *y_vec_in_up_bin_iter;
	    // if we were to go to the next iteration now, we'd skip this value of
	    // x_vec_out entirely. Thus set it to --it, so that we get to the same
	    // value of it again next iteration
	    --it;
	}
    }

    std::pair<std::vector<double>, std::vector<double>> pair;
    pair = std::make_pair(x_vec_out_actual, y_vec_out);

    return pair;
}


std::list<TTree *> read_interpolate_and_create_trees(std::list<const char *> path_list,
						     std::list<const char *> tree_names_list,
						     std::pair<std::vector<double>, std::vector<double> > vecPair){
    // this function receives a list of paths to files, creates a tree to read the data
    // create std::vectors for both columns of the data file, interpolates the 
    // vectors to x_vec_out and hands back a new tree with this data
    std::vector<double> x_vec_out;
    x_vec_out = vecPair.first;
    // define vectors for the interpolated vectors
    // note: defined here, so that we have access to x_interp, after the last iteration
    // of the loop over all trees. Need this in order to create the correct data for
    // mainTree.
    // assumption: all trees interpolate to the same x_interp vector in the loop below.
    //             valid, since they all have the exact same x column in the data files
    std::vector<double> x_interp;
    std::vector<double> y_interp;
    
    // define output list for trees
    std::list<TTree *> tree_list;

    // print all tree names listed in list given
    std::for_each( tree_names_list.begin(), tree_names_list.end(), [](const char* name){
	    std::cout << "tree names: " << name << std::endl;
	} );
    
    
    // since we might need to throw out elements of x_vec_out
    // we create a new TTree, which will then be used as the main tree in the calling 
    // function
    TTree *mainTree;
    mainTree = new TTree(tree_names_list.back(), "");
    tree_names_list.pop_back();

    // get list size (now both lists have same size, since we popped element 
    // in line above)
    unsigned int size_list;
    size_list = path_list.size();
    for(unsigned int i = 0; i < size_list; i++){
	// run over all paths and create tree
	TTree tempTree;	
	const char *path;
	int entries;
	path = path_list.back();
	path_list.pop_back();
	entries = tempTree.ReadFile(path, "energy:transmission");
	// now that we have this tree, associate local variables for x and y
	Float_t x, y;
	tempTree.SetBranchAddress("energy", &x);
	tempTree.SetBranchAddress("transmission", &y);
	// and define x and y input vectors...
	std::vector<double> x_vec_in;
	std::vector<double> y_vec_in;
	// ... and get them from tree
	for(int j = 0; j < entries; j++){
	    tempTree.GetEntry(j);
	    // we divide x (energy from file) by 1000 to convert from eV to keV
	    x_vec_in.push_back(x/1000.0);
	    y_vec_in.push_back(y);
	}
	// now we have vectors for x and y, and with x_vec_out, we can perform the interpolation
	std::pair<std::vector<double>, std::vector<double>> pair;
	pair = linear_interpolation_function(x_vec_in, y_vec_in, x_vec_out);
	// now we should have the properly interpolated vectors in our pair
	// use these to create a new tree and add that to tree_list
	TTree *tree_for_list;
	tree_for_list = new TTree(tree_names_list.back(), "");
	tree_names_list.pop_back();
	
	// define vars for branch addresses
	Float_t x_new;
	Float_t y_new;
	tree_for_list->Branch("energy", &x_new, 1000000);
	tree_for_list->Branch("transmission", &y_new, 1000000);
	
	// assign the interpolated vectors from the pair
	x_interp = pair.first;
	y_interp = pair.second;
	
	for(unsigned int j = 0; j < x_interp.size(); j++){
	    // run over all vector elements to fill tree with them...
	    x_new = x_interp[j];
	    y_new = y_interp[j];
	    tree_for_list->Fill();
	}
	tree_list.push_front(tree_for_list);
    }

    // here we can now use x_interp to create the mainTree entries
    // idea: fill tree with all entries of vecPair, which correspond to x_interp
    Float_t xMain;
    Float_t yMain;
    std::vector<double> xVecPair;
    std::vector<double> yVecPair;
    xVecPair = vecPair.first;
    yVecPair = vecPair.second;
    // iterator for the for loop (looping over x_interp)
    std::vector<double>::iterator it;
    // and iterator for x_vec_out and  and set to begin 
    std::vector<double>::iterator it_xVecPair;
    std::vector<double>::iterator it_yVecPair;
    it_xVecPair = xVecPair.begin();
    it_yVecPair = yVecPair.begin();
    mainTree->Branch("energy",    &xMain, 1000000);
    mainTree->Branch("intensity", &yMain, 1000000);
    for(it = x_interp.begin(); it != x_interp.end(); ++it){
	// run over all x_interp elements, advance iterator until found in x_vec_out,
	// fill mainTree
	while(*it != *it_xVecPair){
	    // as long as it_xVecPair does not point to the element it points to,
	    // we advance both x and y iterators
	    std::advance(it_xVecPair, 1);
	    std::advance(it_yVecPair, 1);
	}
	// now it and it_xVecPair point to the same element, fill tree
	xMain = *it_xVecPair;
	yMain = *it_yVecPair;
	mainTree->Fill();
    }
    // at last add mainTree to tree_list and return
    tree_list.push_front(mainTree);

    std::cout << "we'e done interpolating... it seems!!!" << std::endl;
    return tree_list;
}




