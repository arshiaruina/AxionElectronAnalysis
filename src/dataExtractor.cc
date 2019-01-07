#include "dataExtractor.h"

dataExtractor::dataExtractor(std::string dataRootFilePath, std::string dataRootTreeName)
{
    // creator of the dataExtractor object
    
    // create TFile object pointing to data file and create tree pointer
    _rootFile = new TFile(dataRootFilePath.c_str());
    _rootTree = (TTree*)_rootFile->Get(dataRootTreeName.c_str());

    // get tree entries and thus number of events
    _numberOfEvents = _rootTree->GetEntries();

    // set branch addresses for variables to member variables of extractor object
    _rootTree->SetBranchAddress("EnergyFromCharge",&_energyFromCharge);
    _rootTree->SetBranchAddress("LikelihoodMarlin",&_likelihoodMarlin);
    _rootTree->SetBranchAddress("PositionX",&_positionX);
    _rootTree->SetBranchAddress("PositionY",&_positionY);
    _rootTree->SetBranchAddress("RmsTransverse",&_rmsY);
    _rootTree->SetBranchAddress("NumberOfPixels",&_numberOfPixels);
}

dataExtractor::~dataExtractor()
{
    // destructor of dataExtractor
    delete _rootTree;
    _rootFile->Close();
    delete _rootFile;
}

Int_t dataExtractor::extractData(TH1D* dataHistogram, Bool_t setStatisticalErrors, chipRegions::region chipRegion, Double_t *efficiencySetting)
{
    // extract the data, as in get the events, which meet all cutting criteria 
    dataHistogram->Reset();

    Bool_t goldCutActive(false);
    Bool_t silverCutActive(false);
    Bool_t bronzeCutActive(false);
    Bool_t wholeChipActive(false);

    switch(chipRegion)
    {
	case chipRegions::region::gold:
	    goldCutActive = true;
	    break;
	case chipRegions::region::silver:
	    silverCutActive = true;
	    break;
	case chipRegions::region::bronze:
	    bronzeCutActive = true;
	    break;
	case chipRegions::region::goldPlusSilver:
	    goldCutActive = true;
	    silverCutActive =true;
	    break;
	case chipRegions::region::goldPlusSilverPlusBronze:
	    goldCutActive = true;
	    silverCutActive =true;
	    bronzeCutActive = true;
	    break;
	case chipRegions::region::chip:
	    wholeChipActive = true;
	    break;
	default:
	    std::cerr << "Error: Unknown chip region!" << std::endl;
	    return 1;
    }

    for(Int_t iEvent = 0; iEvent < _numberOfEvents; iEvent++)
    {
	_rootTree->GetEntry(iEvent);

	Bool_t goldCut = ( (_positionX >= CHIPREGIONS_GOLD_X_MIN) && (_positionX <= CHIPREGIONS_GOLD_X_MAX) &&
			   (_positionY >= CHIPREGIONS_GOLD_Y_MIN) && (_positionY <= CHIPREGIONS_GOLD_Y_MAX) );

	Double_t radius_xy = TMath::Sqrt( (_positionX - CHIPREGIONS_CHIP_CENTER_X)*(_positionX - CHIPREGIONS_CHIP_CENTER_X) +
					  (_positionY - CHIPREGIONS_CHIP_CENTER_Y)*(_positionY - CHIPREGIONS_CHIP_CENTER_Y));

	Bool_t silverCut = (radius_xy <= CHIPREGIONS_SILVER_RADIUS_MAX) && !goldCut;
	Bool_t bronzeCut = (radius_xy <= CHIPREGIONS_BRONZE_RADIUS_MAX) && !silverCut && !goldCut;

	Bool_t regionCut = ( (goldCutActive && goldCut) ||
			     (silverCutActive && silverCut) ||
			     (bronzeCutActive && bronzeCut) );

	Bool_t rmsYCut = _rmsY < 1.5;

	Bool_t likelihoodMarlinCut = ( ( ((_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE0_LOW)    && 
					  (_energyFromCharge <= EFFICIENCYCALCULATOR_ENERGY_RANGE0_HIGH)) && 
					 (_likelihoodMarlin) < efficiencySetting[0] ) ||
				       ( ((_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE1_LOW)    && 
					  (_energyFromCharge <= EFFICIENCYCALCULATOR_ENERGY_RANGE1_HIGH)) && 
					 (_likelihoodMarlin) < efficiencySetting[1] ) ||
				       ( ((_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE2_LOW)    && 
					  (_energyFromCharge <= EFFICIENCYCALCULATOR_ENERGY_RANGE2_HIGH)) && 
					 (_likelihoodMarlin) < efficiencySetting[2] ) ||
				       ( ((_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE3_LOW)    && 
					  (_energyFromCharge <= EFFICIENCYCALCULATOR_ENERGY_RANGE3_HIGH)) && 
					 (_likelihoodMarlin) < efficiencySetting[3] ) ||
				       ( ((_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE4_LOW)    && 
					  (_energyFromCharge <= EFFICIENCYCALCULATOR_ENERGY_RANGE4_HIGH)) && 
					 (_likelihoodMarlin) < efficiencySetting[4] ) ||
				       ( ((_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE5_LOW)    && 
					  (_energyFromCharge <= EFFICIENCYCALCULATOR_ENERGY_RANGE5_HIGH)) && 
					 (_likelihoodMarlin) < efficiencySetting[5] ) ||
				       ( ((_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE6_LOW)    && 
					  (_energyFromCharge <= EFFICIENCYCALCULATOR_ENERGY_RANGE6_HIGH)) && 
					 (_likelihoodMarlin) < efficiencySetting[6] ) ||
				       ( (_energyFromCharge > EFFICIENCYCALCULATOR_ENERGY_RANGE7_LOW)     && 
					 (_likelihoodMarlin) < efficiencySetting[7] ) );

	if(!rmsYCut){continue;}
	if(!likelihoodMarlinCut){continue;}
	if(wholeChipActive == false){
	    if(!regionCut){continue;}
	}

	dataHistogram->Fill(_energyFromCharge);
    }

    if(setStatisticalErrors)
    {
	dataHistogram->Sumw2();
    }
    else
    {
	for(Int_t iBin = 1; iBin <= dataHistogram->GetNbinsX(); iBin++)
	{
	    dataHistogram->SetBinError(iBin,0.);
	}
    }

    std::cout << "Printing size of histogram: " << dataHistogram->GetEntries() << std::endl;

    return 0;
}


void dataExtractor::dumpRatePlotFromHisto(std::string filename,
					  Double_t measurementTime,
					  TH1D *histogram,
					  chipRegions::region chipRegion,
					  Double_t energyMin,
					  Double_t energyMax,
					  Int_t energyBins){
    // this function can be used to dump a signal or background rate plot to
    // filename. then we can use the python script PyS_plotRates.py to plot it
    // nicely... 
    gDirectory->cd(0);

    ofstream outfile;
    outfile.open(filename);
    outfile << "# Energy [keV]    Background rate [10^{-5} /keV/cm^{2}/s]    xerr    error_up    error_down\n";

    Double_t factor(1.e5);
    std::cout << "factor " << factor << " measurement Time " << measurementTime  << " chip region area " << chipRegions::area(chipRegion)/100 << " bin width " << histogram->GetBinWidth(1) << std::endl;

    for(Int_t i = 0; i < histogram->GetNbinsX(); i++)
    {
	Double_t scale = factor/(measurementTime*chipRegions::area(chipRegion)/100.*histogram->GetBinWidth(i+1));
	Double_t energy = histogram->GetBinCenter(i+1);
	Double_t error = histogram->GetBinWidth(i+1) / TMath::Sqrt(12.);
	Double_t entry = histogram->GetBinContent(i+1) * scale;
	Double_t errorUp = ( TMath::ChisquareQuantile(1.-((1.-0.68)/2.),2.*(histogram->GetBinContent(i+1)+1.)) / 2. - histogram->GetBinContent(i+1) ) * scale;
	Double_t errorDown = ( histogram->GetBinContent(i+1) - TMath::ChisquareQuantile((1.-0.68)/2.,2.*histogram->GetBinContent(i+1)) / 2. ) * scale;

	outfile << energy << "\t" << entry << "\t" << error << "\t" << errorUp << "\t" << errorDown << "\n";
    }

    outfile.close();

    return;
}

void dataExtractor::dumpDataRate(std::string filename,
				 Double_t measurementTime, // measurement time here is the measurement time during data taking!
				 chipRegions::region chipRegion,
				 Double_t *efficiencySetting,
				 Double_t energyMin,
				 Double_t energyMax,
				 Int_t energyBins){
    // this function creates a histogram so that it can dump the data
    // using the dumpRatePlotFromHisto function
    // this function is called from axionElectronLimit.cc

    gDirectory->cd(0);
    TH1D* histogram = new TH1D("histogram", "", energyBins, energyMin, energyMax);

    extractData(histogram, false, chipRegion, efficiencySetting);

    dumpRatePlotFromHisto(filename, measurementTime, histogram, chipRegion, energyMin, energyMax, energyBins);

    delete(histogram);
	
    return;
}
