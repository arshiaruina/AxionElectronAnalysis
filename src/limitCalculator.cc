#include "limitCalculator.h"

limitCalculator::limitCalculator(std::string identifier, TLimitDataSource* dataSource, Bool_t useStatisticErrors, Double_t ge_ggamma):
    _identifier(identifier),
    _dataSource(dataSource),
    _useStatisticErrors(useStatisticErrors),
    _ge_ggammaOriginal(ge_ggamma),
    _ge_ggammaCurrent(ge_ggamma),
    _observedCLb(0.),
    _observedCLs95Limit(0.),
    _expectedCLs95Limit(0.),
    _expectedCLs95LimitPlus1Sigma(0.),
    _expectedCLs95LimitMinus1Sigma(0.)
{
    _randomGenerator = new TRandom3();
    _randomGenerator->SetSeed(0);
}

limitCalculator::~limitCalculator(){
    delete _randomGenerator;
}

Double_t limitCalculator::getObservedCLs95Limit(){
    return _observedCLs95Limit;
}

Double_t limitCalculator::getExpectedCLs95Limit(){
    return _expectedCLs95Limit;
}

Double_t limitCalculator::getExpectedCLs95LimitPlus1Sigma(){
    return _expectedCLs95LimitPlus1Sigma;
}

Double_t limitCalculator::getExpectedCLs95LimitMinus1Sigma(){
    return _expectedCLs95LimitMinus1Sigma;
}

Double_t limitCalculator::getObservedCLb(){
    return _observedCLb;
}

void limitCalculator::rescaleSignals(Double_t ge_ggamma){
    // TODO: change this to rescale to g_ae instead of beta_gamma
    
    TObjArray* array = _dataSource->GetSignal();

    // scaling done as: (from data file)
    // #I calculated the flux using gae = 0.511*10^-10 
    // #for other values of gae use:
    // #FLUX = Table*[gae/(0.511*10^-10)]^2
    Double_t scaleFactor = TMath::Power(ge_ggamma/_ge_ggammaCurrent,2.);

    for(Int_t iChannel = 0; iChannel < array->GetEntriesFast(); iChannel++)
    {
	((TH1*)array->At(iChannel))->Scale(scaleFactor);
    }

    _ge_ggammaCurrent = ge_ggamma;

    return;
}

Int_t limitCalculator::performCLs95LimitScan(Double_t ge_ggammaMax,
					     Double_t ge_ggammaMin,
					     Double_t ge_ggammaStep,
					     Bool_t saveToFile,
					     Bool_t suppressObservedLimit){
    // TODO: change limit scan to use ge_ggamma 
    // TODO: maybe need to consider axion masses? Given that there's some dependence on the mass
    //       above some value?
    bool perform_log_steps(false);
    Int_t numberSteps;
    if (perform_log_steps == true){
	numberSteps= 100;//(Int_t)( (TMath::Log10(ge_ggammaMax) - TMath::Log10(ge_ggammaMin)) / ge_ggammaStep) + 1;	
    }
    else{
	numberSteps = (Int_t)( (ge_ggammaMax - ge_ggammaMin) / ge_ggammaStep) + 1;
    }
    std::cout << "number of steps " << numberSteps << std::endl;


    std::stringstream fileName;
    fileName << "results/" << _identifier << "-scan.dat";

    std::ofstream file;
    if(saveToFile)
    {
	file.open(fileName.str().c_str(),std::fstream::trunc);
	if(!suppressObservedLimit){file <<	"CLb\tCLs\t";}
	file << "#ge_ggamma\tCLb\tCLs\t<CLs>_b\t<CLs>_b(+1sigma)\t<CLs>_b(-1sigma)\n";
	file.close();
    }

    Bool_t recordObservedCLs95Limit(false);
    Bool_t foundObservedCLs95Limit(false);

    Bool_t recordExpectedCLs95Limit(false);
    Bool_t foundExpectedCLs95Limit(false);

    Bool_t recordExpectedCLs95LimitPlus1Sigma(false);
    Bool_t foundExpectedCLs95LimitPlus1Sigma(false);

    Bool_t recordExpectedCLs95LimitMinus1Sigma(false);
    Bool_t foundExpectedCLs95LimitMinus1Sigma(false);

    Double_t sumObservedCLb(0.);
    Int_t numberObservedCLb(0);

    for(Int_t iStep = 0; iStep <= numberSteps; iStep++)
    {
	Double_t ge_ggamma;
	if (perform_log_steps == true){
	    ge_ggamma = ge_ggammaMax / (TMath::Power(2, iStep));//TMath::Power(10, (iStep * ge_ggammaStep));
	}
	else{
	    ge_ggamma = ge_ggammaMax - (iStep * ge_ggammaStep);
	}


	rescaleSignals(ge_ggamma);

	std::cout << "ge_gamma =  " << ge_ggamma << std::endl;

	TConfidenceLevel* confidenceLevel = TLimit::ComputeLimit(_dataSource,LIMITCALCULATOR_NMC,_useStatisticErrors,_randomGenerator);

	Double_t observedCLb = confidenceLevel->CLb();
	Double_t observedCLs = confidenceLevel->CLs();
	Double_t expectedCLs_b = confidenceLevel->GetExpectedCLs_b(0);
	Double_t expectedCLs_bPlus1Sigma = confidenceLevel->GetExpectedCLs_b(+1);
	Double_t expectedCLs_bMinus1Sigma = confidenceLevel->GetExpectedCLs_b(-1);

	delete confidenceLevel;

	numberObservedCLb++;
	sumObservedCLb+=observedCLb;

	if(!foundObservedCLs95Limit && (observedCLs > 0.05) )
	{
	    foundObservedCLs95Limit = true;
	    recordObservedCLs95Limit = true;
	    iStep--;
	    iStep--;
	    continue;
	}

	if(recordObservedCLs95Limit)
	{
	    _observedCLb = observedCLb;
	    _observedCLs95Limit = ge_ggamma;
	    recordObservedCLs95Limit = false;
	    std::cout.precision(5);
	    std::cout << "\tFound observed 95 % CLs limit at " << std::scientific << ge_ggamma << " (" << observedCLs << ")" << std::endl;
	}

	if(!foundExpectedCLs95Limit && (expectedCLs_b > 0.05) )
	{
	    foundExpectedCLs95Limit = true;
	    recordExpectedCLs95Limit = true;
	    iStep--;
	    iStep--;
	    continue;
	}

	if(recordExpectedCLs95Limit)
	{
	    _expectedCLs95Limit = ge_ggamma;
	    recordExpectedCLs95Limit = false;
	    std::cout.precision(5);
	    std::cout << "\tFound expected 95 % <CLs>_b limit at " << std::scientific << ge_ggamma << " (" << expectedCLs_b << ")" << std::endl;
	}

	if(!foundExpectedCLs95LimitPlus1Sigma && (expectedCLs_bPlus1Sigma > 0.05) )
	{
	    foundExpectedCLs95LimitPlus1Sigma = true;
	    recordExpectedCLs95LimitPlus1Sigma = true;
	    iStep--;
	    iStep--;
	    continue;
	}

	if(recordExpectedCLs95LimitPlus1Sigma)
	{
	    _expectedCLs95LimitPlus1Sigma = ge_ggamma;
	    recordExpectedCLs95LimitPlus1Sigma = false;
	    std::cout.precision(5);
	    std::cout << "\tFound expected 95 % <CLs>_b(+1sigma) limit at " << std::scientific << ge_ggamma << " (" << expectedCLs_bPlus1Sigma << ")" << std::endl;
	}

	if(!foundExpectedCLs95LimitMinus1Sigma && (expectedCLs_bMinus1Sigma > 0.05) )
	{
	    foundExpectedCLs95LimitMinus1Sigma = true;
	    recordExpectedCLs95LimitMinus1Sigma = true;
	    iStep--;
	    iStep--;
	    continue;
	}

	if(recordExpectedCLs95LimitMinus1Sigma)
	{
	    _expectedCLs95LimitMinus1Sigma = ge_ggamma;
	    recordExpectedCLs95LimitMinus1Sigma = false;
	    std::cout.precision(5);
	    std::cout << "\tFound expected 95 % <CLs>_b(-1sigma) limit at " << std::scientific << ge_ggamma << " (" << expectedCLs_bMinus1Sigma << ")" << std::endl;
	}

	if(saveToFile)
	{
	    file.open(fileName.str().c_str(),std::fstream::app);
	    file << ge_ggamma << "\t";
	    if(!suppressObservedLimit){
		file << observedCLb << "\t"
		     << observedCLs << "\t";
	    }
	    file << expectedCLs_b << "\t"
		 << expectedCLs_bPlus1Sigma << "\t"
		 << expectedCLs_bMinus1Sigma << "\n";
	    file.close();
	}

	if(foundExpectedCLs95Limit && foundExpectedCLs95LimitMinus1Sigma && foundExpectedCLs95LimitPlus1Sigma && (foundObservedCLs95Limit || suppressObservedLimit) && !saveToFile){
	    break;
	}

    }

    _observedCLb = sumObservedCLb / (Double_t)numberObservedCLb;

    if(foundExpectedCLs95Limit && foundExpectedCLs95LimitMinus1Sigma && foundExpectedCLs95LimitPlus1Sigma && (foundObservedCLs95Limit || suppressObservedLimit)){
	return 0;
    }
    else
    {
	std::cerr << "Error: Not all limit values found within scan boundaries!" << std::endl;
	return 1;
    }
}

Int_t limitCalculator::calculateCLs95LimitFast(Double_t ge_ggammaCoarseMax,
					       Double_t ge_ggammaCoarseMin,
					       Double_t ge_ggammaCoarseStep,
					       Double_t ge_ggammaFineStep,
					       Bool_t suppressObservedLimit){
    std::cout << "Performing coarse scan from " 
	      << ge_ggammaCoarseMax 
	      << " to " 
	      << ge_ggammaCoarseMin
	      << " with step size of " 
	      << ge_ggammaCoarseStep
	      << "..." 
	      << std::endl;

    Int_t success = performCLs95LimitScan(ge_ggammaCoarseMax, 
					  ge_ggammaCoarseMin, 
					  ge_ggammaCoarseStep, 
					  false,
					  suppressObservedLimit);

    if(success != 0)
    {
	return 1;
    }

    std::cout << "Performing individual fine scans around found coarse values with step size " 
	      << ge_ggammaFineStep
	      << "..."
	      << std::endl;

    Int_t numberSteps = (Int_t)(ge_ggammaCoarseStep / ge_ggammaFineStep) + 1;

    if(!suppressObservedLimit){
	success += performIndividualCLs95LimitScan(numberSteps, ge_ggammaFineStep, parameter::observed);
    }
    success += performIndividualCLs95LimitScan(numberSteps, ge_ggammaFineStep, parameter::expected);
    success += performIndividualCLs95LimitScan(numberSteps, ge_ggammaFineStep, parameter::expectedMinus1Sigma);
    success += performIndividualCLs95LimitScan(numberSteps, ge_ggammaFineStep, parameter::expectedPlus1Sigma);

    if(success == 0)
    {
	return 0;
    }
    else
    {
	return 1;
    }
}

Int_t limitCalculator::performIndividualCLs95LimitScan(Int_t numberSteps, Double_t ge_ggammaStep, parameter param)
{
    Bool_t found(false);
    Bool_t record(false);

    Double_t observedCLs;
    Double_t observedCLb;
    Double_t expectedCLs_b;
    Double_t expectedCLs_bPlus1Sigma;
    Double_t expectedCLs_bMinus1Sigma;

    Double_t ge_ggammaStart;
    std::string parameterName;
    Double_t* pointerForLimit;
    Double_t* pointer;

    switch(param)
    {
	case parameter::observed:
	    ge_ggammaStart = _observedCLs95Limit;
	    parameterName = "observed 95 % Cls";
	    pointerForLimit = &_observedCLs95Limit;
	    pointer = &observedCLs;
	    break;
	case parameter::expected:
	    ge_ggammaStart = _expectedCLs95Limit;
	    parameterName = "expected 95 % <Cls>_b";
	    pointerForLimit = &_expectedCLs95Limit;
	    pointer = &expectedCLs_b;
	    break;
	case parameter::expectedMinus1Sigma:
	    ge_ggammaStart = _expectedCLs95LimitMinus1Sigma;
	    parameterName = "expected 95 % <Cls>_b(-1sigma)";
	    pointerForLimit = &_expectedCLs95LimitMinus1Sigma;
	    pointer = &expectedCLs_bMinus1Sigma;
	    break;
	case parameter::expectedPlus1Sigma:
	    ge_ggammaStart = _expectedCLs95LimitPlus1Sigma;
	    parameterName = "expected 95 % <Cls>_b(+1sigma)";
	    pointerForLimit = &_expectedCLs95LimitPlus1Sigma;
	    pointer = &expectedCLs_bPlus1Sigma;
	    break;
    }

    for(Int_t iStep = -numberSteps; iStep < (numberSteps+1); iStep++)
    {

	Double_t ge_ggamma = ge_ggammaStart - (iStep * ge_ggammaStep);

	rescaleSignals(ge_ggamma);

	TConfidenceLevel* confidenceLevel = TLimit::ComputeLimit(_dataSource,LIMITCALCULATOR_NMC,_useStatisticErrors,_randomGenerator);

	observedCLs = confidenceLevel->CLs();
	observedCLb = confidenceLevel->CLb();	
	expectedCLs_b = confidenceLevel->GetExpectedCLs_b(0);
	expectedCLs_bPlus1Sigma = confidenceLevel->GetExpectedCLs_b(+1);
	expectedCLs_bMinus1Sigma = confidenceLevel->GetExpectedCLs_b(-1);

	delete confidenceLevel;

	if(!found && (*pointer > 0.05) )
	{
	    found = true;
	    record = true;
	    iStep--;
	    iStep--;
	    continue;
	}

	if(record){
	    if (param == parameter::observed){
		_observedCLb = observedCLb;
	    }
	    *pointerForLimit = ge_ggamma;
	    record = false;
	    std::cout << "\tFound " << parameterName << " limit at " << ge_ggamma << " (" << *pointer << ")" << std::endl;
	    break;
	}

    }

    if(found)
    {
	return 0;
    }
    else
    {
	std::cerr << "Could not find " << parameterName << " limit during individual scan!" << std::endl;
	return 1;
    }
}

Int_t limitCalculator::calculateCLs95Limit(Double_t ge_ggammaMax, 
					   Double_t ge_ggammaMin, 
					   Double_t ge_ggammaStep,
					   Bool_t suppressObservedLimit){
    std::cout << "Performing scan from " << ge_ggammaMax << " to " << ge_ggammaMin << " with step size of " << ge_ggammaStep << "..." << std::endl;

    Int_t success = performCLs95LimitScan(ge_ggammaMax,
					  ge_ggammaMin,
					  ge_ggammaStep,
					  true,
					  suppressObservedLimit);

    return success;
}

void limitCalculator::createExclusionPlotWithExpectedLimit(Float_t ge_ggammaMax)
{
    createExclusionPlot(true,ge_ggammaMax);

    return;
}

void limitCalculator::createExclusionPlotWithObservedLimit(Float_t ge_ggammaMax)
{
    createExclusionPlot(false,ge_ggammaMax);

    return;
}

void limitCalculator::createExclusionPlot(Bool_t showExpectedLimitOnly, Float_t beta_gammaMax)
{
    rootConfig::applySettings();

    gDirectory->cd(0);

    //canvas graphics area dimensions
    Double_t w(ROOTCONFIG_CANVAS_WIDTH);
    Double_t h(ROOTCONFIG_CANVAS_HEIGHT);

    Double_t l(ROOTCONFIG_CANVAS_MARGIN_LEFT);
    Double_t r(ROOTCONFIG_CANVAS_MARGIN_RIGHT);
    Double_t b(ROOTCONFIG_CANVAS_MARGIN_BOTTOM);
    Double_t t(ROOTCONFIG_CANVAS_MARGIN_TOP);

    TCanvas *canvas = new TCanvas("canvas","",w,h);
    canvas->SetWindowSize(w+(w-canvas->GetWw()),h+(h-canvas->GetWh()));
    canvas->SetMargin(l,r,b,t);

    //log scale on both axes
    canvas->GetPad(0)->SetLogx();
    canvas->GetPad(0)->SetLogy();

    Float_t beta_gamma_min = LIMITCALCULATOR_BETA_GAMMA_MIN;
    Float_t beta_gamma_max = (beta_gammaMax > LIMITCALCULATOR_BETA_GAMMA_MAX ? LIMITCALCULATOR_BETA_GAMMA_MAX : beta_gammaMax);

    Float_t beta_m_min = LIMITCALCULATOR_BETA_M_MIN;
    Float_t beta_m_max = LIMITCALCULATOR_BETA_M_MAX;

    Double_t axisAspectRatio = TMath::Log(beta_m_max / beta_m_min) / TMath::Log(beta_gamma_max / beta_gamma_min) / (w*(1.-l-r)) * (h*(1.-t-b));

    Float_t distanceFactorVertical(1.3);
    Float_t distanceFactorHorizontal(2.);

    //use TH1F to create axes
    TH1F *axes = new TH1F("axes","",200,beta_m_min,beta_m_max);
    axes->SetMinimum(beta_gamma_min);
    axes->SetMaximum(beta_gamma_max);
    axes->GetXaxis()->SetTitle("matter coupling #beta_{m}");
    axes->GetXaxis()->CenterTitle(true);
    axes->GetYaxis()->SetTitle("photon coupling #beta_{#gamma}");
    axes->GetYaxis()->CenterTitle(true);
    axes->Draw("AXIS");

    //CAST SDD as published in PLB or observed InGrid limit
    Float_t beta_gamma_cast;
    if(showExpectedLimitOnly){beta_gamma_cast = LIMITCALCULATOR_BETA_GAMMA_LIMIT_SDD;}
    else{beta_gamma_cast = _observedCLs95Limit;}
    const Int_t n0(4);
    Float_t x0[n0] = {1.,1.e+6,1.e+6,1.};
    Float_t y0[n0] = {beta_gamma_cast,beta_gamma_cast,beta_gamma_max,beta_gamma_max};
    TPolyLine *p0 = new TPolyLine(n0,x0,y0,"F");
    p0->SetLineColor(kBlue);
    p0->SetFillColor(kBlue);
    p0->SetFillStyle(1001);

    //colliders (CLEO, precision EW)
    const Int_t n1(4);
    Float_t x1[n1] = {beta_m_min,beta_m_max,beta_m_max,beta_m_min};
    Float_t y1[n1] = {2.54e+15,2.54e+15,beta_gamma_max,beta_gamma_max};
    TPolyLine *p1 = new TPolyLine(n1, x1, y1, "F");
    p1->SetLineColor(kYellow-10);
    p1->SetFillColor(kYellow-10);
    p1->SetFillStyle(1001);

    //torsion pendulum
    const Int_t n2(4);
    Float_t x2[n2] = {beta_m_min,15.3269,15.3269,beta_m_min};
    Float_t y2[n2] = {beta_gamma_min,beta_gamma_min,beta_gamma_max,beta_gamma_max};
    TPolyLine *p2 = new TPolyLine(n2, x2, y2, "F");
    p2->SetLineColor(kGreen-10);
    p2->SetFillColor(kGreen-10);
    p2->SetFillStyle(1001);

    //neutrons
    const Int_t n3(4);
    Float_t x3[n3] = {4.7e+06,beta_m_max,beta_m_max,4.7e+06};
    Float_t y3[n3] = {beta_gamma_min,beta_gamma_min,2.54e+15,2.54e+15};
    if(beta_gamma_max <= 2.54e+15){
	y3[2]=beta_gamma_max;
	y3[3]=beta_gamma_max;
    }
    TPolyLine *p3 = new TPolyLine(n3, x3, y3, "F");
    p3->SetLineColor(kBlue-10);
    p3->SetFillColor(kBlue-10);
    p3->SetFillStyle(1001);

    //afterglow (GammeV-CHASE)
    const Int_t n4(54);
    Int_t n4a(n4);
    Float_t x4[n4] = {22340.4,22340.4,1.97036e+13,2.66351e+13,2.80074e+13,3.98107e+13,3.98107e+13,4.86713e+13,4.86713e+13,6.91831e+13,6.91831e+13,8.89387e+13,8.89387e+13,1.39783e+14,
		      1.32934e+14,1.46985e+14,2.0893e+14,2.0893e+14,2.68591e+14,2.82429e+14,3.12281e+14,3.2837e+14,4.43887e+14,4.43887e+14,6.00042e+14,6.00042e+14,8.52922e+14,8.52922e+14,
		      1.40958e+15,1.40958e+15,1.09648e+15,1.09648e+15,1.40958e+15,1.40958e+15,1.21237e+15,1.21237e+15,1.90546e+15,2.00363e+15,2.57578e+15,2.57578e+15,2.84804e+15,3.14907e+15,
		      3.14907e+15,3.84995e+15,3.84995e+15,3.31131e+15,3.31131e+15,6.05088e+15,6.05088e+15,7.03514e+15,7.3976e+15,9.04406e+15,1e+16,1e+16};
    Float_t y4[n4] = {2.54e+15,82540421120,86219612160,98270740480,1.22214e+11,1.33352e+11,1.89023e+11,2.15443e+11,2.35078e+11,2.45557e+11,3.05386e+11,3.18998e+11,3.79792e+11,3.79792e+11,
		      6.69511e+11,7.97104e+11,7.6309e+11,1.23285e+12,1.23285e+12,4.76464e+12,4.76464e+12,3.21792e+12,3.08061e+12,6.18966e+12,6.18966e+12,1e+13,1e+13,1.54666e+13,1.54666e+13,
		      1.92349e+13,1.92349e+13,2.49878e+13,2.49878e+13,3.24611e+13,3.24611e+13,3.86475e+13,3.86475e+13,4.60128e+13,4.60128e+13,6.81292e+13,8.11131e+13,8.11131e+13,1.05373e+14,
		      1.05373e+14,1.25454e+14,1.25454e+14,1.94034e+14,1.94034e+14,2.52067e+14,2.52067e+14,4.25391e+14,4.25391e+14,4.25391e+14,2.54e+15};
    if(beta_gamma_max < 2.54e+15){
	y4[0] = beta_gamma_max;
	Int_t i = n4 - 1;
	while(y4[i] > beta_gamma_max)
	{
	    i--;
	}
	n4a = i + 2;
	x4[i+1] = x4[i];
	y4[i+1] = beta_gamma_max;
    }

    TPolyLine *p4 = new TPolyLine(n4a, x4, y4, "F");
    p4->SetLineColor(kRed-10);
    p4->SetFillColor(kRed-10);
    p4->SetFillStyle(1001);

    p3->Draw();
    p4->Draw();
    if(beta_gamma_max > 2.54e+15){p1->Draw();}
    p2->Draw();
    p0->Draw();

    //upper/lower bounds

    //atom interferometry
    const Int_t n5(2);
    Float_t x5[n5] = {1.42e+5,1.42e+5};
    Float_t y5[n5] = {beta_gamma_min,beta_gamma_max};
    TPolyLine *p5 = new TPolyLine(n5, x5, y5);
    p5->SetLineColor(kRed);
    p5->SetLineWidth(4);
    p5->Draw();

    TArrow *arrow0 = new TArrow(x5[0],5.*beta_gamma_min,TMath::Power(10.,TMath::Log10(x5[0]) + 0.5),5.*beta_gamma_min,0.005,"|>");
    arrow0->SetLineColor(kRed);
    arrow0->SetFillColor(kRed);
    arrow0->SetLineWidth(2);
    arrow0->Draw();

    TArrow *arrow1 = new TArrow(x5[0],0.2*beta_gamma_max,TMath::Power(10.,TMath::Log10(x5[0]) + 0.5),0.2*beta_gamma_max,0.005,"|>");
    arrow1->SetLineColor(kRed);
    arrow1->SetFillColor(kRed);
    arrow1->SetLineWidth(2);
    arrow1->Draw();

    //astronomical polarization
    const Int_t n6(3);
    Float_t x6[n6] = {beta_m_min,1.32934e+14,1.32934e+14};
    Float_t y6[n6] = {1.1e+9,1.1e+9,beta_gamma_max};
    TPolyLine *p6 = new TPolyLine(n6, x6, y6);
    p6->SetLineColor(kCyan-5);
    p6->SetLineWidth(4);
    p6->Draw();

    TArrow *arrow2 = new TArrow(5.*beta_m_min,y6[0],5.*beta_m_min,TMath::Power(10.,TMath::Log10(y6[0]) + 0.5/axisAspectRatio),0.005,"|>");
    arrow2->SetLineColor(kCyan-5);
    arrow2->SetFillColor(kCyan-5);
    arrow2->SetLineWidth(2);
    arrow2->Draw();

    TArrow *arrow3 = new TArrow(x6[n6-1],0.2*beta_gamma_max,TMath::Power(10.,TMath::Log10(x6[n6-1]) - 0.5),0.2*beta_gamma_max,0.005,"|>");
    arrow3->SetLineColor(kCyan-5);
    arrow3->SetFillColor(kCyan-5);
    arrow3->SetLineWidth(2);
    arrow3->Draw();

    //solar limit
    const Int_t n7(3);
    Float_t x7[n7] = {1,1e+6,1e+6};
    Float_t y7[n7] = {6.45e+10,6.45e+10,8.3e+10};
    TPolyLine *p7 = new TPolyLine(n7, x7, y7);
    p7->SetLineColor(kBlack);
    p7->SetLineWidth(2);
    p7->SetLineStyle(3);
    p7->Draw();


    //CAST InGrid <CLs>_b
    Float_t beta_gamma_expected = _expectedCLs95Limit;
    Float_t beta_gamma_expected_1sigma_up = _expectedCLs95LimitMinus1Sigma;
    Float_t beta_gamma_expected_1sigma_down = _expectedCLs95LimitPlus1Sigma;
    const Int_t n8(2);
    Float_t x8[n8] = {1,1.e+6};
    Float_t y8[n8] = {beta_gamma_expected,beta_gamma_expected};
    TPolyLine *p8 = new TPolyLine(n8, x8, y8);
    p8->SetLineColor(kBlue);
    p8->SetLineWidth(1);
    p8->SetLineStyle(1);
    const Int_t n81(4);
    Float_t x81[n81] = {1,1,1.e+6,1.e+6};
    Float_t y81[n81] = {beta_gamma_expected_1sigma_up,beta_gamma_expected_1sigma_down,beta_gamma_expected_1sigma_down,beta_gamma_expected_1sigma_up};
    TPolyLine *p81 = new TPolyLine(n81,x81,y81,"F");
    p81->SetLineColor(kCyan);
    p81->SetFillColor(kCyan);
    p81->SetLineWidth(1);
    p81->SetLineStyle(0);
    p81->SetFillStyle(1001);
    if(showExpectedLimitOnly)
    {
	p81->Draw();
	p8->Draw();
    }

    //text & labels

    TLatex *tex0 = new TLatex(getCenteredValueOnLogScale(x0[0],x5[0]),getCenteredValueOnLogScale(y0[0],beta_gamma_max),"CAST");
    tex0->SetTextAlign(22);
    tex0->SetTextColor(kWhite);
    tex0->SetTextSize(0.05);
    tex0->Draw();

    TLatex *tex1 = new TLatex(getCenteredValueOnLogScale(x0[1],x6[1]),getCenteredValueOnLogScale(y1[0],beta_gamma_max),"#splitline{Colliders}{(CLEO, precision EW)}");
    tex1->SetTextAlign(22);
    tex1->SetTextColor(kBlack);
    tex1->SetTextSize(0.05);
    if(beta_gamma_max > 2.54e+15){tex1->Draw();}

    TLatex *tex2 = new TLatex(getCenteredValueOnLogScale(beta_m_min,x0[0]),getCenteredValueOnLogScale(beta_gamma_min,beta_gamma_max),"torsion pendulum");
    tex2->SetTextAlign(22);
    tex2->SetTextColor(kBlack);
    tex2->SetTextAngle(90);
    tex2->SetTextSize(0.05);
    tex2->Draw();

    TLatex *tex3 = new TLatex(getCenteredValueOnLogScale(x3[0],x6[2]),getCenteredValueOnLogScale(beta_gamma_min,y4[1]),"neutrons");
    tex3->SetTextAlign(22);
    tex3->SetTextColor(kBlack);
    tex3->SetTextSize(0.05);
    tex3->Draw();

    TLatex *tex4 = new TLatex(getCenteredValueOnLogScale(x0[2],x4[2]),getCenteredValueOnLogScale(y4[1],y4[n4a-1]),"#splitline{afterglow}{(GammeV-CHASE)}");
    tex4->SetTextAlign(22);
    tex4->SetTextColor(kBlack);
    tex4->SetTextSize(0.05);
    tex4->Draw();

    TLatex *tex5 = new TLatex(x5[0]*distanceFactorHorizontal,getCenteredValueOnLogScale(beta_gamma_min,beta_gamma_max),"atom interferometry");
    tex5->SetTextAlign(22);
    tex5->SetTextColor(kRed);
    tex5->SetTextAngle(90);
    tex5->SetTextSize(0.03);
    tex5->Draw();

    TLatex *tex6 = new TLatex(getCenteredValueOnLogScale(beta_m_min,x3[0]),y6[0]*distanceFactorVertical,"astronomical polarization");
    tex6->SetTextAlign(22);
    tex6->SetTextColor(kCyan-5);
    tex6->SetTextSize(0.03);
    tex6->Draw();

    TLatex *tex7 = new TLatex(getCenteredValueOnLogScale(x0[0],x5[0]),y7[0]*distanceFactorVertical,"Solar limit");
    tex7->SetTextAlign(22);
    tex7->SetTextColor(kBlack);
    tex7->SetTextSize(0.03);
    tex7->Draw();

    TText *preliminary = new TText(getCenteredValueOnLogScale(beta_m_min,beta_m_max),getCenteredValueOnLogScale(beta_gamma_min,beta_gamma_max),"preliminary");
    preliminary->SetTextColor(kGray+2);
    preliminary->SetTextAlign(22);
    preliminary->SetTextSize(0.1);
    preliminary->SetTextAngle(45);
    preliminary->Draw();

    std::string text;
    text = "#splitline{#splitline{" + _identifier + "}{";
    if(showExpectedLimitOnly){text += "95 % <CLs>_b limit";}
    else{text += "95 % CLs limit";}
    text += "}}{For internal use only!}";

    TLatex *internal = new TLatex(TMath::Exp(TMath::Log(beta_m_min)+((TMath::Log(beta_m_max)-TMath::Log(beta_m_min))/20.)),TMath::Exp(TMath::Log(beta_gamma_min)+((TMath::Log(beta_gamma_max)-TMath::Log(beta_gamma_min))/20.*19.)),text.c_str());
    internal->SetTextColor(kGray+2);
    internal->SetTextAlign(13);
    internal->SetTextSize(0.02);
    //internal->Draw();

    axes->Draw("SAMEAXIS");

    std::string fileNameBasis("results/" + _identifier);
    if(showExpectedLimitOnly){fileNameBasis += "-expected";}

    canvas->Print((fileNameBasis + ".svg").c_str());
    canvas->Print((fileNameBasis + ".pdf").c_str());
    canvas->Print((fileNameBasis + ".eps").c_str());
    canvas->Print((fileNameBasis + ".png").c_str());

    delete p0;
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete p5;
    delete p6;
    delete p7;

    delete p8;
    delete p81;

    delete arrow0;
    delete arrow1;
    delete arrow2;
    delete arrow3;

    delete tex0;
    delete tex1;
    delete tex2;
    delete tex3;
    delete tex4;
    delete tex5;
    delete tex6;
    delete tex7;

    delete preliminary;
    delete internal;

    delete axes;
    delete canvas;

    return;
}
