#include "axionElectronLimit.h"

int main(){
    // the main function of the axion electron limit analysis script
    
    gErrorIgnoreLevel = kWarning;

    //configuration
    Bool_t showExpectedLimitOnly(true);

    Bool_t plotExpectedLimitOnly(false);

    // axion specific stuff
    axionRadiation::characteristic radiationCharacteristic(axionRadiation::characteristic::sar);
    Double_t coldboreBlockedLength(0.0); //

    //Xrt specific stuff
    Bool_t perfectXRT(false); //if true transmission of 100 % is assumed for all energies else interpolated data from transmission measurements is used
    Double_t xrtTransmissionAt10Arcmin(0.7); //relative transmission for x-rays at 10' angle compared to parallel beams

    //detector specific stuff
    detector::detectorVersion detectorVersion(detector::detectorVersion::mk2);
    detector::windowMaterial detectorWindowMaterial(detector::windowMaterial::Mylar);
    Double_t detectorWindowThickness(2000.); //nm
    Double_t detectorWindowAreaTransparency(0.826); //Mylar 3x3: 82.6 %, SiN Quad-Ribs 500 µm: 73.5 %
    Double_t detectorWindowAluminiumLayerThickness(40.); //nm Mylar: 40 nm, SiN: 20 nm
    detector::windowMaterial differentialWindowMaterial(detector::windowMaterial::Mylar);
    Double_t differentialWindowThickness(900.); //nm
    Double_t detectorWindowAperture(14.0); //mm
    Bool_t smearEnergy(false);//true);
        
    //analysis specific stuff
    Bool_t useSystematicErrors(false);//true);
    Bool_t useStatisticalErrors(false);
    Bool_t createBackgroundRatePlots(false);

    chipRegions::region chipRegionSignalChannelOne(chipRegions::region::gold);
    Double_t softwareEfficiencyChannelOne(0.8);
    Double_t backgroundScalingFactorChannelOne(1.);
    chipRegions::region chipRegionBackgroundAndDataChannelOne(chipRegions::region::gold);
    Double_t backgroundSystematicErrorsChannelOne[AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    Double_t signalSystematicErrorsChannelOne[AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS]     = {0.0, 0.025, 0.025, 0.1, 0.1, 0.025};
    Bool_t useChannelOne(true);

    chipRegions::region chipRegionSignalChannelTwo(chipRegions::region::silver);
    Double_t softwareEfficiencyChannelTwo(0.8);
    Double_t backgroundScalingFactorChannelTwo(1.);
    chipRegions::region chipRegionBackgroundAndDataChannelTwo(chipRegions::region::silver);
    Double_t backgroundSystematicErrorsChannelTwo[AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    Double_t signalSystematicErrorsChannelTwo[AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS]     = {0.0, 0.025, 0.025, 0.1, 0.1, 0.025};
    Bool_t useChannelTwo(false);

    chipRegions::region chipRegionSignalChannelThree(chipRegions::region::bronze);
    Double_t softwareEfficiencyChannelThree(0.8);
    Double_t backgroundScalingFactorChannelThree(1.);
    chipRegions::region chipRegionBackgroundAndDataChannelThree(chipRegions::region::bronze);
    Double_t backgroundSystematicErrorsChannelThree[AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    Double_t signalSystematicErrorsChannelThree[AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS]     = {0.0, 0.2, 0.025, 0.1, 0.1, 0.1};
    Bool_t useChannelThree(false);

    //choice of dataset
    dataSets::set dataSet(dataSets::set::srt_2014_2015);

    //do some safety and sanity checks
    //abort if now channel is activated
    if(!(useChannelOne || useChannelTwo || useChannelThree)){return 1;}

    //display warnings if data and background for signal region will be derived through scaling or when background will be scaled
    if(chipRegionBackgroundAndDataChannelOne != chipRegionSignalChannelOne)
    {
	std::cout << "WARNING! Background and data for "
		  << chipRegions::areaString(chipRegionSignalChannelOne)
		  << " region will be derived from "
		  << chipRegions::areaString(chipRegionBackgroundAndDataChannelOne)
		  << " region through scaling!" << std::endl;
    }
    if(backgroundScalingFactorChannelOne != 1.)
    {
	std::cout << "WARNING! Background for "
		  << chipRegions::areaString(chipRegionBackgroundAndDataChannelOne)
		  << " region will be scaled by factor of "
		  << backgroundScalingFactorChannelOne
		  << "!" << std::endl;
    }
    if(chipRegionBackgroundAndDataChannelTwo != chipRegionSignalChannelTwo)
    {
	std::cout << "WARNING! Background and data for "
		  << chipRegions::areaString(chipRegionSignalChannelTwo)
		  << " region will be derived from "
		  << chipRegions::areaString(chipRegionBackgroundAndDataChannelTwo)
		  << " region through scaling!" << std::endl;
    }
    if(backgroundScalingFactorChannelTwo != 1.)
    {
	std::cout << "WARNING! Background for "
		  << chipRegions::areaString(chipRegionBackgroundAndDataChannelTwo)
		  << " region will be scaled by factor of "
		  << backgroundScalingFactorChannelTwo
		  << "!" << std::endl;
    }
    if(chipRegionBackgroundAndDataChannelThree != chipRegionSignalChannelThree)
    {
	std::cout << "WARNING! Background and data for "
		  << chipRegions::areaString(chipRegionSignalChannelThree)
		  << " region will be derived from "
		  << chipRegions::areaString(chipRegionBackgroundAndDataChannelThree)
		  << " region through scaling!" << std::endl;
    }
    if(backgroundScalingFactorChannelThree != 1.)
    {
	std::cout << "WARNING! Background for "
		  << chipRegions::areaString(chipRegionBackgroundAndDataChannelThree)
		  << " region will be scaled by factor of "
		  << backgroundScalingFactorChannelThree
		  << "!" << std::endl;
    }

    //only allow display of observed limit when using real and unscaled data
    if(dataSet != dataSets::set::srt_2014_2015){showExpectedLimitOnly = true;}

    // TODO: change string identifier
    //build identifier string
    std::string identifier("");
    identifier += axionRadiation::characteristicString(radiationCharacteristic);
    identifier += "-txrt";
    identifier += (!perfectXRT)?"XX":"100";
    identifier += "-";
    identifier += std::to_string((Int_t)(xrtTransmissionAt10Arcmin*100.+0.5));
    identifier += "-";
    identifier += detector::detectorVersionString(detectorVersion);
    identifier += "-";
    identifier += detector::windowMaterialString(detectorWindowMaterial);
    identifier += std::to_string((Int_t)detectorWindowThickness) + "nm";
    identifier += "Al" + std::to_string((Int_t)detectorWindowAluminiumLayerThickness) + "nm";
    identifier += "-";
    if(differentialWindowMaterial != detector::windowMaterial::None)
    {
	identifier += detector::windowMaterialString(differentialWindowMaterial);
	identifier += std::to_string((Int_t)differentialWindowThickness) + "nm";
	identifier += "-";
    }
    if(smearEnergy){
	identifier += "ER-";
    }
    if(useChannelOne){
	identifier += (chipRegions::areaString(chipRegionSignalChannelOne) +
		       std::to_string((Int_t)(softwareEfficiencyChannelOne*100.+0.5)) + 
		       "-");
    }
    if(useChannelTwo){
	identifier += (chipRegions::areaString(chipRegionSignalChannelTwo) +
		       std::to_string((Int_t)(softwareEfficiencyChannelTwo*100.+0.5)) + 
		       "-");
    }
    if(useChannelThree){
	identifier += (chipRegions::areaString(chipRegionSignalChannelThree) +
		       std::to_string((Int_t)(softwareEfficiencyChannelThree*100.+0.5)) + 
		       "-");
    }
    if(!useStatisticalErrors){
	identifier += "StatErrorsOff-";
    }
    if(!useSystematicErrors){
	identifier += "SystErrorsOff-";
    }
    identifier += dataSets::setString(dataSet);

    std::cout << identifier << std::endl;

    //create instances of tools
    std::cout << "Creating objects..." << std::flush;
    rayTracer* myRayTracer                       = new rayTracer();
    efficiencyCalculator* myEfficiencyCalculator = new efficiencyCalculator(dataSets::calibrationFilePath(dataSet));
    signalGenerator* mySignalGenerator           = new signalGenerator();
    std::cout << "... working ..." << std::flush;
    dataExtractor* myDataExtractor               = new dataExtractor(dataSets::dataFilePath(dataSet),
								     dataSets::dataTreeName(dataSet));
    backgroundExtractor* myBackgroundExtractor   = new backgroundExtractor(dataSets::backgroundFilePath(dataSet),
									   dataSets::backgroundTreeName(dataSet),
									   dataSets::backgroundTime(dataSet));

    // define efficiency setting arrays for three channels
    Double_t* efficiencySettingChannelOne   = new Double_t[EFFICIENCYCALCULATOR_NUMBER_RANGES];
    Double_t* efficiencySettingChannelTwo   = new Double_t[EFFICIENCYCALCULATOR_NUMBER_RANGES];
    Double_t* efficiencySettingChannelThree = new Double_t[EFFICIENCYCALCULATOR_NUMBER_RANGES];

    std::cout << "... done" << std::endl;
    // TODO: check how ray tracing changes for axions
    std::cout << "Performing ray tracing MC..." << std::flush;
    myRayTracer->calculateFluxFractions(radiationCharacteristic, 
					xrtTransmissionAt10Arcmin,
					detectorWindowAperture,
					0.0, 0.0, 0.0, 0.0,
					coldboreBlockedLength);
    std::cout << "Done!" << std::endl;
    if(useChannelOne){
	std::cout << "\tFlux fraction for " 
		  << chipRegions::areaString(chipRegionSignalChannelOne)
		  << " region: " 
		  << myRayTracer->getFluxFraction(chipRegionSignalChannelOne)
		  << std::endl;
    }
    if(useChannelTwo){
	std::cout << "\tFlux fraction for " 
		  << chipRegions::areaString(chipRegionSignalChannelTwo)
		  << " region: " 
		  << myRayTracer->getFluxFraction(chipRegionSignalChannelTwo)
		  << std::endl;
    }
    if(useChannelThree){
	std::cout << "\tFlux fraction for " 
		  << chipRegions::areaString(chipRegionSignalChannelThree)
		  << " region: " 
		  << myRayTracer->getFluxFraction(chipRegionSignalChannelThree)
		  << std::endl;
    }

    // find the correct efficiency settings based on likelihood method 
    // for each region and put them into efficiencySetting arrays defined
    // above
    std::cout << "Finding efficiency settings..." << std::flush;
    if(useChannelOne){
	myEfficiencyCalculator->findEfficiencySetting(chipRegionBackgroundAndDataChannelOne,
						      softwareEfficiencyChannelOne,
						      efficiencySettingChannelOne);
    }
    if(useChannelTwo){
	myEfficiencyCalculator->findEfficiencySetting(chipRegionBackgroundAndDataChannelTwo,
						      softwareEfficiencyChannelTwo,
						      efficiencySettingChannelTwo);
    }
    if(useChannelThree){
	myEfficiencyCalculator->findEfficiencySetting(chipRegionBackgroundAndDataChannelThree,
						      softwareEfficiencyChannelThree,
						      efficiencySettingChannelThree);
    }
    std::cout << "Done!" << std::endl;

    // now for each used channel evaluate the efficiency for each energy range
    // TODO: understand if evaluation only used as safety check 
    if(useChannelOne)
    {
	Double_t* evaluationChannelOne = new Double_t[EFFICIENCYCALCULATOR_NUMBER_RANGES];
	myEfficiencyCalculator->evaluateEfficiencySetting(chipRegionSignalChannelOne,
							  efficiencySettingChannelOne,
							  evaluationChannelOne);
	std::cout << "\tEfficiency setting for " 
		  << chipRegions::areaString(chipRegionSignalChannelOne)
		  << " region: ";
	for(Int_t i = 0; i < EFFICIENCYCALCULATOR_NUMBER_RANGES; i++)
	{
	    std::cout << efficiencySettingChannelOne[i] 
		      << " (" 
		      << std::setprecision(3) 
		      << (evaluationChannelOne[i]*100.) 
		      << " %)";
	}
	std::cout << std::endl;
    }
    if(useChannelTwo)
    {
	Double_t* evaluationChannelTwo = new Double_t[EFFICIENCYCALCULATOR_NUMBER_RANGES];
	myEfficiencyCalculator->evaluateEfficiencySetting(chipRegionSignalChannelTwo,
							  efficiencySettingChannelTwo,
							  evaluationChannelTwo);
	std::cout << "\tEfficiency setting for " 
		  << chipRegions::areaString(chipRegionSignalChannelTwo)
		  << " region: ";
	for(Int_t i = 0; i < EFFICIENCYCALCULATOR_NUMBER_RANGES; i++)
	{
	    std::cout << efficiencySettingChannelTwo[i] 
		      << " (" 
		      << std::setprecision(3) 
		      << (evaluationChannelTwo[i]*100.) 
		      << " %)";
	}
	std::cout << std::endl;
    }
    if(useChannelThree)
    {
	Double_t* evaluationChannelThree = new Double_t[EFFICIENCYCALCULATOR_NUMBER_RANGES];
	myEfficiencyCalculator->evaluateEfficiencySetting(chipRegionSignalChannelThree,
							  efficiencySettingChannelThree,
							  evaluationChannelThree);
	std::cout << "\tEfficiency setting for " 
		  << chipRegions::areaString(chipRegionSignalChannelThree)
		  << " region: ";
	for(Int_t i = 0; i < EFFICIENCYCALCULATOR_NUMBER_RANGES; i++)
	{
	    std::cout << efficiencySettingChannelThree[i] 
		      << " (" 
		      << std::setprecision(3) 
		      << (evaluationChannelThree[i]*100.) 
		      << " %)";
	}
	std::cout << std::endl;
    }

    Int_t success(0);

    // now create histograms for all channels, signal and background
    TFile* file = new TFile(("results/" + identifier + ".root").c_str(),"RECREATE");
    file->cd();

    TH1D* signalChannelOne       =  new TH1D("signalChannelOne",
					     "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);
    TH1D* signalChannelTwo       =  new TH1D("signalChannelTwo",
					     "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);
    TH1D* signalChannelThree     =  new TH1D("signalChannelThree",
					     "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);

    TH1D* backgroundChannelOne   =  new TH1D("backgroundChannelOne",
					     "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);
    TH1D* backgroundChannelTwo   =  new TH1D("backgroundChannelTwo",
					     "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);
    TH1D* backgroundChannelThree =  new TH1D("backgroundChannelThree",
					     "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);

    TH1D* dataChannelOne 	 =  new TH1D("dataChannelOne",
						 "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);
    TH1D* dataChannelTwo 	 =  new TH1D("dataChannelTwo",
						 "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);
    TH1D* dataChannelThree       =  new TH1D("dataChannelThree",
					     "",
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MIN,
					     AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_MAX);

    // now generate signal based on parameters obtained before
    std::cout << "Generating signal histogram(s)..." << std::flush;
    if(useChannelOne)
    {
	success += mySignalGenerator->generateSignal(signalChannelOne,
						     detectorWindowMaterial,
						     detectorWindowThickness,
						     detectorWindowAreaTransparency,
						     detectorWindowAluminiumLayerThickness,
						     differentialWindowMaterial,
						     differentialWindowThickness,
						     myRayTracer->getFluxFraction(chipRegionSignalChannelOne),
						     softwareEfficiencyChannelOne,
						     perfectXRT,
						     //dataSets::backgroundTime(dataSet),
						     dataSets::dataTime(dataSet),
						     smearEnergy);
	signalChannelOne->Write();
    }
    if(useChannelTwo)
    {
	success += mySignalGenerator->generateSignal(signalChannelTwo,
						     detectorWindowMaterial,
						     detectorWindowThickness,
						     detectorWindowAreaTransparency,
						     detectorWindowAluminiumLayerThickness,
						     differentialWindowMaterial,
						     differentialWindowThickness,
						     myRayTracer->getFluxFraction(chipRegionSignalChannelTwo),
						     softwareEfficiencyChannelTwo,
						     perfectXRT,
						     dataSets::dataTime(dataSet),
						     smearEnergy);
	signalChannelTwo->Write();
    }
    if(useChannelThree)
    {
	success += mySignalGenerator->generateSignal(signalChannelThree,
						     detectorWindowMaterial,
						     detectorWindowThickness,
						     detectorWindowAreaTransparency,
						     detectorWindowAluminiumLayerThickness,
						     differentialWindowMaterial,
						     differentialWindowThickness,
						     myRayTracer->getFluxFraction(chipRegionSignalChannelThree),
						     softwareEfficiencyChannelThree,
						     perfectXRT,
						     dataSets::dataTime(dataSet),
						     smearEnergy);
	signalChannelThree->Write();
    }
    std::cout << "Done!" << std::endl;

    // ... and extract the background for the same channels
    // simply done by scaling the background from the background time to 
    // the signal time
    std::cout << "Extracting background and creating histogram(s)..." << std::flush;
    if(useChannelOne)
    {

	success += myBackgroundExtractor->extractBackground(backgroundChannelOne,
							    useStatisticalErrors,
							    chipRegionBackgroundAndDataChannelOne,
							    dataSets::dataTime(dataSet),
							    efficiencySettingChannelOne);
	backgroundChannelOne->Scale(backgroundScalingFactorChannelOne);
	if(chipRegionBackgroundAndDataChannelOne != chipRegionSignalChannelOne)
	{
	    backgroundChannelOne->Scale(chipRegions::area(chipRegionSignalChannelOne)/chipRegions::area(chipRegionBackgroundAndDataChannelOne));
	}
	backgroundChannelOne->Write();
    }
    if(useChannelTwo)
    {
	success += myBackgroundExtractor->extractBackground(backgroundChannelTwo,
							    useStatisticalErrors,
							    chipRegionBackgroundAndDataChannelTwo,
							    dataSets::dataTime(dataSet),
							    efficiencySettingChannelTwo);
	backgroundChannelTwo->Scale(backgroundScalingFactorChannelTwo);
	if(chipRegionBackgroundAndDataChannelTwo != chipRegionSignalChannelTwo)
	{
	    backgroundChannelTwo->Scale(chipRegions::area(chipRegionSignalChannelTwo)/chipRegions::area(chipRegionBackgroundAndDataChannelTwo));
	}
	backgroundChannelTwo->Write();
    }
    if(useChannelThree)
    {
	success += myBackgroundExtractor->extractBackground(backgroundChannelThree,
							    useStatisticalErrors,
							    chipRegionBackgroundAndDataChannelThree,
							    dataSets::dataTime(dataSet),
							    efficiencySettingChannelThree);
	backgroundChannelThree->Scale(backgroundScalingFactorChannelThree);
	if(chipRegionBackgroundAndDataChannelThree != chipRegionSignalChannelThree)
	{
	    backgroundChannelThree->Scale(chipRegions::area(chipRegionSignalChannelThree)/chipRegions::area(chipRegionBackgroundAndDataChannelThree));
	}
	backgroundChannelThree->Write();
    }
    std::cout << "Done!" << std::endl;


    // now extract data and create the histogram
    std::cout << "Extracting data and creating histogram(s)..." << std::flush;
    if(useChannelOne)
    {
	success += myDataExtractor->extractData(dataChannelOne,
						useStatisticalErrors,
						chipRegionBackgroundAndDataChannelOne,
						efficiencySettingChannelOne);
	dataChannelOne->Write();
    }
    if(useChannelTwo)
    {
	success += myDataExtractor->extractData(dataChannelTwo,
						useStatisticalErrors,
						chipRegionBackgroundAndDataChannelTwo,
						efficiencySettingChannelTwo);
	dataChannelTwo->Write();
    }
    if(useChannelThree)
    {
	success += myDataExtractor->extractData(dataChannelThree,
						useStatisticalErrors,
						chipRegionBackgroundAndDataChannelThree,
						efficiencySettingChannelThree);
	dataChannelThree->Write();
    }
    std::cout << "Done!" << std::endl;

    if(createBackgroundRatePlots)
    {
	std::cout << "Creating background rate plot(s)..." << std::flush;

	gDirectory->cd(0);

	rootConfig::applySettings();

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

	TGraphAsymmErrors* graph = NULL;

	Double_t plotCenterX = AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN + (AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX - AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN)/2.;

	TText* preliminary = new TText();
	preliminary->SetTextSize(0.1);
	preliminary->SetTextColor(kGray+2);
	preliminary->SetTextAlign(22);
	preliminary->SetTextAngle(45);

	if(useChannelOne)
	{
	    // before we create the backgroundrate plot, we dump signal and background data
	    // to a file
	    myBackgroundExtractor->dumpBackgroundRate("backgroundRate.dat",
						      chipRegionSignalChannelOne,
						      efficiencySettingChannelOne,
						      AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN,
						      AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX,
						      AXIONELECTRONLIMIT_PLOT_HISTOGRAM_BINS);

	    myDataExtractor->dumpDataRate("dataRate.dat",
					  dataSets::dataTime(dataSet),
					  chipRegionSignalChannelOne,
					  efficiencySettingChannelOne,
					  AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN,
					  AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX,
					  AXIONELECTRONLIMIT_PLOT_HISTOGRAM_BINS);


	    graph = myBackgroundExtractor->getBackgroundratePlot(chipRegionSignalChannelOne,
								 efficiencySettingChannelOne,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_BINS,
								 kBlue,
								 false);

	    graph->Draw("AP");
	    Double_t plotCenterY = graph->GetYaxis()->GetXmin() + (graph->GetYaxis()->GetXmax() - graph->GetYaxis()->GetXmin())/2.;
	    preliminary->SetText(plotCenterX,plotCenterY,"preliminary");
	    preliminary->Draw();

	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelOne) +
			   std::to_string((Int_t)(softwareEfficiencyChannelOne*100.+0.5)) + 
			   "-backgroundrate.pdf").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelOne) +
			   std::to_string((Int_t)(softwareEfficiencyChannelOne*100.+0.5)) + 
			   "-backgroundrate.png").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelOne) +
			   std::to_string((Int_t)(softwareEfficiencyChannelOne*100.+0.5)) + 
			   "-backgroundrate.eps").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelOne) +
			   std::to_string((Int_t)(softwareEfficiencyChannelOne*100.+0.5)) 
			   + "-backgroundrate.svg").c_str());
	    delete graph;
	}
	if(useChannelThree)
	{
	    canvas->GetPad(0)->SetLogy(1);
	    graph = myBackgroundExtractor->getBackgroundratePlot(chipRegionSignalChannelThree,
								 efficiencySettingChannelThree,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_BINS,
								 kBlue,
								 true);
	    graph->Draw("AP");
	    Double_t plotCenterY = TMath::Exp(TMath::Log(graph->GetYaxis()->GetXmin()) + 
					      (TMath::Log(graph->GetYaxis()->GetXmax()) - 
					       TMath::Log(graph->GetYaxis()->GetXmin()))/2.);
	    preliminary->SetText(plotCenterX,plotCenterY,"preliminary");
	    preliminary->Draw();

	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelThree) +
			   std::to_string((Int_t)(softwareEfficiencyChannelThree*100.+0.5)) + 
			   "-backgroundrate.pdf").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelThree) +
			   std::to_string((Int_t)(softwareEfficiencyChannelThree*100.+0.5)) + 
			   "-backgroundrate.png").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelThree) +
			   std::to_string((Int_t)(softwareEfficiencyChannelThree*100.+0.5)) + 
			   "-backgroundrate.eps").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelThree) +
			   std::to_string((Int_t)(softwareEfficiencyChannelThree*100.+0.5)) + 
			   "-backgroundrate.svg").c_str());
	}
	if(useChannelTwo)
	{
	    canvas->GetPad(0)->SetLogy(1);
	    graph = myBackgroundExtractor->getBackgroundratePlot(chipRegionSignalChannelTwo,
								 efficiencySettingChannelTwo,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MIN,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_MAX,
								 AXIONELECTRONLIMIT_PLOT_HISTOGRAM_BINS,
								 kBlue,
								 true);
	    graph->Draw("AP");
	    Double_t plotCenterY = TMath::Exp(TMath::Log(graph->GetYaxis()->GetXmin()) + 
					      (TMath::Log(graph->GetYaxis()->GetXmax()) - 
					       TMath::Log(graph->GetYaxis()->GetXmin()))/2.);
	    preliminary->SetText(plotCenterX,plotCenterY,"preliminary");
	    preliminary->Draw();

	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelTwo) +
			   std::to_string((Int_t)(softwareEfficiencyChannelTwo*100.+0.5)) + 
			   "-backgroundrate.pdf").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelTwo) +
			   std::to_string((Int_t)(softwareEfficiencyChannelTwo*100.+0.5)) + 
			   "-backgroundrate.png").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelTwo) +
			   std::to_string((Int_t)(softwareEfficiencyChannelTwo*100.+0.5)) + 
			   "-backgroundrate.eps").c_str());
	    canvas->Print(("results/" + chipRegions::areaString(chipRegionSignalChannelTwo) +
			   std::to_string((Int_t)(softwareEfficiencyChannelTwo*100.+0.5)) + 
			   "-backgroundrate.svg").c_str());
	    delete graph;
	}

	delete preliminary;
	delete canvas;

	std::cout << "Done!" << std::endl;
    }

    //cleaning up what is not needed anymore
    delete efficiencySettingChannelOne;
    delete efficiencySettingChannelTwo;
    delete efficiencySettingChannelThree;

    delete myRayTracer;
    delete mySignalGenerator;
    delete myEfficiencyCalculator;
    delete myDataExtractor;
    delete myBackgroundExtractor;

    if(success == 0){
	TVectorD backgroundErrorsChannelOne(AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS,backgroundSystematicErrorsChannelOne);
	TVectorD signalErrorsChannelOne(AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS,signalSystematicErrorsChannelOne);
	TVectorD backgroundErrorsChannelTwo(AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS,backgroundSystematicErrorsChannelTwo);
	TVectorD signalErrorsChannelTwo(AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS,signalSystematicErrorsChannelTwo);
	TVectorD backgroundErrorsChannelThree(AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS,backgroundSystematicErrorsChannelThree);
	TVectorD signalErrorsChannelThree(AXIONELECTRONLIMIT_CHANNEL_SYSTEMATICERRORS,signalSystematicErrorsChannelThree);

	TObjArray* namesSystematicErrorsChannelOne = new TObjArray();
	TObjString name0ChannelOne(AXIONELECTRONLIMIT_SYSTEMATIC0_NAME);
	TObjString name1ChannelOne(AXIONELECTRONLIMIT_SYSTEMATIC1_NAME);
	TObjString name2ChannelOne(AXIONELECTRONLIMIT_SYSTEMATIC2_NAME);
	TObjString name3ChannelOne(AXIONELECTRONLIMIT_SYSTEMATIC3_NAME);
	TObjString name4ChannelOne(AXIONELECTRONLIMIT_SYSTEMATIC4_NAME);
	TObjString name5ChannelOne(AXIONELECTRONLIMIT_SYSTEMATIC5_NAME);

	namesSystematicErrorsChannelOne->AddLast(&name0ChannelOne);
	namesSystematicErrorsChannelOne->AddLast(&name1ChannelOne);
	namesSystematicErrorsChannelOne->AddLast(&name2ChannelOne);
	namesSystematicErrorsChannelOne->AddLast(&name3ChannelOne);
	namesSystematicErrorsChannelOne->AddLast(&name4ChannelOne);
	namesSystematicErrorsChannelOne->AddLast(&name5ChannelOne);

	TObjArray* namesSystematicErrorsChannelTwo = new TObjArray();
	TObjString name0ChannelTwo(AXIONELECTRONLIMIT_SYSTEMATIC0_NAME);
	TObjString name1ChannelTwo(AXIONELECTRONLIMIT_SYSTEMATIC1_NAME);
	TObjString name2ChannelTwo(AXIONELECTRONLIMIT_SYSTEMATIC2_NAME);
	TObjString name3ChannelTwo(AXIONELECTRONLIMIT_SYSTEMATIC3_NAME);
	TObjString name4ChannelTwo(AXIONELECTRONLIMIT_SYSTEMATIC4_NAME);
	TObjString name5ChannelTwo(AXIONELECTRONLIMIT_SYSTEMATIC5_NAME);

	namesSystematicErrorsChannelTwo->AddLast(&name0ChannelTwo);
	namesSystematicErrorsChannelTwo->AddLast(&name1ChannelTwo);
	namesSystematicErrorsChannelTwo->AddLast(&name2ChannelTwo);
	namesSystematicErrorsChannelTwo->AddLast(&name3ChannelTwo);
	namesSystematicErrorsChannelTwo->AddLast(&name4ChannelTwo);
	namesSystematicErrorsChannelTwo->AddLast(&name5ChannelTwo);

	TObjArray* namesSystematicErrorsChannelThree = new TObjArray();
	TObjString name0ChannelThree(AXIONELECTRONLIMIT_SYSTEMATIC0_NAME);
	TObjString name1ChannelThree(AXIONELECTRONLIMIT_SYSTEMATIC1_NAME);
	TObjString name2ChannelThree(AXIONELECTRONLIMIT_SYSTEMATIC2_NAME);
	TObjString name3ChannelThree(AXIONELECTRONLIMIT_SYSTEMATIC3_NAME);
	TObjString name4ChannelThree(AXIONELECTRONLIMIT_SYSTEMATIC4_NAME);
	TObjString name5ChannelThree(AXIONELECTRONLIMIT_SYSTEMATIC5_NAME);

	namesSystematicErrorsChannelThree->AddLast(&name0ChannelThree);
	namesSystematicErrorsChannelThree->AddLast(&name1ChannelThree);
	namesSystematicErrorsChannelThree->AddLast(&name2ChannelThree);
	namesSystematicErrorsChannelThree->AddLast(&name3ChannelThree);
	namesSystematicErrorsChannelThree->AddLast(&name4ChannelThree);
	namesSystematicErrorsChannelThree->AddLast(&name5ChannelThree);

	TLimitDataSource* dataSource = new TLimitDataSource();

	if(useChannelOne)
	{
	    if(useSystematicErrors)
	    {
		dataSource->AddChannel(signalChannelOne,
				       backgroundChannelOne,
				       dataChannelOne,
				       &signalErrorsChannelOne,
				       &backgroundErrorsChannelOne,
				       namesSystematicErrorsChannelOne);
	    }
	    else
	    {
		dataSource->AddChannel(signalChannelOne,
				       backgroundChannelOne,
				       dataChannelOne);
	    }
	}
	if(useChannelTwo)
	{
	    if(useSystematicErrors)
	    {
		dataSource->AddChannel(signalChannelTwo,
				       backgroundChannelTwo,
				       dataChannelTwo,
				       &signalErrorsChannelTwo,
				       &backgroundErrorsChannelTwo,
				       namesSystematicErrorsChannelTwo);
	    }
	    else
	    {
		dataSource->AddChannel(signalChannelTwo,
				       backgroundChannelTwo,
				       dataChannelTwo);
	    }
	}
	if(useChannelThree)
	{
	    if(useSystematicErrors)
	    {
		dataSource->AddChannel(signalChannelThree,
				       backgroundChannelThree,
				       dataChannelThree,
				       &signalErrorsChannelThree,
				       &backgroundErrorsChannelThree,
				       namesSystematicErrorsChannelThree);
	    }
	    else
	    {
		dataSource->AddChannel(signalChannelThree,
				       backgroundChannelThree,
				       dataChannelThree);
	    }
	}

	
	// create a limitCalculator...
	limitCalculator* myLimitCalculator = new limitCalculator(identifier,
								 dataSource,
								 useStatisticalErrors,
								 SIGNALGENERATOR_GE_GGAMMA);

	// ...and calculate the CLs95 limit quickly
	success += myLimitCalculator->calculateCLs95LimitFast(AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_MAX,
							      AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_MIN,
							      AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_STEP,
							      AXIONELECTRONLIMIT_SCAN_GE_GGAMMA_STEP/10,
							      showExpectedLimitOnly);
	

	if(success == 0)
	{
	    std::ofstream resultFile(("results/" + identifier + "-results.txt").c_str(),std::fstream::trunc);

	    resultFile << "#" << identifier << "\n";
	    resultFile << std::string(75,'#') << "\n";
	    if(!showExpectedLimitOnly)
	    {
		resultFile << "observed CLb: " << myLimitCalculator->getObservedCLb() << "\n";
		resultFile << "observed 95 % CLs limit at beta_gamma: " << myLimitCalculator->getObservedCLs95Limit() << "\n";
		resultFile << std::string(75,'#') << "\n";
	    }
	    resultFile << "expected 95 % CLs limit at beta_gamma: " << myLimitCalculator->getExpectedCLs95Limit() << "\n";
	    resultFile << "expected 95 % CLs limit (-1 sigma) at beta gamma: " << myLimitCalculator->getExpectedCLs95LimitMinus1Sigma() << "\n";
	    resultFile << "expected 95 % CLs limit (+1 sigma) at beta gamma: " << myLimitCalculator->getExpectedCLs95LimitPlus1Sigma() << "\n";
	    resultFile << std::string(75,'#') << "\n";

	    resultFile.close();

	    if(showExpectedLimitOnly)
	    {
		myLimitCalculator->createExclusionPlotWithExpectedLimit(AXIONELECTRONLIMIT_PLOT_G_A_GAMMA_MAX);
	    }
	    else
	    {
		myLimitCalculator->createExclusionPlotWithObservedLimit(AXIONELECTRONLIMIT_PLOT_G_A_GAMMA_MAX);
	    }
	}
	delete namesSystematicErrorsChannelOne;
	delete namesSystematicErrorsChannelTwo;
	delete namesSystematicErrorsChannelThree;
	delete dataSource;

	delete myLimitCalculator;
    }
    else{
	std::cout << "Not performing limit calculation. Success variable was != 0:\n"
		  << "success = " << success << std::endl;
    }

    //cleaning up and closing root file
    delete signalChannelOne;
    delete signalChannelTwo;
    delete signalChannelThree;
    delete backgroundChannelOne;
    delete backgroundChannelTwo;
    delete backgroundChannelThree;
    delete dataChannelOne;
    delete dataChannelTwo;
    delete dataChannelThree;
    file->Close();
    delete file;

    return success;
}
