#include <iostream>
#include <iomanip>
#include <fstream>

int main() {
	
	
	std::string filename = "../resources/opacity_tables/OP17.1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored";
	std::string line;
	std::ifstream file;

	file.open(filename.c_str());

	if(!file.good()){
        	std::cout << "Problem with file!" << std::endl;
		return 1;
	}

	int lineNumber = 0;

	while(!file.eof()){
		std::getline(file, line);
		++lineNumber;
		if(lineNumber < 63 || lineNumber > 188) continue;
		
		//if(line.empty()) continue;
		//if(line[0] != '*') continue;
		//std::istringstream iss_line(line);
		std::string tableIndex = line.substr(7,3);
		int posX = line.find("X");
		int posY = line.find("Y");
		std::string H_massFrac = line.substr(posX+2,6);
		std::string He_massFrac = line.substr(posX+2,6);
		if(H_massFrac == "0.9980" && He_massFrac == "0.0000")
			std::cout << "Table Index: " << tableIndex << std::endl;
	}
}

//
//        // if not a comment, line should be added
//        // before we can add intensity, we need to include the conversion
//        // probability from axions to photons
//        iss_line >> _energy >> _axionIntensity;
//while (inFile >> x) {
//        sum = sum + x;
//    }
//
//    inFile.close();
//    cout << "Sum = " << sum << endl;
//    return 0;
//}
//
////--------------------------------------------------//
//
//// get ifstream to file which contains spectrum
//std::ifstream axionSpecIfStream;
//axionSpecIfStream.open(axionSpectrumPath.c_str(), std::ifstream::in);
//if(axionSpecIfStream.good() == false){
//    std::cout << "WARNING: Axion spectrum not found. May result in unexpected behaviour!"
//              << std::endl;
//}
//// and create empty branches for energy and intensity
//// treeAxionSpectrum->Branch("energy",    &_energy, 1000000);
//// treeAxionSpectrum->Branch("intensity", &_axionIntensity, 1000000);
//// now loop over file and append lines to tree branches
//
//while(axionSpecIfStream.good() &&
//      !axionSpecIfStream.eof()){
//
//    // as long as still something to read...
//    std::string line;
//    std::getline(axionSpecIfStream, line);
//    std::istringstream iss_line(line);
//    if(line[0] != '#'){
//        // if not a comment, line should be added
//        // before we can add intensity, we need to include the conversion
//        // probability from axions to photons
//        iss_line >> _energy >> _axionIntensity;
//
//        //filling myfluxhisto
//        myfluxhisto->Fill(_energy,_axionIntensity);
