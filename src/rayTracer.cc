#include "rayTracer.h"

rayTracer::rayTracer(Int_t numberOfPointsEndOfCB, Int_t numberOfPointsSun) :
    _numberOfPointsEndOfCB(numberOfPointsEndOfCB),
    _numberOfPointsSun(numberOfPointsSun)
{
    // Creator of the ray tracer object. 
    // creates and initializes TRandom3 object and inits flux fraction member 
    // variables to 0
    _randomGenerator = new TRandom3();
    _randomGenerator->SetSeed(0);

    _fluxFractionGold = 0.;
    _fluxFractionSilver = 0.;
    _fluxFractionBronze = 0.;
    _fluxFractionDetector = 0.;
    _fluxFractionTotal = 0.;
}

rayTracer::~rayTracer()
{
    delete _randomGenerator;
}

Double_t rayTracer::getFluxFractionGold()
{
    return _fluxFractionGold;
}

Double_t rayTracer::getFluxFractionSilver()
{
    return _fluxFractionSilver;
}

Double_t rayTracer::getFluxFractionBronze()
{
    return _fluxFractionBronze;
}

Double_t rayTracer::getFluxFractionDetector()
{
    return _fluxFractionDetector;
}

Double_t rayTracer::getFluxFractionTotal()
{
    return _fluxFractionTotal;
}

Double_t rayTracer::getFluxFraction(chipRegions::region chipRegion)
{
    switch(chipRegion)
    {
	case chipRegions::region::gold:
	    return _fluxFractionGold;
	    break;
	case chipRegions::region::silver:
	    return _fluxFractionSilver;
	    break;
	case chipRegions::region::bronze:
	    return _fluxFractionBronze;
	    break;
	case chipRegions::region::goldPlusSilver:
	    return _fluxFractionGold + _fluxFractionSilver;
	    break;
	case chipRegions::region::goldPlusSilverPlusBronze:
	    return _fluxFractionGold + _fluxFractionSilver + _fluxFractionBronze;
	    break;
	case chipRegions::region::chip:
	    std::cout << "getFluxFraction() NOTE: flux fraction set to 1 for whole chip!" << std::endl;
	    return 1.;
	    break;
	default:
	    std::cerr << "Error: Unknown chip region!" << std::endl;
	    return 0.;
    }
}

Int_t rayTracer::calculateFluxFractions(axionRadiation::characteristic axionRadiationCharacteristic, 
					Double_t xrtTransmissionAt10Arcmin,
					Double_t detectorWindowAperture,
					Double_t sunMisalignmentH, 
					Double_t sunMisalignmentV, 
					Double_t detectorMisalignmentX, 
					Double_t detectorMisalignmentY, 
					Double_t coldboreBlockedLength)
{
    TVector3 misalignmentSun(TMath::Tan(sunMisalignmentH / 60. * TMath::DegToRad()) * RAYTRACER_DISTANCE_SUN_EARTH, 
			     TMath::Tan(sunMisalignmentV / 60. * TMath::DegToRad()) * RAYTRACER_DISTANCE_SUN_EARTH,
			     0.);
    TVector3 misalignmentDetector(detectorMisalignmentX, detectorMisalignmentY, 0.);

    TVector3 centerSun(0.,0.,RAYTRACER_DISTANCE_SUN_EARTH);
    centerSun+=misalignmentSun;
    
    Double_t radiusSun(RAYTRACER_RADIUS_SUN);

    TVector3 centerEntranceCB(0.,0.,coldboreBlockedLength);
    TVector3 centerExitCBMagneticField(0.,0.,RAYTRACER_LENGTH_COLDBORE_9T);
    TVector3 centerExitCB(0.,0.,RAYTRACER_LENGTH_COLDBORE);
    Double_t radiusCB(RAYTRACER_RADIUS_COLDBORE);

    TVector3 centerExitPipeCBVT4(0.,0.,RAYTRACER_LENGTH_COLDBORE + RAYTRACER_LENGTH_PIPE_CB_VT4);
    Double_t radiusPipeCBVT4(RAYTRACER_RADIUS_PIPE_CB_VT4);

    TVector3 centerExitPipeVT4XRT(0.,0.,RAYTRACER_LENGTH_COLDBORE + RAYTRACER_LENGTH_PIPE_CB_VT4 + RAYTRACER_LENGTH_PIPE_VT4_XRT);
    Double_t radiusPipeVT4XRT(RAYTRACER_RADIUS_PIPE_VT4_XRT);

    Double_t distanceCBAxisXRTAxis(RAYTRACER_DISTANCE_AXIS_CB_AXIS_XRT);

    Double_t integralNormalisation(0.);
    Double_t integralTotal(0.);
    Double_t integralDetector(0.);
    Double_t integralBronze(0.);
    Double_t integralSilver(0.);
    Double_t integralGold(0.);

    Bool_t misalignment = (sunMisalignmentH != 0.0) || (sunMisalignmentV != 0.0) || (detectorMisalignmentX != 0.0) || (detectorMisalignmentY != 0.0);

    std::string plotName;
    plotName = axionRadiation::characteristicString(axionRadiationCharacteristic) + "-txrtXX-" + std::to_string((Int_t)(xrtTransmissionAt10Arcmin * 100. + 0.5));
    if(misalignment)
    {
	plotName += "-" + std::to_string(sunMisalignmentH) + "'";
	plotName += "-" + std::to_string(sunMisalignmentV) + "'";
	plotName += "-" + std::to_string(detectorMisalignmentX) + "mm";
	plotName += "-" + std::to_string(detectorMisalignmentY) + "mm";
    }
    plotName += "-axion-image";

    //canvas graphics area dimensions
    Double_t w(ROOTCONFIG_SQUARECANVAS_WIDTH);
    Double_t h(ROOTCONFIG_SQUARECANVAS_HEIGHT);

    Double_t l(ROOTCONFIG_SQUARECANVAS_MARGIN_LEFT);
    Double_t r(ROOTCONFIG_SQUARECANVAS_MARGIN_RIGHT);
    Double_t b(ROOTCONFIG_SQUARECANVAS_MARGIN_BOTTOM);
    Double_t t(ROOTCONFIG_SQUARECANVAS_MARGIN_TOP);

    rootConfig::applySettings();

    gDirectory->cd(0);

    TCanvas* canvas = new TCanvas("canvas","",w,h);
    canvas->SetWindowSize(w+(w-canvas->GetWw()),h+(h-canvas->GetWh()));
    canvas->SetMargin(l,r,b,t);

    TH2D* image = new TH2D("image","",100,CHIPREGIONS_CHIP_X_MIN,CHIPREGIONS_CHIP_X_MAX,100,CHIPREGIONS_CHIP_Y_MIN,CHIPREGIONS_CHIP_Y_MAX);

    for(Int_t iExitCB = 0; iExitCB < _numberOfPointsEndOfCB; iExitCB++)
    {
	TVector3 pointExitCBMagneticField = getRandomPointOnDisk(centerExitCBMagneticField,radiusCB);

	for(Int_t iSun = 0; iSun < _numberOfPointsSun; iSun++)
	{
	    integralNormalisation++;

	    TVector3 pointInSun;

	    switch(axionRadiationCharacteristic)
	    {
		case axionRadiation::characteristic::sar:
		    pointInSun = getRandomPointFromSolarModel(centerSun,radiusSun);
		    break;
		case axionRadiation::characteristic::def:
		    // nothing implemented here
		    //pointInSun = centerSun;
		    std::cerr << "Error: Default radiation characteristic not implemented" << std::endl;
		    return 1;		    
		    //break;
		default:
		    std::cerr << "Error: Unknown axion radiation characteristic" << std::endl;
		    return 1;
	    }

	    TVector3 intersect;
	    Double_t pathCB(0.);

	    Bool_t intersectsEntranceCB = lineIntersectsCircle(pointInSun,pointExitCBMagneticField,centerEntranceCB,radiusCB,intersect);
	    Bool_t intersectsCB = false;

	    if(!intersectsEntranceCB)
	    {
		intersectsCB = lineIntersectsCylinderOnce(pointInSun,pointExitCBMagneticField,centerEntranceCB,centerExitCBMagneticField,radiusCB,intersect);
	    }

	    if(!intersectsEntranceCB && !intersectsCB){continue;}

	    pathCB = pointExitCBMagneticField.Z() - intersect.Z();

	    TVector3 pointExitCB;

	    if(!lineIntersectsCircle(pointInSun,pointExitCBMagneticField,centerExitCB,radiusCB,pointExitCB)){continue;}

	    TVector3 pointExitPipeCBVT4;

	    if(!lineIntersectsCircle(pointExitCBMagneticField,pointExitCB,centerExitPipeCBVT4,radiusPipeCBVT4,pointExitPipeCBVT4)){continue;}

	    TVector3 pointExitPipeVT4XRT;

	    if(!lineIntersectsCircle(pointExitCB,pointExitPipeCBVT4,centerExitPipeVT4XRT,radiusPipeVT4XRT,pointExitPipeVT4XRT)){continue;}

	    TVector3 vectorBeforeXRT = pointExitPipeVT4XRT - pointExitCB;

	    //Changing COS now: CB -> XRT
	    //====================================================================================================

	    TVector3 pointEntranceXRT(pointExitPipeVT4XRT.X(),pointExitPipeVT4XRT.Y() + distanceCBAxisXRTAxis,pointExitPipeVT4XRT.Z());

	    Double_t angle = vectorBeforeXRT.Theta();

	    Double_t r_x = pointEntranceXRT.X();
	    Double_t r_y = pointEntranceXRT.Y();

	    Double_t theta_x = vectorBeforeXRT.X() / vectorBeforeXRT.Z();
	    Double_t theta_y = vectorBeforeXRT.Y() / vectorBeforeXRT.Z();

	    Double_t theta_x_prime = theta_x - ( r_x / RAYTRACER_FOCAL_LENGTH_XRT);
	    Double_t theta_y_prime = theta_y - ( r_y / RAYTRACER_FOCAL_LENGTH_XRT);

	    TVector3 vectorAfterXRT(TMath::Sin(theta_x_prime) * 100., TMath::Sin(theta_y_prime) * 100., 100.);

	    TVector3 centerDetectorWindow(0.,0.,pointEntranceXRT.Z() + RAYTRACER_FOCAL_LENGTH_XRT + RAYTRACER_DISTANCE_FOCAL_PLANE_DETECTOR_WINDOW);

	    Double_t lambda = ( centerDetectorWindow.Z() - pointEntranceXRT.Z() ) / vectorAfterXRT.Z();

	    TVector3 pointDetectorWindow = pointEntranceXRT + lambda * vectorAfterXRT;
	    pointDetectorWindow-=misalignmentDetector;

	    Double_t weight = ( telescopeTransmission(angle,xrtTransmissionAt10Arcmin) * 
				(pathCB * pathCB / RAYTRACER_LENGTH_COLDBORE_9T / RAYTRACER_LENGTH_COLDBORE_9T) );

	    integralTotal += weight;

	    //detector COS has (0/0) at the bottom left corner of the chip
	    pointDetectorWindow.SetX(pointDetectorWindow.X() + CHIPREGIONS_CHIP_CENTER_X);
	    pointDetectorWindow.SetY(pointDetectorWindow.Y() + CHIPREGIONS_CHIP_CENTER_Y);

	    Bool_t gold = ( (pointDetectorWindow.X() >= CHIPREGIONS_GOLD_X_MIN) && 
			    (pointDetectorWindow.X() <= CHIPREGIONS_GOLD_X_MAX) &&
			    (pointDetectorWindow.Y() >= CHIPREGIONS_GOLD_Y_MIN) && 
			    (pointDetectorWindow.Y() <= CHIPREGIONS_GOLD_Y_MAX) );

	    Double_t r_xy = TMath::Sqrt( ( (pointDetectorWindow.X() - CHIPREGIONS_CHIP_CENTER_X) * 
					   (pointDetectorWindow.X() - CHIPREGIONS_CHIP_CENTER_X) ) +
					 ( (pointDetectorWindow.Y() - CHIPREGIONS_CHIP_CENTER_Y) * 
					   (pointDetectorWindow.Y() - CHIPREGIONS_CHIP_CENTER_Y) ) );

	    Bool_t silver = ( r_xy <= CHIPREGIONS_SILVER_RADIUS_MAX ) && !gold;
	    Bool_t bronze = !gold && !silver && ( r_xy <= CHIPREGIONS_BRONZE_RADIUS_MAX );
	    Bool_t withinWindow = r_xy < detectorWindowAperture/2;
	    Bool_t detector = ( (pointDetectorWindow.X() >= CHIPREGIONS_CHIP_X_MIN) && 
				(pointDetectorWindow.X() <= CHIPREGIONS_CHIP_X_MAX) &&
				(pointDetectorWindow.Y() >= CHIPREGIONS_CHIP_Y_MIN) && 
				(pointDetectorWindow.Y() <= CHIPREGIONS_CHIP_Y_MAX) );

	    if(gold && withinWindow){integralGold+=weight;}
	    if(silver && withinWindow){integralSilver+=weight;}
	    if(bronze && withinWindow){integralBronze+=weight;}
	    if(detector && withinWindow){integralDetector+=weight;}

	    if(withinWindow){
		image->Fill(pointDetectorWindow.X(),pointDetectorWindow.Y(),weight);
	    }
	}
    }

    _fluxFractionTotal    = integralTotal    / integralNormalisation;
    _fluxFractionDetector = integralDetector / integralNormalisation;
    _fluxFractionBronze   = integralBronze   / integralNormalisation;
    _fluxFractionSilver   = integralSilver   / integralNormalisation;
    _fluxFractionGold     = integralGold     / integralNormalisation;

    image->GetXaxis()->SetTitle("x [mm]");
    image->GetXaxis()->CenterTitle(true);
    image->GetYaxis()->SetTitle("y [mm]");
    image->GetYaxis()->CenterTitle(true);
    image->GetZaxis()->SetTitle("Flux fraction");
    image->GetZaxis()->CenterTitle(true);
    image->Scale(1./integralNormalisation);
    image->Draw("colz");

    TBox* box = new TBox(CHIPREGIONS_GOLD_X_MIN, 
			 CHIPREGIONS_GOLD_Y_MIN, 
			 CHIPREGIONS_GOLD_X_MAX, 
			 CHIPREGIONS_GOLD_Y_MAX);
    box->SetLineColor(kRed);
    box->SetLineStyle(1);
    box->SetLineWidth(3);
    box->SetFillStyle(0);
    box->Draw();

    TEllipse* circle = new TEllipse(CHIPREGIONS_CHIP_CENTER_X,
				    CHIPREGIONS_CHIP_CENTER_Y,
				    CHIPREGIONS_SILVER_RADIUS_MAX);
    circle->SetLineColor(kRed);
    circle->SetLineStyle(1);
    circle->SetLineWidth(3);
    circle->SetFillStyle(0);
    // NOTE: don't draw silver region
    //circle->Draw();

    TEllipse* circleDotted = new TEllipse(CHIPREGIONS_CHIP_CENTER_X,
					  CHIPREGIONS_CHIP_CENTER_Y,
					  CHIPREGIONS_BRONZE_RADIUS_MAX);
    circleDotted->SetLineColor(kRed);
    circleDotted->SetLineStyle(2);
    circleDotted->SetLineWidth(3);
    circleDotted->SetFillStyle(0);
    // NOTE: don't draw bronze region
    // circleDotted->Draw();

    canvas->Print(("results/" + plotName + ".pdf").c_str());
    canvas->Print(("results/" + plotName + ".png").c_str());
    canvas->Print(("results/" + plotName + ".eps").c_str());
    canvas->Print(("results/" + plotName + ".svg").c_str());

    delete image;
    delete box;
    delete circle;
    delete circleDotted;
    delete canvas;

    return 0;
}

TVector3 rayTracer::getRandomPointOnSphericalShell(const TVector3 &center, const Double_t &radius, const Double_t &width)
{
    Double_t x,y,z;
    Double_t r;

    if(radius > 0.)
    {
	Double_t r_min = radius - width/2.;
	Double_t r_max = radius + width/2.;
	Double_t uniform = TMath::Power(r_min/r_max,3.) + (1.-TMath::Power(r_min/r_max,3.)) * _randomGenerator->Rndm();
	r = r_max * TMath::Power(uniform,1./3.);
    }
    else
    {
	r = radius;
    }

    _randomGenerator->Sphere(x,y,z,r);

    TVector3 vector(x,y,z);

    vector += center;

    return vector;
}

TVector3 rayTracer::getRandomPointFromSolarModel(const TVector3 &center, const Double_t &radius)
{
    // this function returns a random point from a sphere
    Double_t x,y,z;
    Double_t r;

    //r = TMath::Power(radius*_randomGenerator->Rndm(), 1./3.);
    
    // in case of the standard axion radiation, we use 1/100 of the solar radius as 
    // the origin of axion radiation. In that region we assume homogeneous emission

    r = radius * 1e-1 * _randomGenerator->Rndm();
    //r = radius * _randomGenerator->Rndm();

    _randomGenerator->Sphere(x,y,z,r);

    TVector3 vector(x,y,z);

    vector += center;

    return vector;
}

TVector3 rayTracer::getRandomPointOnDisk(const TVector3 &center, const Double_t &radius)
{
    Double_t x,y;
    Double_t r = radius * TMath::Sqrt(_randomGenerator->Rndm());

    _randomGenerator->Circle(x,y,r);

    TVector3 vector(x,y,0.);

    vector += center;

    return vector;
}

Bool_t rayTracer::lineIntersectsCircle(const TVector3 &point_1, const TVector3 &point_2, const TVector3 &center, const Double_t &radius, TVector3 &intersect)
{
    TVector3 vector = point_2 - point_1;

    Double_t lambda = ( center.Z() - point_1.Z() ) / vector.Z();

    intersect = point_1 + lambda * vector;

    Double_t r_xy_intersect = TMath::Sqrt( intersect.X()*intersect.X() + intersect.Y()*intersect.Y() );

    if(r_xy_intersect < radius)
    {
	return true;
    }
    else
    {
	return false;
    }
}

Bool_t rayTracer::lineIntersectsCylinderOnce(const TVector3 &point_1, const TVector3 &point_2, const TVector3 &centerBegin, const TVector3 &centerEnd, const Double_t &radius, TVector3 &intersect)
{
    TVector3 vector = point_2 - point_1;

    Double_t lambda_dummy = ( -1000. - point_1.Z() ) / vector.Z();

    TVector3 dummy = point_1 + lambda_dummy * vector;

    TVector3 vector_dummy = point_2 - dummy;

    Double_t factor = (vector_dummy.X()*vector_dummy.X() + vector_dummy.Y()*vector_dummy.Y());
    Double_t p = 2. * (dummy.X()*vector_dummy.X() + dummy.Y()*vector_dummy.Y()) / factor;
    Double_t q = (dummy.X()*dummy.X() + dummy.Y()*dummy.Y() - radius*radius) / factor;

    Double_t lambda_1 = -p/2. + TMath::Sqrt( p*p/4. - q);
    Double_t lambda_2 = -p/2. - TMath::Sqrt( p*p/4. - q);

    TVector3 intersect_1 = dummy + lambda_1 * vector_dummy;
    TVector3 intersect_2 = dummy + lambda_2 * vector_dummy;

    Bool_t intersect_1_valid = (intersect_1.Z() > centerBegin.Z() ) && (intersect_1.Z() < centerEnd.Z());
    Bool_t intersect_2_valid = (intersect_2.Z() > centerBegin.Z() ) && (intersect_2.Z() < centerEnd.Z());

    if( (intersect_1_valid && intersect_2_valid) || (!intersect_1_valid && !intersect_2_valid) )
    {
	return false;
    }
    else if(intersect_1_valid)
    {
	intersect = intersect_1;
	return true;
    }
    else
    {
	intersect = intersect_2;
	return true;
    }
}

Double_t rayTracer::telescopeTransmission(Double_t angle, Double_t transmissionAt10Arcmin)
{
    Double_t b = 1.0;
    Double_t a = (1.0 - transmissionAt10Arcmin) / (0. - (10./60.*TMath::DegToRad()) );

    Double_t t = a*angle + b;

    if(t < 0.){t = 0.;}

    return t;
}
