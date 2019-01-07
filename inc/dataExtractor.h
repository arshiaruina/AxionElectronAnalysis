#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H 1

//c++
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
//root
#include "TROOT.h"
#include "TSystem.h"
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
//
#include "chipRegions.h"
#include "efficiencyCalculator.h"

class dataExtractor
{

 public:

  dataExtractor(std::string dataRootFilePath, std::string dataRootTreeName);

  ~dataExtractor();

  Int_t extractData(TH1D* dataHistogram, Bool_t setStatisticalErrors, chipRegions::region chipRegion, Double_t *efficiencySetting);

  // public function to call in order to dump the data rate stored in the internal root tree
  void dumpDataRate(std::string filename,
		    Double_t measurementTime, 
		    chipRegions::region chipRegion,
		    Double_t *efficiencySettings,
		    Double_t energyMin,
		    Double_t energyMax,
		    Int_t energyBins);


  // public (so that backgroundExtractor can call it) function which is called internally after call to dumpDataRate
  void dumpRatePlotFromHisto(std::string filename,
			     Double_t measurementTime, 
			     TH1D *histogram,
			     chipRegions::region chipRegion,
			     Double_t energyMin,
			     Double_t energyMax,
			     Int_t energyBins);

 private:

  TFile* _rootFile;
  TTree* _rootTree;

  Int_t _numberOfEvents;
  Double_t _energyFromCharge;
  Double_t _likelihoodMarlin;
  Double_t _positionX;
  Double_t _positionY;
  Double_t _rmsY;
  Int_t _numberOfPixels;

};

#endif
