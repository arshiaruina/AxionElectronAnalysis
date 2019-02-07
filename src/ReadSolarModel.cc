//Code to read the opacity files and extract the required tables from them to be accessed later
//Author: Arshia Ruina

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "ReadSolarModel.h"

int main(){	
	SolarModel f;
	//f.ReadAndStoreTable(); 
	//f.MetalMassFraction();
	f.ReadOpacityFileName();
    return 0;
}

 
int SolarModel::ReadAndStoreTable() {

	int lineNumber = 0;
	int i = 0;

	solarmodel_file.open(filename_solarmodel.c_str());

	if(!solarmodel_file.good()){
        	std::cout << "Problem with file!" << std::endl;
		return 1;
	}

        while(!solarmodel_file.eof() && lineNumber < 25){
        //while(!solarmodel_file.eof()){
		std::getline(solarmodel_file, line);
                std::istringstream iss_line(line);
                if(line.find("#")==0 || line.empty()) {
                        std::cout << "[DEBUG] Skipping line " << lineNumber << std::endl;
                        //++lineNumber;
                }
                else {
                        //std::cout << "[DEBUG] Reading line " << lineNumber << std::endl;
			row.push_back(ROW());
			iss_line >> row[i].massFrac >> row[i].radius >> row[i].temp >> row[i].density >> row[i].pressure >> row[i].lumiFrac >> row[i].H_massFrac >> row[i].He4_massFrac >> row[i].He3_massFrac >> row[i].C12_massFrac >> row[i].C13_massFrac >> row[i].N14_massFrac >> row[i].N15_massFrac >> row[i].O16_massFrac >> row[i].O17_massFrac >> row[i].O18_massFrac >> row[i].Ne_massFrac >> row[i].Na_massFrac >> row[i].Mg_massFrac >> row[i].Al_massFrac >> row[i].Si_massFrac >> row[i].P_massFrac >> row[i].S_massFrac >> row[i].Cl_massFrac >> row[i].Ar_massFrac >> row[i].K_massFrac >> row[i].Ca_massFrac >> row[i].Sc_massFrac >> row[i].Ti_massFrac >> row[i].V_massFrac >> row[i].Cr_massFrac >> row[i].Mn_massFrac >> row[i].Fe_massFrac >> row[i].Co_massFrac >> row[i].Ni_massFrac;
			row[i].He_massFrac = row[i].He4_massFrac + row[i].He3_massFrac;
			row[i].C_massFrac = row[i].C12_massFrac + row[i].C13_massFrac;
			row[i].N_massFrac = row[i].N14_massFrac + row[i].N15_massFrac;
			row[i].O_massFrac = row[i].O16_massFrac + row[i].O17_massFrac + row[i].O18_massFrac;
                        //std::cout << "[DEBUG] line: " << line << std::endl;
                        //std::cout << "[DEBUG] (iss_line) row contents: " << row[i].massFrac << " " <<  row[i].radius << " " <<  row[i].temp << " " <<  row[i].density << " " <<  row[i].pressure << " " <<  row[i].lumiFrac << " " <<  row[i].H_massFrac << " " <<  row[i].He_massFrac << " " <<  row[i].C12_massFrac << " " <<  row[i].C13_massFrac << " " <<  row[i].N14_massFrac << " " <<  row[i].N15_massFrac << " " <<  row[i].O16_massFrac << " " <<  row[i].O17_massFrac << " " <<  row[i].O18_massFrac << " " <<  row[i].Ne_massFrac << " " <<  row[i].Na_massFrac << " " <<  row[i].Mg_massFrac << " " <<  row[i].Al_massFrac << " " <<  row[i].Si_massFrac << " " <<  row[i].S_massFrac << " " <<  row[i].Ar_massFrac << " " <<  row[i].Ca_massFrac << " " <<  row[i].Cr_massFrac << " " <<  row[i].Mn_massFrac << " " <<  row[i].Fe_massFrac << " " <<  row[i].Ni_massFrac;
			std::cout << std::endl;	
                        ++i;
		}
                ++lineNumber;
	}


}

int SolarModel::MetalMassFraction() {

        std::cout << "[DEBUG] Row size: " << row.size() << std::endl;
        
	for(int i=0; i < row.size(); i++) {
	
		double total_massFrac = row[i].H_massFrac + row[i].He4_massFrac + row[i].He3_massFrac + row[i].C12_massFrac + row[i].C13_massFrac + row[i].N14_massFrac + row[i].N15_massFrac + row[i].O16_massFrac + row[i].O17_massFrac + row[i].O18_massFrac + row[i].Ne_massFrac + row[i].Na_massFrac + row[i].Mg_massFrac + row[i].Al_massFrac + row[i].Si_massFrac + row[i].P_massFrac + row[i].S_massFrac + row[i].Cl_massFrac + row[i].Ar_massFrac + row[i].K_massFrac + row[i].Ca_massFrac + row[i].Sc_massFrac + row[i].Ti_massFrac + row[i].V_massFrac + row[i].Cr_massFrac + row[i].Mn_massFrac + row[i].Fe_massFrac + row[i].Co_massFrac + row[i].Ni_massFrac;
		double non_metal_massFrac = row[i].H_massFrac + row[i].He_massFrac;
		double metal_massFrac_actual = 1.0 - non_metal_massFrac; 
		double metal_massFrac = row[i].C_massFrac + row[i].N_massFrac + row[i].O_massFrac + row[i].Ne_massFrac + row[i].Na_massFrac + row[i].Mg_massFrac + row[i].Al_massFrac + row[i].Si_massFrac + row[i].S_massFrac + row[i].Ar_massFrac + row[i].Ca_massFrac + row[i].Cr_massFrac + row[i].Mn_massFrac + row[i].Fe_massFrac + row[i].Ni_massFrac;
		/* metal_massFrac_actual and metal_massFrac are not equal
		   the former is slightly larger than the latter (difference is of the order 10**-5)
		   reason: while summing for metal_massFrac, we are inlcuding only those elements 
		   which are available in the opacity tables
		   for our purposes, we will consider the metal_massFrac as the total metal mass frac
		   reason: that's what corresponds to the opacity tables */

		//std::cout << "[DEBUG] total massFrac: " << total_massFrac << std::endl;
		//std::cout << "[DEBUG] H_massFrac: " << row[i].H_massFrac << std::endl;
		//std::cout << "[DEBUG] He_massFrac: " << row[i].He_massFrac << std::endl;
		//std::cout << "[DEBUG] C_massFrac: " << row[i].C_massFrac << std::endl;
		//std::cout << "[DEBUG] N_massFrac: " << row[i].N_massFrac << std::endl;
		//std::cout << "[DEBUG] O_massFrac: " << row[i].O_massFrac << std::endl;	
		//std::cout << "[DEBUG] Non-metal massFrac: " << non_metal_massFrac << std::endl;
		//std::cout << "[DEBUG] Metal massFrac: " << metal_massFrac << std::endl;
		//std::cout << "[DEBUG] Metal massFrac summed: " << metal_massFrac_summed << std::endl;
		
		row[i].total_metal_massFrac = metal_massFrac;
		for(int j=0;j<15;j++)
			row[i].X_Z.push_back(0.0);
		row[i].X_Z[0] = row[i].C_massFrac  / row[i].total_metal_massFrac;	//row[i].X_C  
		row[i].X_Z[1] = row[i].N_massFrac  / row[i].total_metal_massFrac;	//row[i].X_N  
		row[i].X_Z[2] = row[i].O_massFrac  / row[i].total_metal_massFrac;	//row[i].X_O  
		row[i].X_Z[3] = row[i].Ne_massFrac / row[i].total_metal_massFrac;	//row[i].X_Ne 
		row[i].X_Z[4] = row[i].Na_massFrac / row[i].total_metal_massFrac;	//row[i].X_Na 
		row[i].X_Z[5] = row[i].Mg_massFrac / row[i].total_metal_massFrac;	//row[i].X_Mg 
		row[i].X_Z[6] = row[i].Al_massFrac / row[i].total_metal_massFrac;	//row[i].X_Al 
		row[i].X_Z[7] = row[i].Si_massFrac / row[i].total_metal_massFrac;	//row[i].X_Si 
		row[i].X_Z[8] = row[i].S_massFrac  / row[i].total_metal_massFrac;	//row[i].X_S  
		row[i].X_Z[9] = row[i].Ar_massFrac / row[i].total_metal_massFrac;	//row[i].X_Ar 
		row[i].X_Z[10] = row[i].Ca_massFrac / row[i].total_metal_massFrac;	//row[i].X_Ca 
		row[i].X_Z[11] = row[i].Cr_massFrac / row[i].total_metal_massFrac;	//row[i].X_Cr 
		row[i].X_Z[12] = row[i].Mn_massFrac / row[i].total_metal_massFrac;	//row[i].X_Mn 
		row[i].X_Z[13] = row[i].Fe_massFrac / row[i].total_metal_massFrac;	//row[i].X_Fe 
		row[i].X_Z[14] = row[i].Ni_massFrac / row[i].total_metal_massFrac;	//row[i].X_Ni 
		
		std::cout << "Row number " << i << std::endl;	
		std::cout << row[i].X_C  << std::endl;  
                std::cout << row[i].X_N  << std::endl;
                std::cout << row[i].X_O  << std::endl;
                std::cout << row[i].X_Ne << std::endl;
                std::cout << row[i].X_Na << std::endl;
                std::cout << row[i].X_Mg << std::endl;
                std::cout << row[i].X_Al << std::endl;
                std::cout << row[i].X_Si << std::endl;
                std::cout << row[i].X_S  << std::endl;
                std::cout << row[i].X_Ar << std::endl;
                std::cout << row[i].X_Ca << std::endl;
                std::cout << row[i].X_Cr << std::endl;
                std::cout << row[i].X_Mn << std::endl;
                std::cout << row[i].X_Fe << std::endl;
                std::cout << row[i].X_Ni << std::endl;
		std::cout << std::endl;
	}
}

void SolarModel::ReadOpacityFileName(){

	//for(int i=0; i < OpacityFiles.size(); i++) {
	for(int i=0; i < 20; i++) {
		//std::vector<std::string> X_Z;
		std::cout << "Reading filename: " << OpacityFiles[i] << std::endl;
		sthttp://www.cplusplus.com/forum/beginner/68340/d::string name = OpacityFiles[i];	

		//XZ_VecForOpFile.push_back(std::vector<std::string>);
		std::vector<std::string> X_Z;
		XZ_VecForOpFile.push_back(X_Z);

		std::size_t start_pos = name.find("OP17") + 5;
		std::size_t end_pos = name.find("-",start_pos+1); // no need to check for E- for the first end_pos because
							 	  // the carbon mass fraction is given in full decimals
								  // and not in E notation for the files I have 
		std::size_t possible_end_pos = 0;
		//std::cout << "[DEBUG] " << "end_pos: " << end_pos << std::endl;
		//std::cout << "[DEBUG] " << "end: " << name.find("stored") << std::endl;
		//while(end_pos != std::string::npos){
		
		//std::cout << "[DEBUG] " << "name.find(\"stored\"): " << name.find("stored") << std::endl;

		while(end_pos < name.find("stored")) {

			//std::cout << "[DEBUG] " << start_pos << std::endl;
			//std::cout << "[DEBUG] " << end_pos << std::endl;

			std::string cooz = name.substr(start_pos,end_pos-start_pos);
			//std::cout << "[DEBUG] " << cooz << std::endl;

			X_Z.push_back(cooz);

			start_pos = end_pos + 1;	
			possible_end_pos = name.find("-",start_pos+1);
			//std::cout << "[DEBUG] " << "possible_end_pos:  " << possible_end_pos << std::endl;

			if(possible_end_pos!=std::string::npos && name.substr(possible_end_pos-1,1) == "E") { 
				//std::cout << "[DEBUG] " << "Found an " << name.substr(possible_end_pos-1,1) << std::endl;
				end_pos = name.find("-",possible_end_pos+1);
			}
			else
				end_pos = possible_end_pos;
			//std::cout << "[DEBUG] " << "start_pos: " << start_pos << std::endl;
			//std::cout << "[DEBUG] " << "end_pos: " << end_pos << std::endl;

		}

		//std::cout << "[DEBUG] " << "Hi!" << start_pos << "\t" << end_pos << std::endl;
		std::cout << name.length() << std::endl;
		std::string lastcooz = name.substr(start_pos,name.length()-7-start_pos);
		//std::cout << "[DEBUG] " << lastcooz << std::endl;	
		X_Z.push_back(lastcooz);
		//std::cout << "[DEBUG] " << "Length of string vector: " << X_Z.size() << std::endl;	
		XZ_VecForOpFile.back() = X_Z;
		//std::cout << "[DEBUG] " << "XZ_VecForOpFile stored" << std::endl;
		//std::cout << "[DEBUG] " << X_Z[0] << std::endl;
		//std::cout << "[DEBUG] " << XZ_VecForOpFile[0][0] << std::endl;
	}	

	std::cout << std::endl;

	//for(int i=0; i < OpacityFiles.size(); i++) {
	for(int i=0; i < 20; i++) {
		std::cout << "File: " << OpacityFiles[i] << std::endl;
		for(int j=0; j < 15; j++) {
			std::cout << "XZ_VecForOpFile: " << XZ_VecForOpFile[i][j] << std::endl;
		}
		std::cout << "End of file" << std::endl;
	}
}

int SolarModel::GetTableIndex() {

        int lineNumber = 0;
        int posX = 0;
        int posY = 0;
	std::vector<std::string> tableIndex;

	for(int i=0; i < row.size(); i++) {
		/*
			for each row, compare the computed metal mass fractions
			with the extracted X_Z for from every opacity file
			and the H and He mass fractions of every table inside
			and the R and T 
			find the 5 closest matching sets 
		*/        
	
		for(int j=0; j < OpacityFiles.size(); j++) {
        		int match=0;
			for(int k=0; k<15; k++){

				if(XZ_VecForOpFile[j][k] == row[i].X_Z[k]){
					++match;
				}
				if(match == 15){
		
					opacity_file.open(OpacityFiles[j].c_str());
				
        				if(!opacity_file.good()){
                				std::cout << "Problem with file!" << std::endl;
                				return 1;
        				}

        				while(!opacity_file.eof()){
                				std::getline(opacity_file, op_line);
                				++lineNumber;

                				if(lineNumber > 63 && lineNumber < 189){
                        	
                                			std::string tableIndex;
                                			posX = line.find("X");
                                			posY = line.find("Y");
                                			std::string _H_massFrac = line.substr(posX+2,6);
                                			std::string _He_massFrac = line.substr(posY+2,6);
                                			if(std::stod(_H_massFrac) == row[i].H_massFrac && std::stod(_He_massFrac) == row[i].He_massFrac){
                                        			tableIndex.push_back(line.substr(8,3));
                                        			//select[i].tableIndex = tableIndex;
                                        			//std::cout << "[DEBUG] Selected mass fraction of H: " << select[i].H_massFrac << std::endl;
                                        			//std::cout << "[DEBUG] Selected mass fraction of He: " << select[i].He_massFrac << std::endl;
                                        			//std::cout << "[DEBUG] Selected table: " << select[i].tableIndex << std::endl;
                                			}	
                        			}
        				}
        				opacity_file.close();
				}	
			}
		}
	}
}
