//To read the opacity files and extract the required tables from them to be accessed later
//Author: Arshia Ruina

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include "ReadSolarModel.h"

int main(){	
	SolarModel f;
	//f.ReadAndStoreSolarModel(); 
	//f.MetalMassFraction();
	f.ReadOpacityFileName();
    	f.AccessSolarModel();
	
	return 0;
}

double SquaredDistance(double x1, double x2){
	return((x1-x2)*(x1-x2));
}

double ComputeDistance(double x1, double x2){
	return(std::fabs(x1-x2));
}

bool SortDistances(DISVAL& v1, DISVAL& v2){
	return (v1.dist < v2.dist);
}

//double SolarModel::AccessOpacityFile(std::string s, std::string H, std::string He, std::string R, std::string T);

/*--------------------------------------------------------------------------------------------------------
This function reads and stores the contents of the Solar Model in a vector called row
and simultaneously computes and stores the metal mass fractions for each row in another vector called X_Z.
The elements of X_Z will be compared to the opacity filenames to find the closest matching metal composition
for a particular row and later access that opacity table.
----------------------------------------------------------------------------------------------------------*/
int SolarModel::AccessSolarModel() {

	int lineNumber = 0;
	int i = 0;
	double total_massFrac = 0.0;
	double non_metal_massFrac = 1.0;
	double metal_massFrac_actual = 0.0;
	double metal_massFrac = 0.0;

	solarmodel_file.open(solarmodel_filename.c_str());

	if(!solarmodel_file.good()){
        	std::cout << "Problem with file!" << std::endl;
		return 1;
	}

        std::cout << "[INFO] Reading solar model file... " << std::endl;
	//++lineNumber;

        while(!solarmodel_file.eof() && lineNumber < 50) {
		std::getline(solarmodel_file, line);
                std::istringstream iss_line(line);
                if(line.find("#")==0 || line.empty()) {
                        std::cout << "[INFO] Skipping line " << lineNumber << std::endl;
                        //++lineNumber;
                }
                else {
                        std::cout << "[INFO] Reading line " << lineNumber << std::endl;
			row.push_back(ROW());
			iss_line >> row[i].massFrac >> row[i].radius >> row[i].temp >> row[i].density >> row[i].pressure >> row[i].lumiFrac >> row[i].H_massFrac >> row[i].He4_massFrac >> row[i].He3_massFrac >> row[i].C12_massFrac >> row[i].C13_massFrac >> row[i].N14_massFrac >> row[i].N15_massFrac >> row[i].O16_massFrac >> row[i].O17_massFrac >> row[i].O18_massFrac >> row[i].Ne_massFrac >> row[i].Na_massFrac >> row[i].Mg_massFrac >> row[i].Al_massFrac >> row[i].Si_massFrac >> row[i].P_massFrac >> row[i].S_massFrac >> row[i].Cl_massFrac >> row[i].Ar_massFrac >> row[i].K_massFrac >> row[i].Ca_massFrac >> row[i].Sc_massFrac >> row[i].Ti_massFrac >> row[i].V_massFrac >> row[i].Cr_massFrac >> row[i].Mn_massFrac >> row[i].Fe_massFrac >> row[i].Co_massFrac >> row[i].Ni_massFrac;
	
                        std::cout << "[INFO] Computing and storing isotope mass fractions..." << std::endl;
			row[i].He_massFrac = row[i].He4_massFrac + row[i].He3_massFrac;
			row[i].C_massFrac = row[i].C12_massFrac + row[i].C13_massFrac;
			row[i].N_massFrac = row[i].N14_massFrac + row[i].N15_massFrac;
			row[i].O_massFrac = row[i].O16_massFrac + row[i].O17_massFrac + row[i].O18_massFrac;
                        //std::cout << "[DEBUG] line: " << line << std::endl;
                        //std::cout << "[DEBUG] (iss_line) row contents: " << row[i].massFrac << " " <<  row[i].radius << " " <<  row[i].temp << " " <<  row[i].density << " " <<  row[i].pressure << " " <<  row[i].lumiFrac << " " <<  row[i].H_massFrac << " " <<  row[i].He_massFrac << " " <<  row[i].C12_massFrac << " " <<  row[i].C13_massFrac << " " <<  row[i].N14_massFrac << " " <<  row[i].N15_massFrac << " " <<  row[i].O16_massFrac << " " <<  row[i].O17_massFrac << " " <<  row[i].O18_massFrac << " " <<  row[i].Ne_massFrac << " " <<  row[i].Na_massFrac << " " <<  row[i].Mg_massFrac << " " <<  row[i].Al_massFrac << " " <<  row[i].Si_massFrac << " " <<  row[i].S_massFrac << " " <<  row[i].Ar_massFrac << " " <<  row[i].Ca_massFrac << " " <<  row[i].Cr_massFrac << " " <<  row[i].Mn_massFrac << " " <<  row[i].Fe_massFrac << " " <<  row[i].Ni_massFrac;
        
                        std::cout << "[INFO] Computing and storing total metal fraction and the required metal mass fractions (which will be later used to choose the opacity file)..." << std::endl;
			total_massFrac = row[i].H_massFrac + row[i].He4_massFrac + row[i].He3_massFrac + row[i].C12_massFrac + row[i].C13_massFrac + row[i].N14_massFrac + row[i].N15_massFrac + row[i].O16_massFrac + row[i].O17_massFrac + row[i].O18_massFrac + row[i].Ne_massFrac + row[i].Na_massFrac + row[i].Mg_massFrac + row[i].Al_massFrac + row[i].Si_massFrac + row[i].P_massFrac + row[i].S_massFrac + row[i].Cl_massFrac + row[i].Ar_massFrac + row[i].K_massFrac + row[i].Ca_massFrac + row[i].Sc_massFrac + row[i].Ti_massFrac + row[i].V_massFrac + row[i].Cr_massFrac + row[i].Mn_massFrac + row[i].Fe_massFrac + row[i].Co_massFrac + row[i].Ni_massFrac;
			non_metal_massFrac = row[i].H_massFrac + row[i].He_massFrac;
			metal_massFrac_actual = 1.0 - non_metal_massFrac; 
			metal_massFrac = row[i].C_massFrac + row[i].N_massFrac + row[i].O_massFrac + row[i].Ne_massFrac + row[i].Na_massFrac + row[i].Mg_massFrac + row[i].Al_massFrac + row[i].Si_massFrac + row[i].S_massFrac + row[i].Ar_massFrac + row[i].Ca_massFrac + row[i].Cr_massFrac + row[i].Mn_massFrac + row[i].Fe_massFrac + row[i].Ni_massFrac;
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
			
			//std::cout << "Row number " << i << std::endl;	
			//std::cout << row[i].X_C  << std::endl;  
	                //std::cout << row[i].X_N  << std::endl;
	                //std::cout << row[i].X_O  << std::endl;
	                //std::cout << row[i].X_Ne << std::endl;
	                //std::cout << row[i].X_Na << std::endl;
	                //std::cout << row[i].X_Mg << std::endl;
	                //std::cout << row[i].X_Al << std::endl;
	                //std::cout << row[i].X_Si << std::endl;
	                //std::cout << row[i].X_S  << std::endl;
	                //std::cout << row[i].X_Ar << std::endl;
	                //std::cout << row[i].X_Ca << std::endl;
	                //std::cout << row[i].X_Cr << std::endl;
	                //std::cout << row[i].X_Mn << std::endl;
	                //std::cout << row[i].X_Fe << std::endl;
	                //std::cout << row[i].X_Ni << std::endl;
			//std::cout << std::endl;
		
			//--------------------------------------------------------------------------------------//

			std::cout << "[INFO] Computing and storing total metal fraction and the required metal mass fractions (which will be later used to compute the number densities and absorption coefficients )..." << std::endl;
                        for(int j=0; j<28; j++)
                                row[i].X_Z_all.push_back(0.0);
                        row[i].X_Z_all[0] = row[i].H_massFrac   / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[1] = row[i].He4_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[2] = row[i].He3_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[3] = row[i].C12_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[4] = row[i].C13_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[5] = row[i].N14_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[6] = row[i].N15_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[7] = row[i].O16_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[8] = row[i].O17_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[9] = row[i].O18_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[10] = row[i].Ne_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[11] = row[i].Na_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[12] = row[i].Mg_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[12] = row[i].Al_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[13] = row[i].Si_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[14] = row[i].P_massFrac  / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[15] = row[i].S_massFrac  / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[16] = row[i].Cl_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[17] = row[i].Ar_massFrac / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[18] = row[i].K_massFrac  / row[i].total_metal_massFrac;        
                        row[i].X_Z_all[19] = row[i].Ca_massFrac / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[20] = row[i].Sc_massFrac / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[21] = row[i].Ti_massFrac / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[22] = row[i].V_massFrac  / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[23] = row[i].Cr_massFrac / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[24] = row[i].Mn_massFrac / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[25] = row[i].Fe_massFrac / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[26] = row[i].Co_massFrac / row[i].total_metal_massFrac;     
                        row[i].X_Z_all[27] = row[i].Ni_massFrac / row[i].total_metal_massFrac;     

			//--------------------------------------------------------------------------------------//
			
                        std::cout << "[INFO] Choosing the closest opacity file... " << std::endl;
			std::vector<DISVAL> OpFileDistVec;
			for(int j=0; j < OpacityFiles.size(); j++) { 
				OpFileDistVec.push_back(DISVAL());
				//OpFileDistVec.back().value = OpacityFiles[j];
				OpFileDistVec.back().index = j;
				double d = 0.0;
                 		for(int k=0; k < 15; k++) {
                         		d += SquaredDistance(row[i].X_Z[k],std::stod(XZ_VecForOpFile[j][k]));
                 		}	 
				OpFileDistVec.back().dist = std::sqrt(d);
         		}
			//std::sort(OpFileDistVec.begin(),OpFileDistVec.end(),SortDistances);
			sort(OpFileDistVec.begin(),OpFileDistVec.end(),SortDistances);
			//std::string SelectedOpacityFile = OpFileDistVec[0].value;			
			int SelectedOpacityFile = OpFileDistVec[0].index;			

			/*
			OpFileDistVec now contains the opacity file names arranged in ascending order
			of their distance to the metal mass fractions of this row.
			*/
			//std::vector<std::string> MatchedOpacityFiles;	
			
			//MatchedOpacityFile.push_back(std::string);
			//MatchedOpacityFile.back() = OpFileDistVec[0].filename;
			//MatchedOpacityFile.push_back(std::string);
			//MatchedOpacityFile.back() = OpFileDistVec[1].filename;
			//MatchedOpacityFile.push_back(std::string);
			//MatchedOpacityFile.back() = OpFileDistVec[2].filename; 

			/*H and He mass fractions have to be checked for simultaneously so that 
			a table can be chosen for a combination of both the values. Finding the 
			closest H and He mass fractions separately will give the closest matches
			but may not give us a table for which the H and He mass fractions are  both
			equal to the closest matched values!*/

                        std::cout << "[INFO] H mass fraction of this row: " << row[i].H_massFrac << std::endl;
                        std::cout << "[INFO] He mass fraction of this row: " << row[i].He_massFrac << std::endl;
                        std::cout << "[INFO] Choosing the closest combination of H and He mass fractions... " << std::endl;
                        std::vector<DISVAL> HandHe_massFrac_distVec;
                        //for(int j=0; j < H_massFrac_inOPfiles.size(); j++) { //H_massFrac_inOPfiles.size() = 126
                        for(int j=0; j < 126; j++) { //H_massFrac_inOPfiles.size() = 126
                               	double d_H = ComputeDistance(row[i].H_massFrac,H_massFrac_inOPfiles[j]);
                               	double d_He = ComputeDistance(row[i].He_massFrac,He_massFrac_inOPfiles[j]);
                               	HandHe_massFrac_distVec.push_back(DISVAL());
                               	HandHe_massFrac_distVec.back().dist = std::sqrt(d_H*d_H + d_He*d_He);        
                               	//H_massFrac_distVec.back().dist = ComputeDistance(row[i].H_massFrac,H_massFrac_inOPfiles[j]);
                        	//H_massFrac_distVec.back().value = std::to_string(H_massFrac_inOPfiles[j]);
                        	//HandHe_massFrac_distVec.back().value = std::to_string(j);
                        	HandHe_massFrac_distVec.back().index = j;
                        }
			
                        std::sort(HandHe_massFrac_distVec.begin(),HandHe_massFrac_distVec.end(),SortDistances);
                        //std::string SelectedHandHemassFrac = "";
                        int SelectedHandHemassFrac = -1;
                        if(HandHe_massFrac_distVec.size() > 0){
                                SelectedHandHemassFrac = HandHe_massFrac_distVec[0].index;
                        }
                        std::cout << "[INFO] H mass fraction found to be closest: " << H_massFrac_inOPfiles[SelectedHandHemassFrac] << std::endl;
                        std::cout << "[INFO] He mass fraction found to be closest: " << He_massFrac_inOPfiles[SelectedHandHemassFrac] << std::endl;


			/*
                        std::cout << "[INFO] H mass fraction of this row: " << row[i].H_massFrac << std::endl;
                        std::cout << "[INFO] Choosing the closest H mass fraction... " << std::endl;
			std::vector<DISVAL> H_massFrac_distVec;
			for(int j=0; j < H_massFrac_inOPfiles.size(); j++) { //H_massFrac_inOPfiles.size() = 126
				//double d = SquaredDistance(row[i].H_massFrac,H_massFrac_inOPfiles[j]);
				H_massFrac_distVec.push_back(DISVAL());
				//H_massFrac_distVec.back().dist = std::sqrt(d);	
				H_massFrac_distVec.back().dist = ComputeDistance(row[i].H_massFrac,H_massFrac_inOPfiles[j]);
				H_massFrac_distVec.back().value = std::to_string(H_massFrac_inOPfiles[j]);
			}
			std::sort(H_massFrac_distVec.begin(),H_massFrac_distVec.end(),SortDistances);
			std::string SelectedHmassFrac = "";
			if(H_massFrac_distVec.size() > 0){
				SelectedHmassFrac = H_massFrac_distVec[0].value;
			}
                        std::cout << "[INFO] H mass fraction found to be closest: " << SelectedHmassFrac << std::endl;

                        std::cout << "[INFO] He mass fraction of this row: " << row[i].He_massFrac << std::endl;
                        std::cout << "[INFO] Choosing the closest He mass fraction... " << std::endl;
			std::vector<DISVAL> He_massFrac_distVec;
			for(int j=0; j < He_massFrac_inOPfiles.size(); j++) { //He_massFrac_inOPfiles.size() = 126
				//double d = SquaredDistance(row[i].He_massFrac,He_massFrac_inOPfiles[j]);
				He_massFrac_distVec.push_back(DISVAL());
				//He_massFrac_distVec.back().dist = std::sqrt(d);	
				He_massFrac_distVec.back().dist = ComputeDistance(row[i].He_massFrac,He_massFrac_inOPfiles[j]);
				He_massFrac_distVec.back().value = std::to_string(He_massFrac_inOPfiles[j]);
			}
			std::sort(He_massFrac_distVec.begin(),He_massFrac_distVec.end(),SortDistances);
			std::string SelectedHemassFrac = "";
			if(He_massFrac_distVec.size() > 0) {
				SelectedHemassFrac = He_massFrac_distVec[0].value;
			}
                        std::cout << "[INFO] He mass fraction found to be closest: " << SelectedHemassFrac << std::endl;
			*/

                        std::cout << "[INFO] Density value of this row: " << row[i].density << std::endl;
			double var = std::log10( row[i].density / (row[i].temp * 1e-6) ); 
                        std::cout << "[INFO] log R value of this row: " << var << std::endl;
                        std::cout << "[INFO] Choosing the closest log R value... " << std::endl;
			std::vector<DISVAL> logR_distVec;
			for(int j=0; j < logR.size(); j++) { //logR.size() = 18
				/* x stores the density in a form that makes is comparable to how it is defined
				in the opacity tables: x = log R = log( row[i].density / (row[i].temp * 10e-6) ) */ 
				//double d = SquaredDistance(x,logR[j]);	
				logR_distVec.push_back(DISVAL());	
				//logR_distVec.back().dist = std::sqrt(d);	
				logR_distVec.back().dist = ComputeDistance(var,logR[j]);	
				//logR_distVec.back().value = std::to_string(logR[j]);	
				//logR_distVec.back().value = std::to_string(j);//index of the logR value	
				logR_distVec.back().index = j;//index of the logR value	
			}
			std::sort(logR_distVec.begin(),logR_distVec.end(),SortDistances);
			//std::string SelectedlogR = "";
			int SelectedlogR = -1;
			if(logR_distVec.size() > 0) {
				//SelectedlogR = logR_distVec[0].value;
				SelectedlogR = logR_distVec[0].index;
                        }
			//std::cout << "[INFO] log R found to be closest: " << logR[std::stod(SelectedlogR)] << std::endl;
			std::cout << "[INFO] log R found to be closest: " << logR[SelectedlogR] << std::endl;


                        std::cout << "[INFO] Temperature value of this row: " << row[i].temp << std::endl;
                        std::cout << "[INFO] log T value of this row: " << std::log10(row[i].temp) << std::endl;
                        std::cout << "[INFO] Choosing the closest log T value... " << std::endl;
			std::vector<DISVAL> logT_distVec;
			for(int j=0; j < logT.size(); j++) { //logT.size() = 70
				//double d = SquaredDistance(std::log(row[i].temp),logT[j]);
				logT_distVec.push_back(DISVAL());	
				//logT_distVec.back().dist = std::sqrt(d);	
				logT_distVec.back().dist = ComputeDistance(std::log10(row[i].temp),logT[j]);
				//logT_distVec.back().value = std::to_string(logT[j]);	
				//logT_distVec.back().value = std::to_string(j);//index of the logT value	
				logT_distVec.back().index = j;//index of the logT value	
			}
			std::sort(logT_distVec.begin(),logT_distVec.end(),SortDistances);
			//std::string SelectedlogT = logT_distVec[0].value;
			//std::string SelectedlogT = "";
			int SelectedlogT = -1;
			if(logT_distVec.size() > 0) {
				SelectedlogT = logT_distVec[0].index;
                        }
			//std::cout << "[INFO] log T found to be closest: " << logT[std::stod(SelectedlogT)] << std::endl;
			std::cout << "[INFO] log T found to be closest: " << logT[SelectedlogT] << std::endl;



			// Add up all the distances to find the least distance!
			//std::vector<DISVAL> TotalDistVec;
			//for(j=0; j<3; j++) {
			//TotalDistVec.push_back(DISVAL);
			//TotalDistVec.back().dist = sqrt( std::pow(OpFileDistVec[0].dist,2) + std::pow(H_massFrac_distVec[0].dist,2) + std::pow(He_massFrac_distVec[0].dist,2) + std::pow(logR_distVec[0].dist,2) + std::pow(logT_distVec[0].dist,2) );
			//TotalDistVec.value = OpFileDistVec[j].value;
			//}
			//sort(TotalDistVec.begin(),TotalDistVec.end(),SortDistances);
			
                        std::cout << "[INFO] Calling the function to access the chosen opacity file and obtain the opacity value for the chosen H and He mass fractions, log R and log T values..." << std::endl;
			//row[i].opacity_value = AccessOpacityFile(SelectedOpacityFile, SelectedHmassFrac, SelectedHemassFrac, SelectedlogR, SelectedlogT);
			row[i].opac_xsec = AccessOpacityFile(SelectedOpacityFile, SelectedHandHemassFrac, SelectedlogR, SelectedlogT);
	
			std::cout << "Opacity xsec stored for this row: " << row[i].opac_xsec<< std::endl;

			std::cout << std::endl;	
                        ++i;
		}	
	        ++lineNumber;
	}
}

/*----------------------------------------------------------------------------------------
This function reads all the opacity filenames, extracts the metal mass fractions from them
and stores them in a vector of string vectors called XZ_VecForOpFile where each element is 
a vector of the mass fractions corresponding to the opacity file denoted by its position.
The opacity filenames are stored in a global vector called OpacityFiles. 
----------------------------------------------------------------------------------------*/
void SolarModel::ReadOpacityFileName(){

	for(int i=0; i < OpacityFiles.size(); i++) {
	//for(int i=0; i < 20; i++)  {
		//std::vector<std::string> X_Z;
		std::cout << "Reading filename: " << OpacityFiles[i] << std::endl;
		sthttp://www.cplusplus.com/forum/beginner/68340/d::string name = OpacityFiles[i];	

		//XZ_VecForOpFile.push_back(std::vector<std::string>);
		std::vector<std::string> X_Z;
		XZ_VecForOpFile.push_back(X_Z);
		
		//std::cout << "DEBUG 1" << std::endl;

		std::vector<double> X_Z_numeric;
		XZ_VecForOpFile_numeric.push_back(X_Z_numeric); 

		//std::cout << "DEBUG 2" << std::endl;
		
		std::string name = OpacityFiles[i];
		std::size_t start_pos = name.find("OP17") + 5;
		std::size_t end_pos = name.find("-",start_pos+1); // no need to check for E- for the first end_pos because
							 	  // the carbon mass fraction is given in full decimals
								  // and not in E notation for the files I have 
		
		//std::cout << "DEBUG 3" << std::endl;
	
		std::size_t possible_end_pos = 0;
		//std::cout << "[DEBUG] " << "end_pos: " << end_pos << std::endl;
		//std::cout << "[DEBUG] " << "end: " << name.find("stored") << std::endl;
		//while(end_pos != std::string::npos){
		
		//std::cout << "[DEBUG] " << "name.find(\"stored\"): " << name.find("stored") << std::endl;

		//std::cout << "DEBUG 4" << std::endl;

		while(end_pos < name.find("stored")) {

			//std::cout << "[DEBUG] " << start_pos << std::endl;
			//std::cout << "[DEBUG] " << end_pos << std::endl;

			//std::cout << "DEBUG 5" << std::endl;
			
			std::string cooz = name.substr(start_pos,end_pos-start_pos);
			//std::cout << "[DEBUG] " << cooz << std::endl;

			X_Z.push_back(cooz);
			X_Z_numeric.push_back(std::stod(cooz));

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
		//std::cout << "DEBUG 6" << name.length() << std::endl;
		//std::cout << "DEBUG 7" << std::endl;
		std::string lastcooz = name.substr(start_pos,name.length()-7-start_pos);
		//std::cout << "[DEBUG] " << lastcooz << std::endl;	
		X_Z.push_back(lastcooz);
		X_Z_numeric.push_back(std::stod(lastcooz));
		//std::cout << "[DEBUG] " << "Length of string vector: " << X_Z.size() << std::endl;	
		XZ_VecForOpFile.back() = X_Z;
		XZ_VecForOpFile_numeric.back() = X_Z_numeric;
		//std::cout << "[DEBUG] " << "XZ_VecForOpFile stored" << std::endl;
		//std::cout << "[DEBUG] " << X_Z[0] << std::endl;
		//std::cout << "[DEBUG] " << XZ_VecForOpFile[0][0] << std::endl;
		//std::cout << "DEBUG 8" << std::endl;
	}	

	std::cout << std::endl;
	std::cout << "OpacityFiles.size() = " << OpacityFiles.size() << std::endl; 
	std::cout << "XZ_VecForOpFile.size() = " << XZ_VecForOpFile.size() << std::endl; 

	for(int i=0; i < OpacityFiles.size(); i++) {
	//for(int i=0; i < 20; i++) {
		std::cout << "File: " << OpacityFiles[i] << std::endl;
		for(int j=0; j < 15; j++) {
			std::cout << "XZ_VecForOpFile: " << XZ_VecForOpFile[i][j] << std::endl;
			std::cout << "XZ_VecForOpFile_numeric: " << XZ_VecForOpFile_numeric[i][j] << std::endl;
		}
		std::cout << "End of file" << std::endl;
	} 
}

/*
compare XZ vector for ith row with XZ vector for jth file
find distance between row[i].X_Z[k] and XZ_VecForOpFile[j][k]
*/

/*----------------------------------------------------------------------------------------
This function compares the metal mass fraction vectors for a row from the Solar Model 
to those extracted from the opacity filenames. The latter have to be first coverted
to numeric type with the correct precision before making the comparison.

For every row, a distance is computed and stored corresponding to every opacity filename.
The distances are then sorted such that we can choose the filename with the least distance
for that row. 
-----------------------------------------------------------------------------------------*/	
/*int SolarModel::CompareMetalMassFractions() { 
	
	for(int i=0; i < OpacityFiles.size(); i++) {
		for(int j=0; j < 15; j++) {
			SquareDistance(row[i],XZ_VecForOpFile[i][j]
		}	
	}

}*/


//double AccessOpacityFile(std::string s, std::string H, std::string He, std::string R, std::string T) {
//double SolarModel::AccessOpacityFile(std::string s, std::string H, std::string He, std::string R, std::string T) {
double SolarModel::AccessOpacityFile(int s, int HandHe, int R, int T) {

        int lineNumber = 0;
	int startTable_lineNumber = 0;                                 
        int posX = 0;
        int posY = 0;
	std::string tableIndex;
	std::string selected_tableIndex;
	//std::vector<std::string> tableIndex;
			

	std::ifstream opacity_file;
	//opacity_file.open(s.c_str());
	opacity_file.open(OpacityFiles[s].c_str());

	//std::cout << "[INFO] Accessing opacity file: " << s.c_str() << std::endl;
	//std::cout << "[INFO] Looking for H mass fraction: " << H << std::endl;
	//std::cout << "[INFO] Looking for He mass fraction: " << He << std::endl;
	//std::cout << "[INFO] Looking for log R value: " << logR[std::stoi(R)] << std::endl;
	//std::cout << "[INFO] Looking for log T value: " << logT[std::stoi(T)] << std::endl;

	std::cout << "[INFO] Accessing opacity file: " << OpacityFiles[s].c_str() << std::endl;
	std::cout << "[INFO] Looking for H mass fraction: " << H_massFrac_inOPfiles[HandHe] << std::endl;
	std::cout << "[INFO] Looking for He mass fraction: " << He_massFrac_inOPfiles[HandHe] << std::endl;
	std::cout << "[INFO] Looking for log R value: " << logR[R] << std::endl;
	std::cout << "[INFO] Looking for log T value: " << logT[T] << std::endl;

	if(!opacity_file.good()){
		std::cout << "Problem with file!" << std::endl;
		return 1;
	}

	while(!opacity_file.eof()){
		std::string op_line = "";
		std::getline(opacity_file, op_line);
                std::istringstream iss_op_line(op_line);

		++lineNumber;	
		
		/*		
		//std::cout << "[INFO] Finding table index by comparing the H and He mass fractions..." << std::endl;
		if(lineNumber > 63 && lineNumber < 189){

			posX = op_line.find("X");
			posY = op_line.find("Y");
			std::string _H_massFrac = op_line.substr(posX+2,6);
			std::string _He_massFrac = op_line.substr(posY+2,6);
			//std::cout << "[DEBUG] _H_massFrac: " << _H_massFrac << std::endl;
			//std::cout << "[DEBUG] _He_massFrac: " << _He_massFrac << std::endl;
			
			//if(std::stod(_H_massFrac) == row[i].H_massFrac && std::stod(_He_massFrac) == row[i].He_massFrac){
			//if(std::stod(_H_massFrac) == H.c_str() && std::stod(_He_massFrac) == He.c_str()){
			//if(_H_massFrac == H.c_str() && _He_massFrac == He.c_str()){
			//if(std::stod(_H_massFrac) == std::stod(H) && std::stod(_He_massFrac) == std::stod(He)){
			if(std::stod(_H_massFrac) == H_massFrac_inOPfiles[HandHe] && std::stod(_He_massFrac) == He_massFrac_inOPfiles[HandHe]){
        			selected_tableIndex = op_line.substr(8,3);
				//tableIndex.push_back(line.substr(8,3));
        			//select[i].tableIndex = tableIndex;
        			//std::cout << "[DEBUG] Selected mass fraction of H: " << select[i].H_massFrac << std::endl;
        			//std::cout << "[DEBUG] Selected mass fraction of He: " << select[i].He_massFrac << std::endl;
       				std::cout << "[INFO] Selected table: " << selected_tableIndex << std::endl;
       				//std::cout << "[DEBUG] Aur kuch? " << std::endl;
       			}	
       			//std::cout << "[DEBUG] nAHI, BATA NA, Aur kuch? " << lineNumber << std::endl;
			
		}
		*/			
	
		//TODO: Can simply do --->
		selected_tableIndex = std::to_string(HandHe+1);

		//std::cout << "Selected table number: " << selected_tableIndex << std::endl; 
		//std::cout << "Locating the selected table in the opacity file..." << selected_tableIndex << std::endl; 

		if(lineNumber > 240) {
		//if(lineNumber > 3850 && lineNumber < 3950) { //testing for table 48

			//std::string tableIndex;
			std::string opacity_xsec = "";
                        std::size_t posTableIndex = op_line.find("TABLE");

                        if (posTableIndex!=std::string::npos){ // 1. if at the first line of a table
                        	//std::cout << "DEBUG: at first line of a table " << line << std::endl;
                                tableIndex = op_line.substr(posTableIndex+7,3);
                                if(tableIndex == selected_tableIndex){ // 1.a) if at the start of a table we want to access
                                	//std::cout << "DEBUG: at first line of a table we want to access" << std::endl;
                                        //std::cout << "DEBUG: " << line << std::endl;
                                        //s << line << std::endl;
                                        //select[i].lineNumber = lineNumber; // storing the location of start of that table
					//std::cout << "[DEBUG 1] tableIndex: " << tableIndex << std::endl;
					//std::cout << "[DEBUG 2] selected_tableIndex: " << selected_tableIndex << std::endl;
					//std::cout << "[DEBUG 3] lineNumber " << lineNumber << std::endl;
                                        startTable_lineNumber = lineNumber; // storing the location of start of that table
                                        //std::cout << "[DEBUG 4] Selected table starts on line: " << startTable_lineNumber << std::endl;
                                	//std::cout << "[DEBUG 5] startTable_lineNumber+5 " << startTable_lineNumber+5 << std::endl;
                                	//std::cout << "[DEBUG 6] startTable_lineNumber+76 " << startTable_lineNumber+76 << std::endl;
                                                 //foundTable = true;
                                }
                                else { // 1.b) if at the start of a table we don't want to access
                                       //std::cout << "DEBUG: at the start of a table we don't want to access" << std::endl;
                                	continue;
                                }
			}
                        else { // 2. if inside a table
                                //std::cout << "[DEBUG] inside a table" << std::endl;
                                //std::cout << "[DEBUG] line number " << lineNumber << std::endl;
                                //std::cout << "[DEBUG] startTable_lineNumber " << startTable_lineNumber << std::endl;
                                //std::cout << "[DEBUG] line number " << lineNumber << std::endl;
				//std::cout << "[DEBUG 7] startTable_lineNumber+5 " << startTable_lineNumber+5 << std::endl;
                                //std::cout << "[DEBUG 8] startTable_lineNumber+76 " << startTable_lineNumber+76 << std::endl;
                        	//if(lineNumber > startTable_lineNumber+5 ) {std::cout << "[DEBUG 9] line number: " << lineNumber << " and lineNumber > startTable_lineNumber+5" << std::endl;}
                        	//if(lineNumber < startTable_lineNumber+76 ) {std::cout << "[DEBUG 10] lineNumber: " << lineNumber << " and lineNumber < startTable_lineNumber+76" << std::endl;}
                        	if(lineNumber > startTable_lineNumber+5 && lineNumber < startTable_lineNumber+76 ) { // 2.a) if inside a table we want to access
                                	//std::cout << "[INFO] Inside a table we want to access" << std::endl;
                                        //s << line << std::endl;
                                        //std::cout << "DEBUG: " << line << std::endl;
					//std::cout << " [INFO] lineNumber: " << lineNumber << std::endl;
                        		std::vector<std::string> row_in_OPtable;
					std::string word = "";
					while(iss_op_line >> word){
						row_in_OPtable.push_back(word);
					}
					//std::cout << " [DEBUG 11]" << std::endl;
					//iss_op_line >> row_in_OPtable[0] >> row_in_OPtable[1] >> row_in_OPtable[2] >> row_in_OPtable[3] >> row_in_OPtable[4] >> row_in_OPtable[5] >> row_in_OPtable[6] >> row_in_OPtable[7] >> row_in_OPtable[8] >> row_in_OPtable[9] >> row_in_OPtable[10] >> row_in_OPtable[11] >> row_in_OPtable[12] >> row_in_OPtable[13] >> row_in_OPtable[14] >> row_in_OPtable[15] >> row_in_OPtable[16] >> row_in_OPtable[17] >> row_in_OPtable[18] >> row_in_OPtable[19];   
					//std::cout << " [DEBUG 12]" << std::endl;
					std::string OPtable_temp = row_in_OPtable[0];
					//std::cout << " [DEBUG 13]" << std::endl;
					//if(row_in_OPtable[0] == logT[std::stoi(T)])
					//std::cout << "[DEBUG 14] OPtable_temp: " << OPtable_temp << std::endl;
					//if(std::stod(OPtable_temp) == logT[std::stoi(T)]) {
					if(std::stod(OPtable_temp) == logT[T]) {
						opacity_xsec = row_in_OPtable[R+1];
						std::cout << "[INFO] Found the best opacity xsec value for this row: " << opacity_xsec << std::endl;
						return std::stod(opacity_xsec);
					}
				}
                                else { // 2.b) if inside a table we don't want to access
                                       //std::cout << "DEBUG: inside a table we don't want to access" << std::endl;
                                }
			}			
		}
	}
}

/*TODO: 

Write separate functions to compute the following -->

1. number density of an element (from a given mass fraction)
2. absorption coefficient
3. Compton emission rate
4. Bremsstrahlung emission rate
5. F(w,y)
6. a general integral function
7. differential flux (as a function of energy and radial position inside the sun)

Maybe later separate this whole part into a separate file.

*/

//double SolarModel::ElectronNumberDensity(double zone_density, double H_massFrac){
///*
//	18.04.2019
//	Number density of electrons is computed using the following assumptions:
//	1. H_massFrac + He_massFrac \approx 1
//	2. They are fully ionised
//	TODO later: Check the validity of these assumptions
// 
//*/
//	return((zone_density/amu)*(1+H_massFrac)/2);
//	
//	
//}
//
//double SolarModel::NumberDensity(double Z_massFrac, double zone_density, double Z_atomicMass) { // called iteratively for each element in AbsorptionCoefficient()
//	return ((Z_massFrac * zone_density) / (Z_atomicMass * amu));
//}
//
////To call:
////row[i].abs_coeff = AbsorptionCoefficient(std::stod(row[i].temp), std::stod(row[i].density),row[i].X_Z_all, row[i].opacity_value);
//double SolarModel::AbsorptionCoefficient(double E, double T, double Rho, array XZ, double op_xsec){
//	double k = 0.0;
//	for(int j=0: j<28; j++){
//		//k += op_xsec * (1 - exp(E/T)) * NumberDensity(X_Z_all[j],std::stod(row[i].density),atomic_mass[j]);
//		k += op_xsec * (1 - exp(E/T)) * NumberDensity(X_Z_all[j],Rho,atomic_mass[j]);
//	}  
//	return(k);
//}
//
//double SolarModel::ComptonEmissionRate(double E, double T){
//	return((alpha * g_ae * g_ae * E * E * n_e) / (3 * m_e * m_e * (exp(E/T -1))));
//}
//
//double SolarModel::BremssEmissionRate(){
//	return(alpha * alpha * g_ae * g_ae * (4/3) * sqrt(M_PI) * n_e * n_e * exp(-E/T) * F(E/T,sqrt(2)*y) / (sqrt(T) * pow(m_e,3.5) * E) );
//}
//
//double SolarModel::F(){
//	//TODO: How to define a function of a variable in C++ (basically in order to perform an integration)
//	double t, x;
//	double t_func = t*t*t/(pow(t*t + y*y),2);
//	double t_up_limit = sqrt()
//	double x_func = x * exp(-x*x) * Integrate(t_func,)
//	return(Integrate(x_func,0,inf));
//} 
//
//double ReadSolarModel::Integrate(){
//
//}
//
//void ReadSolarModel::DifferentialAxionFlux(){
//
//}


/*
int ReadOpacityFile::ReadAndStoreTable() {

        for(int i = 0; i < input; i++){

        	int lineNumber = 0;
        	int posTableIndex = 0;

        	in_file.open(in_filename.c_str());
	
	        if(!in_file.good()){
	                std::cout << "Problem with file!" << std::endl;
	                return 1;
	        }

                std::stringstream s;
                std::ofstream out_file;
                std::string out_filename = "../resources/extracted_tables/" + select[i].H_massFrac + "-" + select[i].He_massFrac + "-" + in_filename.substr(32,60) + ".stored";
                out_file.open(out_filename);

                while(!in_file.eof()){
                        std::getline(in_file, line);
                        ++lineNumber;
                        if(lineNumber > 240 ){

                                std::string tableIndex;
                                std::size_t posTableIndex = line.find("TABLE");

                                if (posTableIndex!=std::string::npos){ // 1. if at the first line of a table
					//std::cout << "DEBUG: at first line of a table " << line << std::endl;
				        tableIndex = line.substr(posTableIndex+7,3);
                                        if(tableIndex == select[i].tableIndex){	// 1.a) if at the start of a table we want to access
                                                //std::cout << "DEBUG: at first line of a table we want to access" << std::endl;
						//std::cout << "DEBUG: " << line << std::endl;
						s << line << std::endl;
                                                select[i].lineNumber = lineNumber; // storing the location of start of that table
                                                std::cout << "Table number " << tableIndex << " starts on line " << lineNumber << std::endl;
                                                //foundTable = true;
                                        }
					else { // 1.b) if at the start of a table we don't want to access
						//std::cout << "DEBUG: at the start of a table we don't want to access" << std::endl;
						continue;
                                	}
				}
                                else { // 2. if inside a table
					//std::cout << "DEBUG: inside a table" << std::endl;
					if(lineNumber < select[i].lineNumber+76) { // 2.a) if inside a table we want to access
                                		//std::cout << "DEBUG: inside a table we want to access" << std::endl;
						s << line << std::endl;
						//std::cout << "DEBUG: " << line << std::endl;
                                	}
					else { // 2.b) if inside a table we don't want to access
						//std::cout << "DEBUG: inside a table we don't want to access" << std::endl;
					}
				}
                        }
                } 
                std::cout << "Finished parsing table" << std::endl;
                std::cout << "Storing the parsed table " << select[i].tableIndex << " in ouput file " << out_filename << std::endl;
                out_file << s.str() << std::endl;
                out_file.close();
                std::cout << "Output file closed" << std::endl;
                in_file.close();
                std::cout << "Input file closed" << std::endl;
        }

        std::cout << "Done!" << std::endl;





	
		++lineNumber;


	}
	opacity_file.close();
}       
*/
