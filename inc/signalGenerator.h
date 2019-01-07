#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H 1

//c++
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <utility>
#include <list>
//root
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TH1D.h>
#include <TRandom3.h>
// interpolation
#include "interpolation.h"

//definitions
#define SIGNALGENERATOR_AREA_COLDBORE 			     1452. //mm²
#define SIGNALGENERATOR_AREATRANSPARENCY_MYLAR 		     0.826
#define SIGNALGENERATOR_AREATRANSPARENCY_SILICONNITRIDE      0.735
#define SIGNALGENERATOR_AXIONSPECTRUM_BINWIDTH 	             5.96316503762e-05 // keV// 0.0199 //keV
#define SIGNALGENERATOR_AXIONSPECTRUM_BINAREA 	             100. // mm^2 //16. //mm²
#define SIGNALGENERATOR_AXIONSPECTRUM_BINTIME 	             86400. //s //3600. //s
#define SIGNALGENERATOR_BETA_GAMMA 			     6.457e10
#define SIGNALGENERATOR_G_A_GAMMA                            1e-5 // set to 1e-11 so that product is O(1e-22 / GeV)
#define SIGNALGENERATOR_G_A_E                                0.511e-10
#define SIGNALGENERATOR_GE_GGAMMA                            (0.511e-10 * 1e-5) // starting value for product of g_ae and g_agamma
#define SIGNALGENERATOR_B_FIELD                              9 // T
#define SIGNALGENERATOR_B_LENGTH                             9.26 // m
#define SIGNALGENERATOR_AXION_MASS                           1e-6 // keV (starting mass for axion, just be smaller than 10meV)
                                                                  // given in keV, since we need it to calculate momentum transfer
#define SIGNALGENERATOR_GEOMETRICAREAXRT                     24.2987 //cm² 

#define SIGNALGENERATOR_AXIONSPECTRUM_PATH 		     "./resources/axion_gae_flux.dat"
#define SIGNALGENERATOR_TRANSMISSIONMYLAR2000NM_PATH	     "./resources/transmission-mylar-2000nm.dat"
#define SIGNALGENERATOR_TRANSMISSIONMYLAR900NM_PATH	     "./resources/transmission-mylar-900nm.dat"
#define SIGNALGENERATOR_TRANSMISSIONALUMINIUM40NM_PATH	     "./resources/transmission-aluminium-40nm.dat"
#define SIGNALGENERATOR_TRANSMISSIONALUMINIUM20NM_PATH	     "./resources/transmission-aluminium-20nm.dat"
#define SIGNALGENERATOR_TRANSMISSIONSILICONNITRIDE200NM_PATH "./resources/transmission-silicon-nitride-200nm.dat"
#define SIGNALGENERATOR_ENERGYRESOLUTIONDETECTOR_PATH        "./resources/energyresolution-detector.dat"
#define SIGNALGENERATOR_TRANSMISSIONARGON30MM_PATH           "./resources/transmission-argon-30mm-1050mbar-295K.dat"
#define SIGNALGENERATOR_EFFECTIVEAREAXRT_PATH                "./resources/effectivearea-xrt.dat"
#define SIGNALGENERATOR_ENERGYRESOLUTIONDETECTOR_PATH        "./resources/energyresolution-detector.dat"

#define SIGNALGENERATOR_ENERGYSMEARING_SAMPLES 		     1000
#define SIGNALGENERATOR_NUMBER_RANGES                        8
#define SIGNALGENERATOR_ENERGY_RANGE0_LOW                    0.15 //keV
#define SIGNALGENERATOR_ENERGY_RANGE0_HIGH                   0.4 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE0              0.45 //sigma_E / E
#define SIGNALGENERATOR_ENERGY_RANGE1_LOW                    0.4 //keV
#define SIGNALGENERATOR_ENERGY_RANGE1_HIGH 		     0.7 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE1 	     0.35 //sigma_E / E
#define SIGNALGENERATOR_ENERGY_RANGE2_LOW 		     0.7 //keV
#define SIGNALGENERATOR_ENERGY_RANGE2_HIGH 		     1.2 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE2 	     0.28 //sigma_E / E
#define SIGNALGENERATOR_ENERGY_RANGE3_LOW 		     1.2 //keV
#define SIGNALGENERATOR_ENERGY_RANGE3_HIGH 		     2.1 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE3 	     0.27 //sigma_E / E
#define SIGNALGENERATOR_ENERGY_RANGE4_LOW 		     2.1 //keV
#define SIGNALGENERATOR_ENERGY_RANGE4_HIGH 		     3.2 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE4 	     0.12 //sigma_E / E
#define SIGNALGENERATOR_ENERGY_RANGE5_LOW 		     3.2 //keV
#define SIGNALGENERATOR_ENERGY_RANGE5_HIGH 		     4.9 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE5 	     0.10 //sigma_E / E
#define SIGNALGENERATOR_ENERGY_RANGE6_LOW 		     4.9 //keV
#define SIGNALGENERATOR_ENERGY_RANGE6_HIGH 		     6.9 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE6 	     0.11 //sigma_E / E
#define SIGNALGENERATOR_ENERGY_RANGE7_LOW 		     6.9 //keV
#define SIGNALGENERATOR_ENERGY_RANGE7_HIGH 		     10.0 //keV
#define SIGNALGENERATOR_ENERGYRESOLUTION_RANGE7 	     0.09 //sigma_E / E

namespace detector{
    enum class windowMaterial{Mylar,SiN,None};

    inline std::string windowMaterialString(windowMaterial material){
	switch(material){
	    case windowMaterial::Mylar: return std::string("Mylar"); break;
	    case windowMaterial::SiN: return std::string("SiN"); break;
	    case windowMaterial::None: return std::string("None"); break;
	    default: std::cerr << "Error: Unknown window material!" << std::endl; return std::string("");
	}
    }

    enum class detectorVersion{mk2,mk4};

    inline std::string detectorVersionString(detectorVersion version){
	switch(version){
	    case detectorVersion::mk2: return std::string("InGrid-CAST-MkII"); break;
	    case detectorVersion::mk4: return std::string("InGrid-CAST-MkIV"); break;
	    default: std::cerr << "Error: Unknown detector version!" << std::endl; return std::string("");
	}
    }
}
    
class signalGenerator{
	
public:
    
    signalGenerator(std::string axionSpectrumPath = SIGNALGENERATOR_AXIONSPECTRUM_PATH,
		    std::string transmissionMylar2000nmPath = SIGNALGENERATOR_TRANSMISSIONMYLAR2000NM_PATH,
		    std::string transmissionMylar900nmPath = SIGNALGENERATOR_TRANSMISSIONMYLAR900NM_PATH,
		    std::string transmissionAluminium40nmPath = SIGNALGENERATOR_TRANSMISSIONALUMINIUM40NM_PATH,
		    std::string transmissionAluminium20nmPath = SIGNALGENERATOR_TRANSMISSIONALUMINIUM20NM_PATH,
		    std::string transmissionSiliconNitride200nmPath = SIGNALGENERATOR_TRANSMISSIONSILICONNITRIDE200NM_PATH,
		    std::string transmissionArgon30mmPath = SIGNALGENERATOR_TRANSMISSIONARGON30MM_PATH,
		    std::string effectiveAreaXRTPath = SIGNALGENERATOR_EFFECTIVEAREAXRT_PATH,
		    std::string energyResolutionDetectorPath = SIGNALGENERATOR_ENERGYRESOLUTIONDETECTOR_PATH);


    ~signalGenerator();

    Int_t generateSignal(TH1D* signalHistogram,
			 detector::windowMaterial detectorWindowMaterial,
			 Double_t detectorWindowThickness,
			 Double_t detectorWindowAreaTransparency,
			 Double_t detectorWindowAlumniumLayerThickness,
			 detector::windowMaterial differentialWindowMaterial,
			 Double_t differentialWindowThickness,
			 Double_t fluxFraction,
			 Double_t softwareEfficiency,
			 Bool_t perfectXRT,
			 Double_t measurementTime,
			 Bool_t smearEnergy = true);


private:

    void setAllTreesToEntry(Int_t entry);

    Double_t getEnergyResolution(Double_t energy);

    Double_t getVisibleEnergy(Double_t energy);

    Double_t getXRTTransmission(Double_t energy);

// function to read axion flux from file and perform axion photon conversion calc.
    std::pair<std::vector<double>, std::vector<double> > readAxionFluxAndConvert(std::string axionSpectrumPath);

    std::vector< std::pair<Double_t, Double_t> > _energyRanges;
    std::vector<Double_t> _energyResolutions;

    TTree* _treeAxionSpectrum;
    TTree* _treeTransmissionMylar2000nm;
    TTree* _treeTransmissionMylar900nm;
    TTree* _treeTransmissionAluminium40nm;
    TTree* _treeTransmissionAluminium20nm;
    TTree* _treeTransmissionSiliconNitride200nm;
    TTree* _treeTransmissionArgon30mm;
    TTree* _treeEffectiveAreaXRT;
    TTree* _treeEnergyResolutionDetector;    

    std::vector<Double_t> _xrtTransmissions;
    std::vector<Double_t> _xrtEnergies;

    std::vector<Double_t> _detectorEnergies;
    std::vector<Double_t> _detectorResolutions;

    TRandom3* _randomGenerator;

    Int_t _entriesTree;

    Float_t _energy;
    Float_t _axionIntensity;
    Float_t _transmissionMylar2000nm;
    Float_t _transmissionMylar900nm;
    Float_t _transmissionAluminium40nm;
    Float_t _transmissionAluminium20nm;
    Float_t _transmissionSiliconNitride200nm;
    Float_t _transmissionArgon30mm;

};

#endif
