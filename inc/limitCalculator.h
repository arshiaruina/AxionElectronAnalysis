#ifndef LIMITCALCULATOR_H
#define LIMITCALCULATOR_H 1

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
#include "TLimit.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TLimitDataSource.h"
#include "TConfidenceLevel.h"
#include "TObjArray.h"
#include "TLatex.h"
#include "TText.h"
#include "TH1.h"
#include "TH1F.h"
#include "TPolyLine.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TArrow.h"
//
#include "rootConfig.h"
//definitions
#define LIMITCALCULATOR_NMC 50000//50000
#define LIMITCALCULATOR_BETA_GAMMA_MIN 0.99e+8
#define LIMITCALCULATOR_BETA_GAMMA_MAX 1.01e+18
#define LIMITCALCULATOR_BETA_M_MIN 0.01
#define LIMITCALCULATOR_BETA_M_MAX 1.001e+16
#define LIMITCALCULATOR_G_A_GAMMA_MIN 0.99e-12
#define LIMITCALCULATOR_G_A_GAMMA_MAX 1.01e-8
#define LIMITCALCULATOR_G_A_E_MIN 0.99e-16
#define LIMITCALCULATOR_G_A_E_MAX 1.01e-10
#define LIMITCALCULATOR_BETA_GAMMA_LIMIT_SDD 9.26076e+10
#define LIMITCALCULATOR_G_A_E_G_A_GAMMA_2013 8.1e-17//23
#define LIMITCALCULATOR_DISTANCEFACTOR_VERTICAL 1.3
#define LIMITCALCULATOR_DISTANCEFACTOR_HORIZONTAL 2.

class limitCalculator
{

  public:
    
    limitCalculator(std::string identifier, TLimitDataSource* dataSource, Bool_t useStatisticErrors, Double_t ge_ggamma);

    ~limitCalculator();

    Int_t calculateCLs95LimitFast(Double_t ge_ggammaCoarseMax,
				  Double_t ge_ggammaCoarseMin,
				  Double_t ge_ggammaCoarseStep,
				  Double_t ge_ggammaFineStep,
				  Bool_t suppressObservedLimit);

    Int_t calculateCLs95Limit(Double_t ge_ggammaMax, 
			      Double_t ge_ggammaMin, 
			      Double_t ge_ggammaStep,
			      Bool_t suppressObservedLimit);

    void createExclusionPlotWithExpectedLimit(Float_t beta_gammaMax);

    void createExclusionPlotWithObservedLimit(Float_t beta_gammaMax);

    Double_t getObservedCLs95Limit();

    Double_t getExpectedCLs95Limit();

    Double_t getExpectedCLs95LimitPlus1Sigma();

    Double_t getExpectedCLs95LimitMinus1Sigma();

    Double_t getObservedCLb();

  private:

    enum class parameter{observed,expected,expectedPlus1Sigma,expectedMinus1Sigma};

    Int_t performCLs95LimitScan(Double_t ge_ggammaMax,
				Double_t ge_ggammaMin,
				Double_t ge_ggammaStep,
				Bool_t saveToFile,
				Bool_t suppressObservedLimit);

    Int_t performIndividualCLs95LimitScan(Int_t numberSteps, Double_t ge_ggammaStep, parameter param);

    // TODO: decide if needs g_agamma
    void rescaleSignals(Double_t ge_ggamma);

    void createExclusionPlot(Bool_t showExpectedLimitOnly, Float_t beta_gammaMax);

    Double_t getCenteredValueOnLogScale(Double_t min, Double_t max)
    {
	Double_t center;
	center = TMath::Exp(TMath::Log(min)+((TMath::Log(max)-TMath::Log(min))/2.));
	return center;
    }

    std::string _identifier;

    TLimitDataSource* _dataSource;
    TRandom3* _randomGenerator;
    Bool_t _useStatisticErrors;

    Double_t _ge_ggammaOriginal;
    Double_t _ge_ggammaCurrent;

    Double_t _observedCLb;
    Double_t _observedCLs95Limit;
    Double_t _expectedCLs95Limit;
    Double_t _expectedCLs95LimitPlus1Sigma;
    Double_t _expectedCLs95LimitMinus1Sigma;

};

#endif
