#ifndef AXIONELECTRONLIMIT_H
#define AXIONELECTRONLIMIT_H 1

//c++
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
//root
#include "TROOT.h"
#include "TSystem.h"
#include "TLimitDataSource.h"
#include "TVectorD.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
//classes
#include "backgroundExtractor.h"
#include "dataExtractor.h"
#include "signalGenerator.h"
#include "efficiencyCalculator.h"
#include "rayTracer.h"
#include "limitCalculator.h"
#include "chipRegions.h"
#include "dataSets.h"
#include "rootConfig.h"
// help functions
// #include "interpolation.h" // not needed here (only signalGenerator)
//definitions
#define CHAMELEONLIMIT_CHANNEL_HISTOGRAM_MIN 0.2 //keV
#define CHAMELEONLIMIT_CHANNEL_HISTOGRAM_MAX 2.0 //keV
#define CHAMELEONLIMIT_CHANNEL_HISTOGRAM_BINS 9
#define AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN 0.2 //keV
#define AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX 10.0 //keV
#define AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS 25//49
/* #define AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN 0.2 //keV */
/* #define AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX 4.0 //keV */
/* #define AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS 18//49 */
#define AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN 0.2 //keV
#define AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX 10.0 //keV
#define AXIONELECTRONLIMIT_PLOT_HISTOGRAM_BINS 25
/* #define AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN 0.2 //keV */
/* #define AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX 4.0 //keV */
/* #define AXIONELECTRONLIMIT_PLOT_HISTOGRAM_BINS 18 */
#define AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS 6
#define AXIONELECTRONLIMIT_SYSTEMATIC0_NAME "axion spectrum uncertainty"
// TODO: check if these systematics remain the same
#define AXIONELECTRONLIMIT_SYSTEMATIC1_NAME "imaging uncertainty"
#define AXIONELECTRONLIMIT_SYSTEMATIC2_NAME "XRT transmission uncertainty"
#define AXIONELECTRONLIMIT_SYSTEMATIC3_NAME "differential window transmission uncertainty"
#define AXIONELECTRONLIMIT_SYSTEMATIC4_NAME "detector window transmission uncertainty"
#define AXIONELECTRONLIMIT_SYSTEMATIC5_NAME "software efficiency uncertainty"
// TODO: decide if these make sense
#define AXIONELECTRONLIMIT_PLOT_G_A_GAMMA_MAX 1.e-8
#define AXIONELECTRONLIMIT_PLOT_G_A_GAMMA_MIN 1.e-12
#define AXIONELECTRONLIMIT_PLOT_G_A_E_MAX 1.e-10
#define AXIONELECTRONLIMIT_PLOT_G_A_E_MIN 1.e-16
#define AXIONELECTRONLIMIT_SCAN_G_A_GAMMA_MAX 1.e-8
#define AXIONELECTRONLIMIT_SCAN_G_A_GAMMA_MIN 1.e-12
#define AXIONELECTRONLIMIT_SCAN_G_A_GAMMA_STEP 1.e-11
#define AXIONELECTRONLIMIT_SCAN_G_A_E_MAX 1.e-9
#define AXIONELECTRONLIMIT_SCAN_G_A_E_MIN 1.e-11
#define AXIONELECTRONLIMIT_SCAN_G_A_E_STEP 1.e-12
/* #define AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_MAX  1e10//1e-13 */
/* #define AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_MIN  1e-30//5e-14 */
/* #define AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_STEP 1//1e-15 */
#define AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_MAX  5e-15//1e-10
#define AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_MIN  1e-17//1e-12
#define AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_STEP 1e-16
#endif
