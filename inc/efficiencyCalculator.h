#ifndef EFFICIENCYCALCULATOR_H
#define EFFICIENCYCALCULATOR_H 1

//c++
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <utility>
//root
#include "TROOT.h"
#include "TSystem.h"
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
//
#include "chipRegions.h"
//definitions
#define EFFICIENCYCALCULATOR_NUMBER_RANGES 8
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE0 "calibration-cdl-apr2014-C-EPIC-0.6kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE0_LOW 0.15
#define EFFICIENCYCALCULATOR_ENERGY_RANGE0_HIGH 0.4
#define EFFICIENCYCALCULATOR_CHARGE_RANGE0_MIN 0.
#define EFFICIENCYCALCULATOR_CHARGE_RANGE0_MAX 5.e4
#define EFFICIENCYCALCULATOR_RMSY_RANGE0_MIN 0.1
#define EFFICIENCYCALCULATOR_RMSY_RANGE0_MAX 20.
#define EFFICIENCYCALCULATOR_LENGTH_RANGE0_MAX 6.
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE1 "calibration-cdl-apr2014-Cu-EPIC-0.9kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE1_LOW 0.4
#define EFFICIENCYCALCULATOR_ENERGY_RANGE1_HIGH 0.7
#define EFFICIENCYCALCULATOR_CHARGE_RANGE1_MIN 3.e4
#define EFFICIENCYCALCULATOR_CHARGE_RANGE1_MAX 8.e4
#define EFFICIENCYCALCULATOR_RMSY_RANGE1_MIN 0.0
#define EFFICIENCYCALCULATOR_RMSY_RANGE1_MAX 1.1
#define EFFICIENCYCALCULATOR_LENGTH_RANGE1_MAX 6.
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE2 "calibration-cdl-apr2014-Cu-EPIC-2kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE2_LOW 0.7
#define EFFICIENCYCALCULATOR_ENERGY_RANGE2_HIGH 1.2
#define EFFICIENCYCALCULATOR_CHARGE_RANGE2_MIN 7.e4
#define EFFICIENCYCALCULATOR_CHARGE_RANGE2_MAX 1.3e5
#define EFFICIENCYCALCULATOR_RMSY_RANGE2_MIN 0.0
#define EFFICIENCYCALCULATOR_RMSY_RANGE2_MAX 1.1
#define EFFICIENCYCALCULATOR_LENGTH_RANGE2_MAX 7.
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE3 "calibration-cdl-apr2014-Al-Al-4kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE3_LOW 1.2
#define EFFICIENCYCALCULATOR_ENERGY_RANGE3_HIGH 2.1
#define EFFICIENCYCALCULATOR_CHARGE_RANGE3_MIN 9.e4
#define EFFICIENCYCALCULATOR_CHARGE_RANGE3_MAX 2.1e5
#define EFFICIENCYCALCULATOR_RMSY_RANGE3_MIN 0.0
#define EFFICIENCYCALCULATOR_RMSY_RANGE3_MAX 1.1
#define EFFICIENCYCALCULATOR_LENGTH_RANGE3_MAX 7.
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE4 "calibration-cdl-apr2014-Ag-Ag-6kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE4_LOW 2.1
#define EFFICIENCYCALCULATOR_ENERGY_RANGE4_HIGH 3.2
#define EFFICIENCYCALCULATOR_CHARGE_RANGE4_MIN 2.e5
#define EFFICIENCYCALCULATOR_CHARGE_RANGE4_MAX 4.e5
#define EFFICIENCYCALCULATOR_RMSY_RANGE4_MIN 0.0
#define EFFICIENCYCALCULATOR_RMSY_RANGE4_MAX 1.1
#define EFFICIENCYCALCULATOR_LENGTH_RANGE4_MAX 7.
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE5 "calibration-cdl-apr2014-Ti-Ti-9kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE5_LOW 3.2
#define EFFICIENCYCALCULATOR_ENERGY_RANGE5_HIGH 4.9
#define EFFICIENCYCALCULATOR_CHARGE_RANGE5_MIN 2.9e5
#define EFFICIENCYCALCULATOR_CHARGE_RANGE5_MAX 5.5e5
#define EFFICIENCYCALCULATOR_RMSY_RANGE5_MIN 0.0
#define EFFICIENCYCALCULATOR_RMSY_RANGE5_MAX 1.1
#define EFFICIENCYCALCULATOR_LENGTH_RANGE5_MAX 7.
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE6 "calibration-cdl-apr2014-Mn-Cr-12kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE6_LOW 4.9
#define EFFICIENCYCALCULATOR_ENERGY_RANGE6_HIGH 6.9
#define EFFICIENCYCALCULATOR_CHARGE_RANGE6_MIN 3.5e5
#define EFFICIENCYCALCULATOR_CHARGE_RANGE6_MAX 6.e5
#define EFFICIENCYCALCULATOR_RMSY_RANGE6_MIN 0.0
#define EFFICIENCYCALCULATOR_RMSY_RANGE6_MAX 1.1
#define EFFICIENCYCALCULATOR_LENGTH_RANGE6_MAX 7.
#define EFFICIENCYCALCULATOR_ROOTTREE_RANGE7 "calibration-cdl-apr2014-Cu-Ni-15kV"
#define EFFICIENCYCALCULATOR_ENERGY_RANGE7_LOW 6.9
#define EFFICIENCYCALCULATOR_ENERGY_RANGE7_HIGH 10.0
#define EFFICIENCYCALCULATOR_CHARGE_RANGE7_MIN 5.9e5
#define EFFICIENCYCALCULATOR_CHARGE_RANGE7_MAX 1.e6
#define EFFICIENCYCALCULATOR_RMSY_RANGE7_MIN 0.0
#define EFFICIENCYCALCULATOR_RMSY_RANGE7_MAX 1.1
#define EFFICIENCYCALCULATOR_LENGTH_RANGE7_MAX 7.

class efficiencyCalculator
{

 public:
  
  efficiencyCalculator(std::string efficiencyRootFilePath);

  ~efficiencyCalculator();

  Int_t findEfficiencySetting(chipRegions::region chipRegion, Double_t softwareEfficiency, Double_t *efficiencySetting);

  Int_t evaluateEfficiencySetting(chipRegions::region chipRegion, Double_t *efficiencySetting, Double_t *softwareEfficiencies);

 private:

  Double_t findLikelihoodMarlinCutValue(Double_t softwareEfficiency);

  Double_t getSoftwareEfficiency(Double_t likelihoodMarlinCutValue);

  Int_t generateLikelihoodMarlinDistribution(TTree* rootTree, chipRegions::region chipRegion, Double_t totalChargeMin, Double_t totalChargeMax, Double_t lengthMax, Double_t rmsYMin, Double_t rmsYMax);

  TTree* getAndSetupRootTree(std::string rootTreeName);

  TFile* _rootFile;
  std::vector<TTree*> _rootTrees;
  std::vector<std::pair<Double_t,Double_t> > _energyRanges;
  std::vector<std::pair<Double_t,Double_t> > _chargeRanges;
  std::vector<std::pair<Double_t,Double_t> > _rmsYRanges;
  std::vector<Double_t> _lengthMaxs;

  TH1D* _likelihoodMarlinDistribution;

  Double_t _energyFromCharge;
  Double_t _likelihoodMarlin;
  Double_t _positionX;
  Double_t _positionY;
  Double_t _length;
  Double_t _rmsY;
  Int_t _totalCharge;
  Int_t _numberOfPixels;

};

#endif
