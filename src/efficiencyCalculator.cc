#include "efficiencyCalculator.h"

efficiencyCalculator::efficiencyCalculator(std::string rootFilePath)
{
    // creator of the efficiency calculator object
    // sets member variables:
    //     file path to root file (argument of creator)
    //     fills std::vectors for energy ranges
    //         _rootTrees filled by getAndSetupRootTree function
    //     creates histogram for likelihood distribution
    _rootFile = new TFile(rootFilePath.c_str());

    // ENERGY RANGE 0
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE0));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE0_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE0_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE0_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE0_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE0_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE0_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE0_MAX);


    // ENERGY RANGE 1
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE1));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE1_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE1_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE1_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE1_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE1_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE1_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE1_MAX);


    // ENERGY RANGE 2
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE2));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE2_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE2_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE2_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE2_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE2_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE2_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE2_MAX);


    // ENERGY RANGE 3
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE3));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE3_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE3_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE3_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE3_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE3_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE3_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE3_MAX);


    // ENERGY RANGE 4
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE4));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE4_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE4_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE4_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE4_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE4_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE4_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE4_MAX);


    // ENERGY RANGE 5
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE5));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE5_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE5_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE5_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE5_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE5_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE5_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE5_MAX);


    // ENERGY RANGE 6
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE6));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE6_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE6_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE6_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE6_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE6_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE6_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE6_MAX);


    // ENERGY RANGE 7
    _rootTrees.   push_back(getAndSetupRootTree(EFFICIENCYCALCULATOR_ROOTTREE_RANGE7));
    _energyRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_ENERGY_RANGE7_LOW,
							 EFFICIENCYCALCULATOR_ENERGY_RANGE7_HIGH));
    _chargeRanges.push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_CHARGE_RANGE7_MIN,
							 EFFICIENCYCALCULATOR_CHARGE_RANGE7_MAX));
    _rmsYRanges.  push_back(std::pair<Double_t,Double_t>(EFFICIENCYCALCULATOR_RMSY_RANGE7_MIN,
							 EFFICIENCYCALCULATOR_RMSY_RANGE7_MAX));
    _lengthMaxs.  push_back(EFFICIENCYCALCULATOR_LENGTH_RANGE7_MAX);


    // LIKELIHOOD DISTRIBUTION
    _likelihoodMarlinDistribution = new TH1D("likelihoodMarlin","",500,5.,20.);
}

efficiencyCalculator::~efficiencyCalculator()
{
    for(std::vector<TTree*>::iterator iter = _rootTrees.begin(); iter < _rootTrees.end(); iter++)
    {
	(*iter)->Delete();
    }

    delete _likelihoodMarlinDistribution;

    _rootFile->Close();
    delete _rootFile;
}

Int_t efficiencyCalculator::findEfficiencySetting(chipRegions::region chipRegion, Double_t softwareEfficiency, Double_t *efficiencySetting)
{
    // function to find the correct efficiency setting for each energy region and write
    // them to efficiencySetting array
    // chipRegion:         considered region on the chip (gold, silver, bronze...)
    // softwareEfficiency: the desired software efficiency from which the cut value
    //                     is deduced
    // efficiencySetting:  array which stores the cut values, which are obtained

    Int_t success = 0;

    for(Int_t i = 0; i < EFFICIENCYCALCULATOR_NUMBER_RANGES; i++)
    {
	// for each tree generate the likelihood distribution
	success += generateLikelihoodMarlinDistribution(_rootTrees[i],chipRegion,(_chargeRanges[i]).first,(_chargeRanges[i]).second,_lengthMaxs[i],(_rmsYRanges[i]).first,(_rmsYRanges[i]).second);

	// and then call function to find cut value
	efficiencySetting[i] = findLikelihoodMarlinCutValue(softwareEfficiency);
    }

    return success;
}

Int_t efficiencyCalculator::evaluateEfficiencySetting(chipRegions::region chipRegion, Double_t *efficiencySetting, Double_t *softwareEfficiencies)
{
    // evaluate the chosen cut values by generating likelihood again and reverse
    // calculation as done in findEfficiencySetting. Calculate software efficiency
    // based on cut value
    Int_t success = 0;

    for(Int_t i = 0; i < EFFICIENCYCALCULATOR_NUMBER_RANGES; i++)
    {
	// generate likelihood
	success += generateLikelihoodMarlinDistribution(_rootTrees[i],chipRegion,(_chargeRanges[i]).first,(_chargeRanges[i]).second,_lengthMaxs[i],(_rmsYRanges[i]).first,(_rmsYRanges[i]).second);

        // calc software efficiency for cut value
	softwareEfficiencies[i] = getSoftwareEfficiency(efficiencySetting[i]);
    }

    return success;
}

Double_t efficiencyCalculator::findLikelihoodMarlinCutValue(Double_t softwareEfficiency)
{
    // this function finds the cut value for a given software efficiency
    Double_t cutValue(0.);
    Double_t efficiency(0.);

    Int_t i(0);

    // find the cutValue by looking for the biggest efficiency smaller than the desired
    // software efficiency. Done by integrating the histogram from 0 to cut (iterator of
    // loop) and normalizing by number of entries
    do
    {
	Double_t cut = 5. + (Double_t)i * 0.1;
	efficiency = _likelihoodMarlinDistribution->Integral(0, _likelihoodMarlinDistribution->FindBin(cut)) / _likelihoodMarlinDistribution->GetEntries();

	cutValue = cut;

	i++;
    }
    while(efficiency < softwareEfficiency);

    // last cutValue is wanted value
    return cutValue;
}

Double_t efficiencyCalculator::getSoftwareEfficiency(Double_t likelihoodMarlinCutValue)
{
    // calculate software efficiency for a given cut value by integrating the
    // histogram from 0 to bin which corresponds to cut value and normalizing
    // by number of entries in histogram
    // TODO: is it not obvious that the roughly correct efficiency is recovered?
    Double_t efficiency = _likelihoodMarlinDistribution->Integral(0,_likelihoodMarlinDistribution->FindBin(likelihoodMarlinCutValue)) / _likelihoodMarlinDistribution->GetEntries();

    return efficiency;
}

Int_t efficiencyCalculator::generateLikelihoodMarlinDistribution(TTree* rootTree, chipRegions::region chipRegion, Double_t totalChargeMin, Double_t totalChargeMax, Double_t lengthMax, Double_t rmsYMin, Double_t rmsYMax)
{
    // this function generates the likelihood distribution based on _likelihoodMarlin
    // variable, which is read from the Tree
    _likelihoodMarlinDistribution->Reset();

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
	    silverCutActive = true;
	    bronzeCutActive = true;
	    break;
	case chipRegions::region::chip:
	    wholeChipActive = true;
	    break;
	default:
	    std::cerr << "Error: Unknown chip region!" << std::endl;
	    return 1;
    }


    // run over all events and for each check all cuts and if they are met,
    // add likelihood value of this event to the _likelihoodMarlinDistribution histogram
    for(Int_t iEvent = 0; iEvent < rootTree->GetEntries(); iEvent++)
    {
	rootTree->GetEntry(iEvent);

	Bool_t goldCut = ( (_positionX >= CHIPREGIONS_GOLD_X_MIN) && (_positionX <= CHIPREGIONS_GOLD_X_MAX) &&
			   (_positionY >= CHIPREGIONS_GOLD_Y_MIN) && (_positionY <= CHIPREGIONS_GOLD_Y_MAX) );

	Double_t radius_xy = TMath::Sqrt( (_positionX - CHIPREGIONS_CHIP_CENTER_X)*(_positionX - CHIPREGIONS_CHIP_CENTER_X) +
					  (_positionY - CHIPREGIONS_CHIP_CENTER_Y)*(_positionY - CHIPREGIONS_CHIP_CENTER_Y));

	Bool_t silverCut = (radius_xy <= CHIPREGIONS_SILVER_RADIUS_MAX) && !goldCut;
	Bool_t bronzeCut = (radius_xy <= CHIPREGIONS_BRONZE_RADIUS_MAX) && !silverCut && !goldCut;

	Bool_t regionCut = (goldCutActive && goldCut) ||
	    (silverCutActive && silverCut) ||
	    (bronzeCutActive && bronzeCut);

	Bool_t chargeCut = (_totalCharge > totalChargeMin) && (_totalCharge < totalChargeMax);
	Bool_t rmsYCut = (_rmsY > rmsYMin) && (_rmsY < rmsYMax);
	Bool_t lengthCut = _length < lengthMax;
	Bool_t pixelCut = _numberOfPixels > 3;

	if(!pixelCut){continue;}
	if(!chargeCut){continue;}
	if(!rmsYCut){continue;}
	if(!lengthCut){continue;}

	if(wholeChipActive == false){
	    if(!regionCut){continue;}
	}

	_likelihoodMarlinDistribution->Fill(_likelihoodMarlin);
    }

    return 0;
}

TTree* efficiencyCalculator::getAndSetupRootTree(std::string rootTreeName)
{
    // this function creates a tree from the root file, which was given to
    // efficiencyCalculator creator with name of energy range
    // then branch addresses are set for all the variables we care about
    //     point to member variables for these variables

    TTree* tree = (TTree*)_rootFile->Get(rootTreeName.c_str());

    // TODO: understand if correct: each tree (for each energy range) all set
    //       their branch addresses to the address of the same member variable?
    //       i.e. if we get an entry from any of the trees, the member variables
    //       for the variables of the efficiencyCalculator object are set to
    //       the value found in that entry of that tree?
    tree->SetBranchAddress("EnergyFromCharge",&_energyFromCharge);
    tree->SetBranchAddress("LikelihoodMarlin",&_likelihoodMarlin);
    tree->SetBranchAddress("PositionX",&_positionX);
    tree->SetBranchAddress("PositionY",&_positionY);
    tree->SetBranchAddress("TotalCharge",&_totalCharge);
    tree->SetBranchAddress("Length",&_length);
    tree->SetBranchAddress("RmsTransverse",&_rmsY);
    tree->SetBranchAddress("NumberOfPixels",&_numberOfPixels);

    return tree;
}
