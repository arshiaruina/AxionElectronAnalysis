#ifndef RAYTRACER_H
#define RAYTRACER_H 1

//c++
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
//root
#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TBox.h"
#include "TStyle.h"
#include "TColor.h"
//
#include "chipRegions.h"
#include "rootConfig.h"

//definitions
#define RAYTRACER_DISTANCE_SUN_EARTH 1.5e14 //mm
#define RAYTRACER_RADIUS_SUN 6.9e11 //mm
#define RAYTRACER_RADIUS_COLDBORE 21.5 //mm
#define RAYTRACER_LENGTH_COLDBORE 9756. //mm half B field to end of CB
#define RAYTRACER_LENGTH_COLDBORE_9T 9260. //mm half B field to half B field
#define RAYTRACER_LENGTH_PIPE_CB_VT4 2571.5 //mm from beam pipe drawings
#define RAYTRACER_RADIUS_PIPE_CB_VT4 30. //mm smallest aperture between end of CB and VT4
#define RAYTRACER_LENGTH_PIPE_VT4_XRT 198.2 //mm from XRT drawing
#define RAYTRACER_RADIUS_PIPE_VT4_XRT 35. //m irrelevant, large enough to not loose anything
#define RAYTRACER_FOCAL_LENGTH_XRT 1600. //mm
#define RAYTRACER_DISTANCE_AXIS_CB_AXIS_XRT 58.44 //mm from XRT drawing
#define RAYTRACER_DISTANCE_FOCAL_PLANE_DETECTOR_WINDOW -10. //mm

namespace axionRadiation{
    // sar: standard axion radiation
    // def: default, currently not implemented
    enum class characteristic{sar, def};

    inline std::string characteristicString(characteristic radiationCharacteristic)
    {
	switch(radiationCharacteristic){
	    case characteristic::sar: return std::string("sar"); 
		break;
	    case characteristic::def: return std::string("def"); 
		break;
	    default: std::cerr << "Error: Unknown radiation characteristic!" << std::endl;
		return std::string("");
	}
    }
}

class rayTracer
{

  public:

    rayTracer(Int_t numberOfPointsEndOfCB = 10000, Int_t numberOfPointsSun = 1000);

    ~rayTracer();


    Int_t calculateFluxFractions(axionRadiation::characteristic axionRadiationCharacteristic, 
				 Double_t xrtTransmissionAt10Arcmin,
				 Double_t detectorWindowAperture = 14.0,
				 Double_t sunMisalignmentH = 0.0, 
				 Double_t sunMisalignmentV = 0.0, 
				 Double_t detectorMisalignmentX = 0.0, 
				 Double_t detectorMisalignmentY = 0.0, 
				 Double_t coldboreBlockedLength = 0.0);

    Double_t getFluxFractionGold();


    Double_t getFluxFractionSilver();

    Double_t getFluxFractionBronze();

    Double_t getFluxFractionDetector();

    Double_t getFluxFractionTotal();

    Double_t getFluxFraction(chipRegions::region chipRegion);

  private:

    TVector3 getRandomPointOnSphericalShell(const TVector3 &center, 
					    const Double_t &radius, 
					    const Double_t &width);

    TVector3 getRandomPointFromSolarModel(const TVector3 &center, 
					  const Double_t &radius);


    TVector3 getRandomPointOnDisk(const TVector3 &center, const Double_t &radius);

    Bool_t lineIntersectsCircle(const TVector3 &point_1, 
				const TVector3 &point_2, 
				const TVector3 &center, 
				const Double_t &radius, 
				TVector3 &intersect);

    Bool_t lineIntersectsCylinderOnce(const TVector3 &point_1, 
				      const TVector3 &point_2, 
				      const TVector3 &centerBegin, 
				      const TVector3 &centerEnd, 
				      const Double_t &radius, 
				      TVector3 &intersect);

    Double_t telescopeTransmission(Double_t angle, Double_t transmissionAt10Arcmin);

    TRandom3* _randomGenerator;

    Int_t _numberOfPointsEndOfCB;

    Int_t _numberOfPointsSun;

    Double_t _fluxFractionGold;
    Double_t _fluxFractionSilver;
    Double_t _fluxFractionBronze;

    Double_t _fluxFractionDetector;
    Double_t _fluxFractionTotal;

};

#endif
