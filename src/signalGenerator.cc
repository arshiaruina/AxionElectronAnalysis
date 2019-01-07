#include "signalGenerator.h"
#include <fstream>
#include <sstream>

#include <cstdlib>
#include "TGraphErrors.h"

signalGenerator::signalGenerator(std::string axionSpectrumPath,
				 std::string transmissionMylar2000nmPath,
				 std::string transmissionMylar900nmPath,
				 std::string transmissionAluminium40nmPath,
				 std::string transmissionAluminium20nmPath,
				 std::string transmissionSiliconNitride200nmPath,
				 std::string transmissionArgon30mmPath,
				 std::string effectiveAreaXRTPath,
				 std::string energyResolutionDetectorPath){
    // creator of the signalGenerator object
    // receives strings containing the paths to the axion electron spectrum as 
    // well as the transmission curves
    // after filling all trees from the data files, we need to interpolate
    // the transmission curves to the axion spectrum

    // set the directory to current working directory (I assume that's what 0 does?)
    gDirectory->cd(0);


    // fill energy ranges std::vector with energy ranges given in header
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE0_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE0_HIGH));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE1_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE1_HIGH));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE2_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE2_HIGH));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE3_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE3_HIGH));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE4_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE4_HIGH));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE5_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE5_HIGH));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE6_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE6_HIGH));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(SIGNALGENERATOR_ENERGY_RANGE7_LOW,
							 SIGNALGENERATOR_ENERGY_RANGE7_HIGH));

    // fill energy resolution std::vector with resolutions given in header
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE0);
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE1);
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE2);
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE3);
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE4);
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE5);
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE6);
    _energyResolutions.push_back(SIGNALGENERATOR_ENERGYRESOLUTION_RANGE7);


    // now we call the function to read the axion spectrum from file and convert 
    // it to an expected photon spectrum
    // function returns a pair of vectors
    std::pair<std::vector<Double_t>, std::vector<Double_t> > vecPair;
    vecPair = readAxionFluxAndConvert(axionSpectrumPath);
    // first element of pair is energy vector, second is intensity vector
    
    // now that we have the axion spectrum let's get all other spectra; we will get a spectrum,
    // fill up an std::vector for x and y values, and interpolate to the array of the axion spectrum
    // potentially we still need to drop elements of the axion spectrum afterwards (transmission curves
    // do not span the same range as axion spectrum)
    
    // first create a list off all paths
    std::list<const char *> path_list;
    path_list.push_front(transmissionMylar2000nmPath.c_str());
    path_list.push_front(transmissionMylar900nmPath.c_str());
    path_list.push_front(transmissionAluminium40nmPath.c_str());
    path_list.push_front(transmissionAluminium20nmPath.c_str());
    path_list.push_front(transmissionSiliconNitride200nmPath.c_str());
    path_list.push_front(transmissionArgon30mmPath.c_str());

    
    std::list<const char *> tree_names_list;
    tree_names_list.push_front("treeAxionSpectrum");
    tree_names_list.push_front("treeTransmissionMylar2000nm");
    tree_names_list.push_front("treeTransmissionMylar900nm");
    tree_names_list.push_front("TransmissionAluminium40nm");
    tree_names_list.push_front("treeTransmissionAluminium20nm");
    tree_names_list.push_front("treeTransmissionSiliconNitride200nm");
    tree_names_list.push_front("treeTransmissionArgon30mm");

    std::list<TTree *> tree_list;
    // vecPair.first is the energy vector from the readAxion... function
    // contains all energies of the axion spectrum, such that we can hand
    // it to interpolation helper function
    tree_list = read_interpolate_and_create_trees(path_list, tree_names_list, vecPair);
    

    // now assign _trees to elements of tree_list
    _treeTransmissionMylar2000nm         = tree_list.back();
    tree_list.pop_back();
    _treeTransmissionMylar900nm          = tree_list.back();
    tree_list.pop_back();
    _treeTransmissionAluminium40nm 	 = tree_list.back();
    tree_list.pop_back();
    _treeTransmissionAluminium20nm 	 = tree_list.back();
    tree_list.pop_back();
    _treeTransmissionSiliconNitride200nm = tree_list.back();
    tree_list.pop_back();
    _treeTransmissionArgon30mm           = tree_list.back();
    tree_list.pop_back();
    _treeAxionSpectrum                   = tree_list.back();
    tree_list.pop_back();
    
    // following line unnecessary, since we set the branch addresses in readAxionFluxAndConvert
    _treeAxionSpectrum->SetBranchAddress("energy",&_energy);
    _treeAxionSpectrum->SetBranchAddress("intensity",&_axionIntensity);
    _treeTransmissionMylar2000nm->SetBranchAddress("transmission",&_transmissionMylar2000nm);
    _treeTransmissionMylar900nm->SetBranchAddress("transmission",&_transmissionMylar900nm);
    _treeTransmissionAluminium40nm->SetBranchAddress("transmission",&_transmissionAluminium40nm);
    _treeTransmissionAluminium20nm->SetBranchAddress("transmission",&_transmissionAluminium20nm);
    _treeTransmissionSiliconNitride200nm->SetBranchAddress("transmission",&_transmissionSiliconNitride200nm);
    _treeTransmissionArgon30mm->SetBranchAddress("transmission",&_transmissionArgon30mm);

    // now load the XRT transmission spectrum
    _treeEffectiveAreaXRT = new TTree("treeEffectiveAreaXRT","");
    Int_t entries = _treeEffectiveAreaXRT->ReadFile(effectiveAreaXRTPath.c_str(),"energy:effarea:deffarea");

    Float_t energy;
    Float_t effectiveArea;

    _treeEffectiveAreaXRT->SetBranchAddress("energy",&energy);
    _treeEffectiveAreaXRT->SetBranchAddress("effarea",&effectiveArea);

    for(Int_t i = 0; i < entries; i++)
    {
	_treeEffectiveAreaXRT->GetEntry(i);
	_xrtEnergies.push_back((Double_t)energy);
	_xrtTransmissions.push_back((Double_t)effectiveArea / SIGNALGENERATOR_GEOMETRICAREAXRT);
    }

    delete _treeEffectiveAreaXRT;

    // create a tree for the energy resolution of the detector
    _treeEnergyResolutionDetector = new TTree("treeEnergyResolutionDetector","");
    entries = _treeEnergyResolutionDetector->ReadFile(energyResolutionDetectorPath.c_str(),"energy:resolution");
    Float_t resolution;
    _treeEnergyResolutionDetector->SetBranchAddress("energy",&energy);
    _treeEnergyResolutionDetector->SetBranchAddress("resolution",&resolution);
    // read the data from the tree into member vectors
    for(Int_t i = 0; i < entries; i++){
	_treeEnergyResolutionDetector->GetEntry(i);
	_detectorEnergies.push_back((Double_t)energy);
	_detectorResolutions.push_back((Double_t)resolution);
    }
    // and delete it again
    delete _treeEnergyResolutionDetector;

    _entriesTree = _treeAxionSpectrum->GetEntries();
    // check if axion spectrum was loaded correctly
    if(_entriesTree == 0)
    {
	std::cerr << "Error: Loading axion spectrum returned 0 lines. Do NOT proceed!" << std::endl;
    }
    
    // create and seed a TRandom3 object
    _randomGenerator = new TRandom3();
    _randomGenerator->SetSeed(0);
}



signalGenerator::~signalGenerator()
{
    // destructor of signalGenerator
    delete _treeAxionSpectrum;
    delete _treeTransmissionMylar2000nm;
    delete _treeTransmissionMylar900nm;
    delete _treeTransmissionAluminium40nm;
    delete _treeTransmissionAluminium20nm;
    delete _treeTransmissionSiliconNitride200nm;
    delete _treeTransmissionArgon30mm;

    delete _randomGenerator;
}


std::pair<std::vector<double>, std::vector<double> > signalGenerator::readAxionFluxAndConvert(std::string axionSpectrumPath){
    // this function reads the axion flux from file and performs the calculation
    // which is needed to get the flux of photons from the flux of axions

    // have to read the data by hand into vectors before we can fill a Tree with it
    std::vector<Double_t> energyVec;
    std::vector<Double_t> intensityVec;
    
    // in order to get the photon flux from the axion flux, we need the conversion
    // probability factor
    Double_t conversionProb;
    Double_t g_agamma = SIGNALGENERATOR_G_A_GAMMA;
    Double_t B = SIGNALGENERATOR_B_FIELD;
    Double_t L = SIGNALGENERATOR_B_LENGTH;
    Double_t m_a = SIGNALGENERATOR_AXION_MASS;
    // q is the momentum transfer of the axion to photon. below mass of axion of 10meV
    // given by q = m_a^2 / (2w)
    Double_t q;
    // delta is the argument of the sin 
    Double_t delta;
    // B and L given in T and m. convert both to ~keV
    // 1 T = (eV^2 / (1.44 * 10^(-3))) = 10^(-6) keV^2 / (1.44 * 10^(-3)) 
    B = B / 6.94e-4;
    // L conversion to 1/eV and then 1/keV
    L = L / (1.97227e-7 * 1e-3);
    // with these variables, we can calculate the conversion probability when we
    // run over the file for each energy

    // get ifstream to file which contains spectrum
    std::ifstream axionSpecIfStream;
    axionSpecIfStream.open(axionSpectrumPath.c_str(), std::ifstream::in);
    if(axionSpecIfStream.good() == false){
	std::cout << "WARNING: Axion spectrum not found. May result in unexpected behaviour!"
		  << std::endl;
    }
    // and create empty branches for energy and intensity
    // treeAxionSpectrum->Branch("energy",    &_energy, 1000000);
    // treeAxionSpectrum->Branch("intensity", &_axionIntensity, 1000000);
    // now loop over file and append lines to tree branches

    while(axionSpecIfStream.good() &&
	  !axionSpecIfStream.eof()){
	
	// as long as still something to read...
	std::string line;
	std::getline(axionSpecIfStream, line);
	std::istringstream iss_line(line);
	if(line[0] != '#'){
	    // if not a comment, line should be added
	    // before we can add intensity, we need to include the conversion
	    // probability from axions to photons
	    iss_line >> _energy >> _axionIntensity;
	    // calculate q using energy (m_a and _energy given in keV)
	    q = m_a * m_a / (2.0 * _energy);
	    // calculate argument of sin
	    delta = q * L / 2.0;
	    conversionProb = TMath::Power( ( (g_agamma * B * L / 2.0) * TMath::Sin(delta) / delta ) , 2 );
	    // now we just need to multiply our intensity with this factor
	    // need to also multiply by 10**-19, since flux in file given in 1 / (10**19 keV etc...)
	    _axionIntensity = _axionIntensity * conversionProb * 1.e-19;
	    // now we can fill the tree with the values of _energy and _axionIntensity
	    energyVec.push_back(_energy);
	    intensityVec.push_back(_axionIntensity);
	}
    }

    // after while loop, we can return create a pair of the two vectors 
    // and return it
    std::pair<std::vector<Double_t>, std::vector<Double_t> > vecPair;
    vecPair = std::make_pair(energyVec, intensityVec);

    return vecPair;
}

Int_t signalGenerator::generateSignal(TH1D* signalHistogram,
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
				      Bool_t smearEnergy){
    // generate the signal for a given window setup, flux fraction, software 
    // efficiency, X-ray transmission and measurement time
    signalHistogram->Reset();

    std::cout << "tree has entries " << _entriesTree << std::endl;
    // TODO: change transmissions to 1. use axion flux and 2. do proper 'convolution'
    for(Int_t iTree = 0; iTree < _entriesTree; iTree++)
    {
	setAllTreesToEntry(iTree);

	// for each tree basically just multiply all transmission curves etc.
	Double_t windowTransmission(1.);

	switch(detectorWindowMaterial){
	    case detector::windowMaterial::Mylar: windowTransmission *= TMath::Power(_transmissionMylar2000nm,detectorWindowThickness/2000.); break;
	    case detector::windowMaterial::SiN: windowTransmission *= TMath::Power(_transmissionSiliconNitride200nm,detectorWindowThickness/200.); break;
	    case detector::windowMaterial::None: break;
	}

	windowTransmission *= TMath::Power(_transmissionAluminium20nm,detectorWindowAlumniumLayerThickness/20.);
	windowTransmission *= detectorWindowAreaTransparency;
	switch(differentialWindowMaterial){
	    case detector::windowMaterial::Mylar: windowTransmission *= TMath::Power(_transmissionMylar900nm,differentialWindowThickness/900.); break;
	    case detector::windowMaterial::SiN: windowTransmission *= TMath::Power(_transmissionSiliconNitride200nm,differentialWindowThickness/200.); break;
	    case detector::windowMaterial::None: break;
	}

	Double_t detectorAbsorption = 1. - _transmissionArgon30mm;

	// TODO: check if this is correct for axions
	Double_t signal = _axionIntensity / SIGNALGENERATOR_AXIONSPECTRUM_BINAREA / SIGNALGENERATOR_AXIONSPECTRUM_BINTIME; // per mm² per s

	signal *= SIGNALGENERATOR_AREA_COLDBORE; //per s

	signal *= ((!perfectXRT)?getXRTTransmission(_energy):1.) * fluxFraction;
	signal *= windowTransmission;
	signal *= detectorAbsorption;
	signal *= softwareEfficiency;
	signal *= measurementTime;

	// get visible energy
	Double_t visibleEnergy = getVisibleEnergy(_energy);
	
	// TODO:!!! print signal and energy to file and plot in python to see what's going on!

	//std::cout << "signal and energy " << signal << "   " << _energy << std::endl;
	
	if(!smearEnergy){
	    signalHistogram->Fill(_energy,signal*SIGNALGENERATOR_AXIONSPECTRUM_BINWIDTH);
	}
	else{
	    for(Int_t iSample = 0; iSample < SIGNALGENERATOR_ENERGYSMEARING_SAMPLES; iSample++){
		// next line is chameleon last part
		// signalHistogram->Fill( (_randomGenerator->Gaus(visibleEnergy,getEnergyResolution(visibleEnergy)))/1000. ,signal*SIGNALGENERATOR_CHAMELEONSPECTRUM_BINWIDTH/(Double_t)SIGNALGENERATOR_ENERGYSMEARING_SAMPLES);		
		// TODO: make sure /1000 is correct where it is. Disable smear energy for now!
		signalHistogram->Fill(_randomGenerator->Gaus(_energy,getEnergyResolution(_energy)),signal*SIGNALGENERATOR_AXIONSPECTRUM_BINWIDTH);
	    }
	}
    }

    // potentially smear the energy somewhat
    if(smearEnergy)
    {
	signalHistogram->Scale(1./(Double_t)SIGNALGENERATOR_ENERGYSMEARING_SAMPLES);
    }

    //set bin errrors to zero, for signal only systematic errors are considered
    for(Int_t iBin = 1; iBin <= signalHistogram->GetNbinsX(); iBin++)
    {
	signalHistogram->SetBinError(iBin,0.);
    }

    return 0;
}

Double_t signalGenerator::getVisibleEnergy(Double_t energy){
    Double_t visibleEnergy(0.);

    if(energy < 3206){
	visibleEnergy = energy - 32;
    }
    else{
	visibleEnergy = energy - 140;
    }

    return visibleEnergy;
}

Double_t signalGenerator::getEnergyResolution(Double_t energy)
{

    Int_t i(0);

    while( (energy > _detectorEnergies[i]) && (i < (Int_t)_detectorEnergies.size()) ){
	i++;
    }

    Double_t energyResolution(0.);

    if( (i > 0) && (i < (Int_t)_detectorEnergies.size())){
	Double_t x0 = _detectorEnergies[i - 1];
	Double_t x1 = _detectorEnergies[i];
	Double_t y0 = _detectorResolutions[i -1];
	Double_t y1 = _detectorResolutions[i];

	energyResolution = y0 + (y1 - y0) / (x1 - x0) * (energy - x0);
    }
    else{
	// nice argument in there...
	energyResolution = _detectorResolutions[(i == 0)?i:(i-1)];
    }

    return energyResolution * energy;
}

Double_t signalGenerator::getXRTTransmission(Double_t energy){

    Int_t i(0);

    while( (energy > _xrtEnergies[i]) && (i < (Int_t)_xrtEnergies.size()) ){
	i++;
    }

    Double_t transmission(0.);

    if( (i > 0) && (i < (Int_t)_xrtEnergies.size())){
	Double_t x0 = _xrtEnergies[i - 1];
	Double_t x1 = _xrtEnergies[i];
	Double_t y0 = _xrtTransmissions[i -1];
	Double_t y1 = _xrtTransmissions[i];

	transmission = y0 + (y1 - y0) / (x1 - x0) * (energy - x0);
    }
    else
    {
	transmission = _xrtTransmissions[(i == 0)?i:(i-1)];
    }

    return transmission;
}


void signalGenerator::setAllTreesToEntry(Int_t entry)
{
    _treeAxionSpectrum->GetEntry(entry);
    _treeTransmissionMylar2000nm->GetEntry(entry);
    _treeTransmissionMylar900nm->GetEntry(entry);
    _treeTransmissionAluminium40nm->GetEntry(entry);
    _treeTransmissionAluminium20nm->GetEntry(entry);
    _treeTransmissionSiliconNitride200nm->GetEntry(entry);
    _treeTransmissionArgon30mm->GetEntry(entry);
}
