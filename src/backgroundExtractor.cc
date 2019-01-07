#include "backgroundExtractor.h"

backgroundExtractor::backgroundExtractor(std::string rootFilePath, std::string rootTreeName, Double_t measurementTime) :
    _measurementTime(measurementTime)
{
    // creator of background extractor 
    // basically only creates a dataExtractor object and sets measurement
    // time to member variable
    _dataExtractor = new dataExtractor(rootFilePath,rootTreeName);
}

backgroundExtractor::~backgroundExtractor()
{
    // destructor of background Extractor
    delete _dataExtractor;
}

void backgroundExtractor::printBackgroundRate(TH1D *backgroundHistogram, Double_t measurementTime){
    // this function can be used to pretty print 
    // the background rate in Counts / (keV s ) of the given histogram
    // also prints background rate integrated over 1 to 7 keV (compare w/ pn-CCD detector)

    // std::vector<Double_t> backgroundAllEvents;
    // std::vector<Double_t> background1_7_keV
    // Double_t backgroundAllEvents = 0;
    // Double_t background1_7_keV = 0;
    // for(int i = 1; i <= AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS; i++){
    // 	// run over all bins of the histogram and get bin content
    // 	Double_t bin_content = backgroundHistogram->GetBinContent(i);
    // 	// and the center, i.e. the energy, of the bin
    // 	Double_t bin_center = backgroundHistogram->GetBinCenter(i);
    // 	//std::cout << "bin center  " << bin_center << "  bin content  " << bin_content << std::endl;
    // 	// backgroundAllEvents.push_back(bin_content);
    // 	backgroundAllEvents += bin_content;
    // 	if( (bin_center >= 1) &&
    // 	    (bin_center <= 7) ){
    // 	    //background1_7_keV.push_back(bin_content);
    // 	    background1_7_keV += bin_content;
    // 	}
    // }
    
    std::cout << "measurement times " << measurementTime << "  " << _measurementTime << std::endl;

    // caluclate the background using the integral of the bin
    Double_t backgroundInt;    
    Double_t backgroundRateInt;
    Double_t backgroundRatePerAreaKeV;
    //Double_t backgroundRate1_7keV;
    backgroundInt = backgroundHistogram->Integral(1, AXIONELECTRONLIMIT_CHANNEL_HISTOGRAM_BINS+1);

    // and calculate the rate based on the intrinsic measurement time

    backgroundRateInt = backgroundInt / _measurementTime;

    backgroundRatePerAreaKeV = backgroundRateInt / (10 - 0.2) / (0.25);
    std::cout << "integral background " << backgroundInt << std::endl;
    std::cout << "integral background rate " << backgroundRateInt << std::endl;
    std::cout << "integral background rate per area and kev " << backgroundRatePerAreaKeV << std::endl;
    // from this calculate the backround rate in the units we care about
    



    
    // now that we have the whole background, we can scale it back to a rate
    // Double_t backgroundRate;
    // Double_t backgroundRatePerAreaKeV;
    // backgroundRate = backgroundAllEvents / _measurementTime;
    // backgroundRatePerAreaKeV = backgroundRate / (10 - 0.2) / (0.25);
    // std::cout << "background rate all " << backgroundRate << std::endl;
    // std::cout << "background rate all per cm^2 and keV : " << backgroundRatePerAreaKeV << std::endl;
    // backgroundRate = backgroundAllEvents / measurementTime;
    // std::cout << "background rate all " << backgroundRate << std::endl;
    // Double_t backgroundRate1_7keV;
    // backgroundRate1_7keV = background1_7_keV / _measurementTime;
    // std::cout << "background rate1_7keV all " << backgroundRate1_7keV << std::endl;
    // backgroundRate1_7keV = background1_7_keV / measurementTime;
    // std::cout << "background rate1_7keV all " << backgroundRate1_7keV << std::endl;


}

Int_t backgroundExtractor::extractBackground(TH1D *backgroundHistogram, 
					     Bool_t setStatisticalErrors, 
					     chipRegions::region chipRegion, 
					     Double_t measurementTime, 
					     Double_t *efficiencySetting)
{
    // extract the background by scaling the background histogram by the measurement time
    Int_t success = _dataExtractor->extractData(backgroundHistogram,setStatisticalErrors,chipRegion,efficiencySetting);

    if(success != 0){return 1;}

    printBackgroundRate(backgroundHistogram, measurementTime);

    std::cout << "scaling histogram by " << measurementTime / _measurementTime << std::endl;
    backgroundHistogram->Scale(measurementTime/_measurementTime);

    return 0;
}

TGraphAsymmErrors* backgroundExtractor::getBackgroundratePlot(chipRegions::region chipRegion, 
							      Double_t *efficiencySetting, 
							      Double_t energyMin, 
							      Double_t energyMax, 
							      Int_t energyBins, 
							      EColor color, 
							      Bool_t prepareForLogPlot)
{
    gDirectory->cd(0);

    TH1D* histogram = new TH1D("histogram", "", energyBins, energyMin, energyMax);

    _dataExtractor->extractData(histogram, false, chipRegion, efficiencySetting);

    TGraphAsymmErrors* graph = new TGraphAsymmErrors(histogram->GetNbinsX());

    Double_t factor(1.e5);

    if(prepareForLogPlot){factor = 1.;}

    for(Int_t i = 0; i < histogram->GetNbinsX(); i++)
    {
	Double_t scale = factor/(_measurementTime*chipRegions::area(chipRegion)/100.*histogram->GetBinWidth(i+1));
	Double_t energy = histogram->GetBinCenter(i+1);
	Double_t error = histogram->GetBinWidth(i+1) / TMath::Sqrt(12.);
	Double_t entry = histogram->GetBinContent(i+1) * scale;
	Double_t errorUp = ( TMath::ChisquareQuantile(1.-((1.-0.68)/2.),2.*(histogram->GetBinContent(i+1)+1.)) / 2. - histogram->GetBinContent(i+1) ) * scale;
	Double_t errorDown = ( histogram->GetBinContent(i+1) - TMath::ChisquareQuantile((1.-0.68)/2.,2.*histogram->GetBinContent(i+1)) / 2. ) * scale;
	graph->SetPoint(i,energy,entry);
	graph->SetPointError(i,error,error,errorDown,errorUp);
    }
    graph->SetMarkerColor(color);
    graph->SetLineColor(color);
    graph->SetLineWidth(2);
    graph->SetTitle("");
    graph->GetXaxis()->SetTitle("Energy [keV]");
    graph->GetXaxis()->CenterTitle(true);
    if(prepareForLogPlot){graph->GetYaxis()->SetTitle("Background rate [/keV/cm^{2}/s]");}
    else{graph->GetYaxis()->SetTitle("Background rate [10^{-5} /keV/cm^{2}/s]");}
    graph->GetYaxis()->CenterTitle(true);

    delete histogram;

    return graph;
}


void backgroundExtractor::dumpBackgroundRate(std::string filename,
					     chipRegions::region chipRegion,
					     Double_t *efficiencySetting,
					     Double_t energyMin,
					     Double_t energyMax,
					     Int_t energyBins){
    // this function can be used to dump a signal or background rate plot to
    // filename. then we can use the python script PyS_plotRates.py to plot it
    // nicely... 
    gDirectory->cd(0);
    TH1D* histogram = new TH1D("histogram", "", energyBins, energyMin, energyMax);

    _dataExtractor->extractData(histogram, false, chipRegion, efficiencySetting);

    // second argument _measurement time is neccessary to scale data and background correctly
    _dataExtractor->dumpRatePlotFromHisto(filename, _measurementTime, histogram, chipRegion, energyMin, energyMax, energyBins);

    delete(histogram);
	
    return;
}
