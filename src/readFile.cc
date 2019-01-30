//Code to read the opacity files and extract the required tables from them to be accessed later
//Author: Arshia Ruina

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

struct SELECT {
	std::string H_massFrac = "";
	std::string He_massFrac = "";
	std::string tableIndex = "";
	int lineNumber = 0;
};

int main() { //separate main from other functions	

	//TODO: filename has to be eventually chosen according to the [X_Z]	
	std::string in_filename = "../resources/opacity_tables/OP17.1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored";
	std::string line;
	std::ifstream in_file;

	int lineNumber = 0;
	int posX = 0;
	int posY = 0;
	int posTableIndex = 0;
	//TODO: the number of tables that have to extracted
	//this number would depend on the required combinations of H and He 
	//mass fractions for a particular set of [X_Z] 
	std::vector<SELECT> select;
	std::vector<std::string> H_massFrac_list = {"0.9980","0.9999"};	
	std::vector<std::string> He_massFrac_list = {"0.0000","0.0000"};	
	int input = H_massFrac_list.size();//or size of He_massFrac_list
	std::cout << "size of list: " << input << std::endl;

	for(int i = 0; i < input; i++) {
		select.push_back(SELECT());
		select.back().H_massFrac = H_massFrac_list[i];
		select.back().He_massFrac = He_massFrac_list[i];
	}
	
	in_file.open(in_filename.c_str());

	if(!in_file.good()){
        	std::cout << "Problem with file!" << std::endl;
		return 1;
	}

	while(!in_file.eof()){
		std::getline(in_file, line);
		++lineNumber;

		if(lineNumber > 63 && lineNumber < 189){		
			for(int i = 0; i < input; i++){
				//std::istringstream iss_line(line);
				std::string tableIndex;
				posX = line.find("X");
				posY = line.find("Y");
				std::string H_massFrac = line.substr(posX+2,6);
				std::string He_massFrac = line.substr(posY+2,6);
				if(H_massFrac == select[i].H_massFrac && He_massFrac == select[i].He_massFrac){ 
					tableIndex = line.substr(8,3);
					select[i].tableIndex = tableIndex;
					std::cout << "Selected mass fraction of H: " << select[i].H_massFrac << std::endl;
					std::cout << "Selected mass fraction of He: " << select[i].He_massFrac << std::endl;
					std::cout << "Selected table: " << select[i].tableIndex << std::endl;
				}	
			}
		}

		if(lineNumber > 240 ){	
			for(int i = 0; i < input; i++){
				std::string tableIndex; 
				std::size_t posTableIndex = line.find("TABLE");
				if (posTableIndex!=std::string::npos){
					tableIndex = line.substr(posTableIndex+7,3);
					std::cout << "Table number: " << tableIndex << std::endl;
				}
				else continue;
				if(tableIndex == select[i].tableIndex){
					select[i].lineNumber = lineNumber;
					std::cout << "Line number for start of table: " << lineNumber << std::endl;
				}
			}
		}
	}

	while(!in_file.eof()){
		for(int i = 0; i < input; i++){

			std::cout << "DEBUG: entered the loop to run over input file again and write the output file" << std::endl;
 
			std::stringstream s;
			std::ofstream out_file;
			std::string out_filename = "../resources/extracted_tables/" + select[i].H_massFrac + "-" + select[i].He_massFrac + "-" + in_filename.substr(32,60) + ".stored";
			out_file.open(out_filename);
	
			std::getline(in_file, line);
			++lineNumber;

			int line_counter = 0;
			if(lineNumber == select[i].lineNumber){
				std::cout << "Found line..." << std::endl;
				while(line_counter < 76){
					s << line << std::endl;
					++line_counter;
				}
			}
			std::cout << "Parsed table " << select[i].tableIndex << std::endl;
			out_file << s.str() << std::endl;
			out_file.close();
			std::cout << "Output file closed" << std::endl;
		}
	}
	in_file.close();
	std::cout << "END" << std::endl;
}

