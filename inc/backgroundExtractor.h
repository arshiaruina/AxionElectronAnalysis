#ifndef BACKGROUNDEXTRACTOR_H
#define BACKGROUNDEXTRACTOR_H 1

//c++
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
//root
#include "TROOT.h"
#include "TSystem.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
//
#include "chipRegions.h"
#include "dataExtractor.h"
#include "axionElectronLimit.h"
//defines
//#define AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS 20//49

class backgroundExtractor
{

 public:

  backgroundExtractor(std::string rootFilePath, std::string rootTreeName, Double_t measurementTime);

  ~backgroundExtractor();

  void printBackgroundRate(TH1D *backgroundHistogram, Double_t measurementTime);


  Int_t extractBackground(TH1D *backgroundHistogram, Bool_t setStatisticalErrors, chipRegions::region chipRegion, Double_t measurementTime, Double_t *efficiencySetting);

  // public function to call, which dumps the background rate stored in the root tree of the internal dataExtractor to file
  void dumpBackgroundRate(std::string filename,
			  chipRegions::region chipRegion,
			  Double_t *efficiencySettings,
			  Double_t energyMin,
			  Double_t energyMax,
			  Int_t energyBins);


  TGraphAsymmErrors* getBackgroundratePlot(chipRegions::region chipRegion, Double_t *efficiencySetting, Double_t energyMin, Double_t energyMax, Int_t energyBins, EColor color = kBlue, Bool_t prepareForLogPlot = false);

 private:

  Double_t _measurementTime;
  dataExtractor* _dataExtractor;

};

#endif
