#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>          // mandatory for myPyObject.cast<std::vector<T>>()
#include <pybind11/functional.h>
#include <complex>
// #include <pybind11/pybind11.h>

namespace py = pybind11;

std::vector<std::complex<double>> callPy(std::vector<double> poly){
    py::scoped_interpreter guard{};
    std::cout << "import numpy" << std::endl;
    py::module np = py::module::import("numpy");
    std::cout << "cast poly" << std::endl;
    py::array_t<double> polyNumpy = py::cast(poly);
    std::cout << "access roots" << std::endl;
    py::object roots = np.attr("roots");
    std::cout << "call roots" << std::endl;
    py::object retVal = roots(polyNumpy);
    std::cout << "echo result" << std::endl;
    std::cout << retVal << std::endl;

    return retVal.cast<std::vector<std::complex<double>>>();

}


int main(){

    std::vector<double> poly = {-0.008,0.208,-1.667,5.000,-5.000,1.000};
    //std::vector<double> poly = {1.0, 2.0, 3.0, 4.0};
    //auto result =

    std::cout << "Before call" << std::endl;

    auto retVal = callPy(poly);
    std::cout << "Resulting vector " << std::endl;
    for(auto val : retVal){
	std::cout << "val : " << val << std::endl;
    }
    return 0;
}
