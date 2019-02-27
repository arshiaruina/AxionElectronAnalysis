#ifndef READFILE_H
#define READFILE_H

struct ROW {
	double massFrac = 0.0;	// in units of solar mass
	double radius = 0.0;	// in units of solar radius
	double temp = 0.0;	// in Kelvin
	double density = 0.0;	// in gm per cubic cm
	double pressure = 0.0;	// in dyne per squared cm
	double lumiFrac = 0.0;	// in units of the solar luminosity
	double H_massFrac = 0.0;
	double He4_massFrac = 0.0;
	double He3_massFrac = 0.0;
	double C12_massFrac = 0.0;
	double C13_massFrac = 0.0;
	double N14_massFrac = 0.0;
	double N15_massFrac = 0.0;
	double O16_massFrac = 0.0;
	double O17_massFrac = 0.0;
	double O18_massFrac = 0.0;
	double Ne_massFrac = 0.0;
	double Na_massFrac = 0.0;
	double Mg_massFrac = 0.0;
	double Al_massFrac = 0.0;
	double Si_massFrac = 0.0;
	double P_massFrac = 0.0;
	double S_massFrac = 0.0;
	double Cl_massFrac = 0.0;
	double Ar_massFrac = 0.0;
	double K_massFrac = 0.0;
	double Ca_massFrac = 0.0;
	double Sc_massFrac = 0.0;
	double Ti_massFrac = 0.0;
	double V_massFrac = 0.0;
	double Cr_massFrac = 0.0;
	double Mn_massFrac = 0.0;
	double Fe_massFrac = 0.0;
	double Co_massFrac = 0.0;
	double Ni_massFrac = 0.0;

	double He_massFrac = 0.0;
	double C_massFrac = 0.0;
	double N_massFrac = 0.0;
	double O_massFrac = 0.0;

	double total_metal_massFrac = 0.0;

	std::vector<double> X_Z;
	//double X_C  = 0.0;
	//double X_N  = 0.0;
	//double X_O  = 0.0;
	//double X_Ne = 0.0;
	//double X_Na = 0.0;
	//double X_Mg = 0.0;
	//double X_Al = 0.0;
	//double X_Si = 0.0;
	//double X_S  = 0.0;
	//double X_Ar = 0.0;
	//double X_Ca = 0.0;
	//double X_Cr = 0.0;
	//double X_Mn = 0.0;
	//double X_Fe = 0.0;
	//double X_Ni = 0.0;
	
	double opacity_value = 0.0;

};

struct DISVAL { 
	std::string value;
	double dist;
};


class SolarModel{
private:
	std::vector<ROW> row;
	//std::vector<OPACITYFILE_CONTENTS> opacityfile_contents;
	//std::vector<std::string> OpacityFiles;
	std::vector<std::vector<std::string>> XZ_VecForOpFile;
	std::vector<std::vector<double>> XZ_VecForOpFile_numeric;
    	std::string solarmodel_filename = "../resources/AGSS09_solar_model.dat";
    	std::string line;
	//std::string op_line;
    	std::ifstream solarmodel_file;
	//std::ifstream opacity_file;
        //std::istringstream iss_line;
	
	std::vector<double> H_massFrac_inOPfiles = {0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.3500,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.5000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.7000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9000,0.9500,0.9500,0.9500,0.9500,0.9500,0.9500,0.9500,0.9500,0.9500,0.9500,1.0000,0.9999,0.9997,0.9990,0.9980,0.9960,0.9900,0.9800,0.9700,0.9600,0.9400,0.9200};
	std::vector<double> He_massFrac_inOPfiles = {1.0000,0.9999,0.9997,0.9990,0.9980,0.9960,0.9900,0.9800,0.9700,0.9600,0.9400,0.9200,0.9000,0.9000,0.8999,0.8997,0.8990,0.8980,0.8960,0.8900,0.8800,0.8700,0.8600,0.8400,0.8200,0.8000,0.8000,0.7999,0.7997,0.7990,0.7980,0.7960,0.7900,0.7800,0.7700,0.7600,0.7400,0.7200,0.7000,0.6500,0.6499,0.6497,0.6490,0.6480,0.6460,0.6400,0.6300,0.6200,0.6100,0.5900,0.5700,0.5500,0.5000,0.4999,0.4997,0.4990,0.4980,0.4960,0.4900,0.4800,0.4700,0.4600,0.4400,0.4200,0.4000,0.3000,0.2999,0.2997,0.2990,0.2980,0.2960,0.2900,0.2800,0.2700,0.2600,0.2400,0.2200,0.2000,0.2000,0.1999,0.1997,0.1990,0.1980,0.1960,0.1900,0.1800,0.1700,0.1600,0.1400,0.1200,0.1000,0.1000,0.0999,0.0997,0.0990,0.0980,0.0960,0.0900,0.0800,0.0700,0.0600,0.0400,0.0200,0.0000,0.0500,0.0499,0.0497,0.0490,0.0480,0.0460,0.0400,0.0300,0.0200,0.0100,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000};
	std::vector<double> logR = {-8.0,-7.5,-7.0,-6.5,-6.0,-5.5,-5.0,-4.5,-4.0,-3.5,-3.0,-2.5,-2.0,-1.5,-1.0,-0.5,0.0,0.5,1.0};
	std::vector<double> logT = {3.75,3.80,3.85,3.90,3.95,4.00,4.05,4.10,4.15,4.20,4.25,4.30,4.35,4.40,4.45,4.50,4.55,4.60,4.65,4.70,4.75,4.80,4.85,4.90,4.95,5.00,5.05,5.10,5.15,5.20,5.25,5.30,5.35,5.40,5.45,5.50,5.55,5.60,5.65,5.70,5.75,5.80,5.85,5.90,5.95,6.00,6.10,6.20,6.30,6.40,6.50,6.60,6.70,6.80,6.90,7.00,7.10,7.20,7.30,7.40,7.50,7.60,7.70,7.80,7.90,8.00,8.10,8.30,8.50,8.70};
	
	//opacity files downloaded on 28 Jan 2019
	std::vector<std::string> OpacityFiles= {
		"../resources/opacity_tables/OP17.0.0-0.0-0.560994-0.376801-0.021178-0.03366-0.005376-0.001992-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.0-0.771903-0.137266-0.002442-0.043408-0.003528-0.041454-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.0-1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.8-0.19-0.0-0.0-0.0-0.0-0.0-0.0-0.01-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.39-0.105-0.0-0.0-0.0-0.0-0.0-0.0-0.005-0.0-0.0-0.0-0.5-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.78-0.21-0.0-0.0-0.0-0.0-0.0-0.0-0.01-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.156-0.042-0.0-0.0-0.0-0.0-0.0-0.0-0.002-0.0-0.0-0.0-0.8-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.234-0.063-0.0-0.0-0.0-0.0-0.0-0.0-0.003-0.0-0.0-0.0-0.7-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.312-0.084-0.0-0.0-0.0-0.0-0.0-0.0-0.004-0.0-0.0-0.0-0.6-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.468-0.126-0.0-0.0-0.0-0.0-0.0-0.0-0.006-0.0-0.0-0.0-0.4-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.546-0.147-0.0-0.0-0.0-0.0-0.0-0.0-0.007-0.0-0.0-0.0-0.3-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.624-0.168-0.0-0.0-0.0-0.0-0.0-0.0-0.008-0.0-0.0-0.0-0.2-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.702-0.189-0.0-0.0-0.0-0.0-0.0-0.0-0.009-0.0-0.0-0.0-0.1-0.0.stored",
		"../resources/opacity_tables/OP17.0.4-0.6-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.24566-0.064615-0.513263-0.083258-0.00148-0.026323-0.002043-0.024566-0.011229-0.002292-0.001587-3.29E-4-1.7E-4-0.02189-0.001294.stored",
		"../resources/opacity_tables/OP17.0.26076-0.0655-0.474506-0.08246-0.001804-0.032828-0.002608-0.03135-0.013946-0.002598-0.002006-5.02E-4-2.93E-4-0.027305-0.001535.stored",
		"../resources/opacity_tables/OP17.0.26097-0.065553-0.474887-0.082526-0.001805-0.032106-0.00261-0.031375-0.013951-0.002596-0.002004-5.0E-4-2.86E-4-0.027327-0.001502.stored",
		"../resources/opacity_tables/OP17.0.111111-0.111111-0.111111-0.0-0.0-0.0-0.0-0.0-0.111111-0.111111-0.111111-0.111111-0.111111-0.111111-0.0.stored",
		"../resources/opacity_tables/OP17.0.112295-0.112295-0.562811-0.100083-0.00178-0.031649-0.002573-0.030225-0.013496-0.00225-0.001976-4.8E-4-2.82E-4-0.026325-0.00148.stored",
		"../resources/opacity_tables/OP17.0.122821-0.030851-0.629902-0.112014-7.93E-4-0.035422-0.001146-0.033828-0.014901-0.002411-0.002136-2.63E-4-1.26E-4-0.011729-0.001657.stored",
		"../resources/opacity_tables/OP17.0.131566-0.033048-0.268623-0.047769-8.5E-4-0.015106-0.001228-0.014426-0.006442-0.001074-9.43E-4-2.29E-4-1.35E-4-0.409425-0.069138.stored",
		"../resources/opacity_tables/OP17.0.177251-0.044524-0.573574-0.101997-0.001144-0.032254-0.001654-0.030803-0.01364-0.002233-0.001971-3.36E-4-1.81E-4-0.016927-0.001509.stored",
		"../resources/opacity_tables/OP17.0.219419-0.05266-0.53209-0.031816-0.005485-0.04169-0.005485-0.054306-0.021942-0.005485-0.005485-0.005485-0.005485-0.00768-0.005485.stored",
		"../resources/opacity_tables/OP17.0.223952-0.056254-0.457252-0.081312-0.001446-0.025714-0.00209-0.024556-0.010965-0.001828-0.001606-3.9E-4-2.29E-4-0.021387-0.091018.stored",
		"../resources/opacity_tables/OP17.0.234391-0.058876-0.478566-0.085102-0.001514-0.026912-0.002187-0.0257-0.011476-0.001913-0.00168-4.09E-4-0.04763-0.022384-0.001258.stored",
		"../resources/opacity_tables/OP17.0.239487-0.06985-0.501299-0.089145-0.001586-0.028191-0.002291-0.026921-0.012021-0.002004-0.00176-4.28E-4-2.51E-4-0.023448-0.001318.stored",
		"../resources/opacity_tables/OP17.0.246047-0.061824-0.502363-0.089334-0.001589-0.02825-0.002296-0.026978-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored",
		"../resources/opacity_tables/OP17.0.246051-0.061805-0.502374-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored",
		"../resources/opacity_tables/OP17.0.246052-0.061805-0.502373-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored",
		"../resources/opacity_tables/OP17.0.246052-0.061805-0.502374-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored",
		"../resources/opacity_tables/OP17.0.246053-0.061805-0.502373-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored",
		"../resources/opacity_tables/OP17.0.246067-0.061804-0.502363-0.089334-0.001589-0.02825-0.002296-0.026978-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored",
		"../resources/opacity_tables/OP17.0.246071-0.061803-0.50236-0.089334-0.001589-0.02825-0.002296-0.026978-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023497-0.001321.stored",
		"../resources/opacity_tables/OP17.0.246377-0.061887-0.503038-0.089454-0.001591-0.028288-0.002299-0.027015-0.012063-0.002011-0.001766-4.3E-4-2.52E-4-0.023529-0.0.stored",
		"../resources/opacity_tables/OP17.0.250112-0.062825-0.455128-0.111721-0.001615-0.036994-0.002619-0.03007-0.012489-0.002628-0.002133-4.88E-4-2.5E-4-0.029386-0.001542.stored",
		"../resources/opacity_tables/OP17.0.251368-0.06314-0.513227-0.091266-0.001623-0.028861-0.002346-0.027562-0.012307-0.002051-0.001802-4.38E-4-2.57E-4-0.002401-0.00135.stored",
		"../resources/opacity_tables/OP17.0.251973-0.063292-0.514462-0.091486-0.001627-0.028931-0.002351-0.027628-0.012337-0.002056-0.001806-4.39E-4-2.58E-4-0.0-0.001353.stored",
		"../resources/opacity_tables/OP17.0.251973-0.063293-0.514463-0.091486-0.001627-0.02893-0.002352-0.027628-0.012336-0.002056-0.001806-4.39E-4-2.58E-4-0.0-0.001353.stored",
		"../resources/opacity_tables/OP17.0.257907-0.063309-0.480245-0.072688-0.001956-0.035601-0.002828-0.033999-0.015451-0.001769-0.002169-5.3E-4-3.1E-4-0.029612-0.001627.stored",
		"../resources/opacity_tables/OP17.0.258548-0.064944-0.47048-0.08176-0.001669-0.038242-0.002707-0.031084-0.01291-0.002717-0.002204-5.05E-4-2.59E-4-0.030377-0.001594.stored",
		"../resources/opacity_tables/OP17.0.258739-0.064992-0.470828-0.08182-0.00167-0.03827-0.002709-0.031107-0.012672-0.002415-0.002104-4.19E-4-2.59E-4-0.030399-0.001595.stored",
		"../resources/opacity_tables/OP17.0.260201-0.06844-0.473487-0.082283-0.00168-0.032757-0.002602-0.031283-0.012993-0.002734-0.002001-5.01E-4-2.6E-4-0.027246-0.001532.stored",
		"../resources/opacity_tables/OP17.0.263685-0.060359-0.490624-0.087247-0.001552-0.02759-0.002242-0.026348-0.011765-0.001961-0.001723-4.19E-4-2.46E-4-0.022948-0.00129.stored",
		"../resources/opacity_tables/OP17.0.268093-0.054738-0.465892-0.114363-0.001508-0.026809-0.002179-0.026199-0.011649-0.002168-0.001674-4.18E-4-2.39E-4-0.022818-0.001254.stored",
		"../resources/opacity_tables/OP17.0.277791-0.059203-0.481225-0.085575-0.001522-0.027062-0.002199-0.025843-0.01154-0.001923-0.00169-4.11E-4-2.41E-4-0.022509-0.001265.stored",
		"../resources/opacity_tables/OP17.0.280013-0.070336-0.433689-0.101667-0.001808-0.03215-0.002613-0.030703-0.013709-0.002285-0.002007-4.88E-4-2.87E-4-0.026741-0.001504.stored",
		"../resources/opacity_tables/OP17.0.289473-0.072712-0.414557-0.105101-0.001869-0.033236-0.002701-0.03174-0.014173-0.002362-0.002075-5.05E-4-2.96E-4-0.027645-0.001554.stored",
		"../resources/opacity_tables/OP17.0.293439-0.059913-0.434029-0.106541-0.00165-0.029344-0.002385-0.028676-0.012751-0.002373-0.001832-4.57E-4-2.62E-4-0.024976-0.001373.stored",
		"../resources/opacity_tables/OP17.0.293873-0.073818-0.405658-0.106699-0.001897-0.033741-0.002743-0.032223-0.014388-0.002398-0.002107-5.12E-4-3.01E-4-0.028065-0.001578.stored",
		"../resources/opacity_tables/OP17.0.302863-0.061837-0.447968-0.077848-0.001703-0.030286-0.002462-0.029597-0.01316-0.002449-0.001891-4.72E-4-2.7E-4-0.025778-0.001417.stored",
		"../resources/opacity_tables/OP17.0.314592-0.056186-0.456703-0.081214-0.001445-0.025683-0.002087-0.024526-0.010952-0.001825-0.001604-3.9E-4-2.29E-4-0.021362-0.001201.stored",
		"../resources/opacity_tables/OP17.0.325857-0.081852-0.419786-0.07465-0.002104-0.023606-0.003041-0.022544-0.010198-0.001747-0.001523-5.36E-4-3.33E-4-0.031119-0.001104.stored",
		"../resources/opacity_tables/OP17.0.333333-0.333333-0.333333-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.346052-0.061805-0.402374-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored",
		"../resources/opacity_tables/OP17.0.411715-0.103417-0.167328-0.149485-0.002659-0.047272-0.003842-0.045144-0.020158-0.00336-0.002952-7.18E-4-4.22E-4-0.039319-0.00221.stored",
		"../resources/opacity_tables/OP17.0.767923-0.019025-0.154639-0.027499-4.89E-4-0.008696-7.07E-4-0.008305-0.003708-6.18E-4-5.43E-4-1.32E-4-7.8E-5-0.007233-4.07E-4.stored",
		"../resources/opacity_tables/OP17.0.781582-0.017905-0.145537-0.025881-4.6E-4-0.008184-6.65E-4-0.007816-0.00349-5.82E-4-5.11E-4-1.24E-4-7.3E-5-0.006807-3.83E-4.stored",
		"../resources/opacity_tables/OP17.1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.04265-0.010713-0.870801-0.038897-2.75E-4-0.0123-3.8E-4-0.011479-0.006953-3.64E-4-7.58E-4-9.4E-5-3.2E-5-0.004073-2.29E-4.stored" 
		"../resources/opacity_tables/OP17.0.04921-0.812361-0.100475-0.017867-3.18E-4-0.00565-4.59E-4-0.005396-0.002409-4.02E-4-3.53E-4-8.6E-5-5.0E-5-0.0047-2.64E-4.stored",
		"../resources/opacity_tables/OP17.0.054393-0.013663-0.700716-0.124607-3.51E-4-0.039404-5.08E-4-0.037631-0.016486-0.002635-0.002342-1.71E-4-5.6E-5-0.005194-0.001843.stored",
		"../resources/opacity_tables/OP17.0.082614-0.020752-0.671511-0.119413-5.33E-4-0.037762-7.71E-4-0.036062-0.015832-0.002542-0.002257-2.09E-4-8.5E-5-0.00789-0.001766.stored",
		"../resources/opacity_tables/OP17.0.0-0.078-0.021-0.0-0.0-0.0-0.0-0.0-0.0-0.001-0.0-0.0-0.0-0.9-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-1.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-1.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-1.0-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.5-0.0-0.0-0.0-0.0-0.0-0.5-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.0-0.0-0.0-0.0-0.0-1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.0-0.0-1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored",
		"../resources/opacity_tables/OP17.0.0-0.0-0.560994-0.376801-0.021178-0.03366-0.005376-0.001992-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.0-0.771903-0.137266-0.002442-0.043408-0.003528-0.041454-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.0-1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.8-0.19-0.0-0.0-0.0-0.0-0.0-0.0-0.01-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.39-0.105-0.0-0.0-0.0-0.0-0.0-0.0-0.005-0.0-0.0-0.0-0.5-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.78-0.21-0.0-0.0-0.0-0.0-0.0-0.0-0.01-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.156-0.042-0.0-0.0-0.0-0.0-0.0-0.0-0.002-0.0-0.0-0.0-0.8-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.234-0.063-0.0-0.0-0.0-0.0-0.0-0.0-0.003-0.0-0.0-0.0-0.7-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.312-0.084-0.0-0.0-0.0-0.0-0.0-0.0-0.004-0.0-0.0-0.0-0.6-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.468-0.126-0.0-0.0-0.0-0.0-0.0-0.0-0.006-0.0-0.0-0.0-0.4-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.546-0.147-0.0-0.0-0.0-0.0-0.0-0.0-0.007-0.0-0.0-0.0-0.3-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.624-0.168-0.0-0.0-0.0-0.0-0.0-0.0-0.008-0.0-0.0-0.0-0.2-0.0.stored", 
		"../resources/opacity_tables/OP17.0.0-0.702-0.189-0.0-0.0-0.0-0.0-0.0-0.0-0.009-0.0-0.0-0.0-0.1-0.0.stored", 
		"../resources/opacity_tables/OP17.0.4-0.6-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.24566-0.064615-0.513263-0.083258-0.00148-0.026323-0.002043-0.024566-0.011229-0.002292-0.001587-3.29E-4-1.7E-4-0.02189-0.001294.stored", 
		"../resources/opacity_tables/OP17.0.26076-0.0655-0.474506-0.08246-0.001804-0.032828-0.002608-0.03135-0.013946-0.002598-0.002006-5.02E-4-2.93E-4-0.027305-0.001535.stored", 
		"../resources/opacity_tables/OP17.0.26097-0.065553-0.474887-0.082526-0.001805-0.032106-0.00261-0.031375-0.013951-0.002596-0.002004-5.0E-4-2.86E-4-0.027327-0.001502.stored", 
		"../resources/opacity_tables/OP17.0.111111-0.111111-0.111111-0.0-0.0-0.0-0.0-0.0-0.111111-0.111111-0.111111-0.111111-0.111111-0.111111-0.0.stored", 
		"../resources/opacity_tables/OP17.0.112295-0.112295-0.562811-0.100083-0.00178-0.031649-0.002573-0.030225-0.013496-0.00225-0.001976-4.8E-4-2.82E-4-0.026325-0.00148.stored", 
		"../resources/opacity_tables/OP17.0.122821-0.030851-0.629902-0.112014-7.93E-4-0.035422-0.001146-0.033828-0.014901-0.002411-0.002136-2.63E-4-1.26E-4-0.011729-0.001657.stored", 
		"../resources/opacity_tables/OP17.0.131566-0.033048-0.268623-0.047769-8.5E-4-0.015106-0.001228-0.014426-0.006442-0.001074-9.43E-4-2.29E-4-1.35E-4-0.409425-0.069138.stored", 
		"../resources/opacity_tables/OP17.0.177251-0.044524-0.573574-0.101997-0.001144-0.032254-0.001654-0.030803-0.01364-0.002233-0.001971-3.36E-4-1.81E-4-0.016927-0.001509.stored", 
		"../resources/opacity_tables/OP17.0.219419-0.05266-0.53209-0.031816-0.005485-0.04169-0.005485-0.054306-0.021942-0.005485-0.005485-0.005485-0.005485-0.00768-0.005485.stored", 
		"../resources/opacity_tables/OP17.0.223952-0.056254-0.457252-0.081312-0.001446-0.025714-0.00209-0.024556-0.010965-0.001828-0.001606-3.9E-4-2.29E-4-0.021387-0.091018.stored", 
		"../resources/opacity_tables/OP17.0.234391-0.058876-0.478566-0.085102-0.001514-0.026912-0.002187-0.0257-0.011476-0.001913-0.00168-4.09E-4-0.04763-0.022384-0.001258.stored", 
		"../resources/opacity_tables/OP17.0.239487-0.06985-0.501299-0.089145-0.001586-0.028191-0.002291-0.026921-0.012021-0.002004-0.00176-4.28E-4-2.51E-4-0.023448-0.001318.stored", 
		"../resources/opacity_tables/OP17.0.246047-0.061824-0.502363-0.089334-0.001589-0.02825-0.002296-0.026978-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.246051-0.061805-0.502374-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.246052-0.061805-0.502373-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.246052-0.061805-0.502374-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.246053-0.061805-0.502373-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.246067-0.061804-0.502363-0.089334-0.001589-0.02825-0.002296-0.026978-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.246071-0.061803-0.50236-0.089334-0.001589-0.02825-0.002296-0.026978-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023497-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.246377-0.061887-0.503038-0.089454-0.001591-0.028288-0.002299-0.027015-0.012063-0.002011-0.001766-4.3E-4-2.52E-4-0.023529-0.0.stored", 
		"../resources/opacity_tables/OP17.0.250112-0.062825-0.455128-0.111721-0.001615-0.036994-0.002619-0.03007-0.012489-0.002628-0.002133-4.88E-4-2.5E-4-0.029386-0.001542.stored", 
		"../resources/opacity_tables/OP17.0.251368-0.06314-0.513227-0.091266-0.001623-0.028861-0.002346-0.027562-0.012307-0.002051-0.001802-4.38E-4-2.57E-4-0.002401-0.00135.stored", 
		"../resources/opacity_tables/OP17.0.251973-0.063292-0.514462-0.091486-0.001627-0.028931-0.002351-0.027628-0.012337-0.002056-0.001806-4.39E-4-2.58E-4-0.0-0.001353.stored", 
		"../resources/opacity_tables/OP17.0.251973-0.063293-0.514463-0.091486-0.001627-0.02893-0.002352-0.027628-0.012336-0.002056-0.001806-4.39E-4-2.58E-4-0.0-0.001353.stored", 
		"../resources/opacity_tables/OP17.0.257907-0.063309-0.480245-0.072688-0.001956-0.035601-0.002828-0.033999-0.015451-0.001769-0.002169-5.3E-4-3.1E-4-0.029612-0.001627.stored", 
		"../resources/opacity_tables/OP17.0.258548-0.064944-0.47048-0.08176-0.001669-0.038242-0.002707-0.031084-0.01291-0.002717-0.002204-5.05E-4-2.59E-4-0.030377-0.001594.stored", 
		"../resources/opacity_tables/OP17.0.258739-0.064992-0.470828-0.08182-0.00167-0.03827-0.002709-0.031107-0.012672-0.002415-0.002104-4.19E-4-2.59E-4-0.030399-0.001595.stored", 
		"../resources/opacity_tables/OP17.0.260201-0.06844-0.473487-0.082283-0.00168-0.032757-0.002602-0.031283-0.012993-0.002734-0.002001-5.01E-4-2.6E-4-0.027246-0.001532.stored", 
		"../resources/opacity_tables/OP17.0.263685-0.060359-0.490624-0.087247-0.001552-0.02759-0.002242-0.026348-0.011765-0.001961-0.001723-4.19E-4-2.46E-4-0.022948-0.00129.stored", 
		"../resources/opacity_tables/OP17.0.268093-0.054738-0.465892-0.114363-0.001508-0.026809-0.002179-0.026199-0.011649-0.002168-0.001674-4.18E-4-2.39E-4-0.022818-0.001254.stored", 
		"../resources/opacity_tables/OP17.0.277791-0.059203-0.481225-0.085575-0.001522-0.027062-0.002199-0.025843-0.01154-0.001923-0.00169-4.11E-4-2.41E-4-0.022509-0.001265.stored", 
		"../resources/opacity_tables/OP17.0.280013-0.070336-0.433689-0.101667-0.001808-0.03215-0.002613-0.030703-0.013709-0.002285-0.002007-4.88E-4-2.87E-4-0.026741-0.001504.stored", 
		"../resources/opacity_tables/OP17.0.289473-0.072712-0.414557-0.105101-0.001869-0.033236-0.002701-0.03174-0.014173-0.002362-0.002075-5.05E-4-2.96E-4-0.027645-0.001554.stored", 
		"../resources/opacity_tables/OP17.0.293439-0.059913-0.434029-0.106541-0.00165-0.029344-0.002385-0.028676-0.012751-0.002373-0.001832-4.57E-4-2.62E-4-0.024976-0.001373.stored", 
		"../resources/opacity_tables/OP17.0.293873-0.073818-0.405658-0.106699-0.001897-0.033741-0.002743-0.032223-0.014388-0.002398-0.002107-5.12E-4-3.01E-4-0.028065-0.001578.stored", 
		"../resources/opacity_tables/OP17.0.302863-0.061837-0.447968-0.077848-0.001703-0.030286-0.002462-0.029597-0.01316-0.002449-0.001891-4.72E-4-2.7E-4-0.025778-0.001417.stored", 
		"../resources/opacity_tables/OP17.0.314592-0.056186-0.456703-0.081214-0.001445-0.025683-0.002087-0.024526-0.010952-0.001825-0.001604-3.9E-4-2.29E-4-0.021362-0.001201.stored", 
		"../resources/opacity_tables/OP17.0.325857-0.081852-0.419786-0.07465-0.002104-0.023606-0.003041-0.022544-0.010198-0.001747-0.001523-5.36E-4-3.33E-4-0.031119-0.001104.stored", 
		"../resources/opacity_tables/OP17.0.333333-0.333333-0.333333-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored", 
		"../resources/opacity_tables/OP17.0.346052-0.061805-0.402374-0.089336-0.001589-0.028251-0.002296-0.026979-0.012047-0.002008-0.001764-4.29E-4-2.52E-4-0.023498-0.001321.stored", 
		"../resources/opacity_tables/OP17.0.411715-0.103417-0.167328-0.149485-0.002659-0.047272-0.003842-0.045144-0.020158-0.00336-0.002952-7.18E-4-4.22E-4-0.039319-0.00221.stored", 
		"../resources/opacity_tables/OP17.0.767923-0.019025-0.154639-0.027499-4.89E-4-0.008696-7.07E-4-0.008305-0.003708-6.18E-4-5.43E-4-1.32E-4-7.8E-5-0.007233-4.07E-4.stored", 
		"../resources/opacity_tables/OP17.0.781582-0.017905-0.145537-0.025881-4.6E-4-0.008184-6.65E-4-0.007816-0.00349-5.82E-4-5.11E-4-1.24E-4-7.3E-5-0.006807-3.83E-4.stored", 
		"../resources/opacity_tables/OP17.1.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0-0.0.stored" 
 
	};
public:
    	int ReadAndStoreTable();
        int MetalMassFraction();
	void ReadOpacityFileName();
	int GetTableIndex();
	int AccessSolarModel();
	double AccessOpacityFile(std::string s, std::string H, std::string He, std::string R, std::string T); 
};

#endif // READFILE_H
