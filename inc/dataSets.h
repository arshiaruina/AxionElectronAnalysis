#ifndef DATASETS_H
#define DATASETS_H 1
//c++
#include <iostream>
#include <cmath>
#include <string>
//definitions
#define DATASETS_SST2014_TIME 155221. //s
#define DATASETS_SST2015_TIME 738663. //s
#define DATASETS_SRT2014_TIME 151323. //s
#define DATASETS_SRT2015_TIME 785014. //s
#define DATASETS_DUTYCYCLE 0.9765
#define DATASETS_SECONDS_PER_FRAME 0.98 //s
#define DATASETS_FRAMES2014 2379029
#define DATASETS_FRAMES2015 17022741


// this namespace provides classes to deal with different datasets
// functions provide string descriptor, total time of dataset, background time, 
// tree names, background tree names, file paths and calibration file path
namespace dataSets
{
  enum class set{sst_2014_2015,srt_2014_2015,srt_1month,srt_2month,srt_3month,srt_2week,srt_4month,srt_1week};

inline std::string setString(set dataSet)
{
	switch(dataSet)
	{
	case set::srt_1week: return std::string("srt-1week"); break;
	case set::srt_2week: return std::string("srt-2week"); break;
	case set::srt_1month: return std::string("srt-1month"); break;
	case set::srt_2month: return std::string("srt-2month"); break;
	case set::srt_3month: return std::string("srt-3month"); break;
	case set::srt_4month: return std::string("srt-4month"); break;
	case set::sst_2014_2015: return std::string("sst20142015"); break;
	case set::srt_2014_2015: return std::string("srt20142015"); break;
	default: std::cerr << "Error: Unknown data set!" << std::endl; return std::string("");
	}
}

inline Double_t dataTime(set dataSet)
{
	switch(dataSet)
	{
	case set::srt_1week: return (7. * 5400.)*DATASETS_DUTYCYCLE; break;
	case set::srt_2week: return (14. * 5400.)*DATASETS_DUTYCYCLE; break;
	case set::srt_1month: return (30. * 5400.)*DATASETS_DUTYCYCLE; break;
	case set::srt_2month: return (60. * 5400.)*DATASETS_DUTYCYCLE; break;
	case set::srt_3month: return (90. * 5400.)*DATASETS_DUTYCYCLE; break;
	case set::srt_4month: return (120. * 5400.)*DATASETS_DUTYCYCLE; break;
	case set::sst_2014_2015: //return (DATASETS_SST2014_TIME + DATASETS_SST2015_TIME)*DATASETS_DUTYCYCLE; break;
	case set::srt_2014_2015: return (DATASETS_SRT2014_TIME + DATASETS_SRT2015_TIME)*DATASETS_DUTYCYCLE; break;
	default: std::cerr << "Error: Unknown data set!" << std::endl; return 0.;
	}
}

inline Double_t backgroundTime(set dataSet)
{
	switch(dataSet)
	{
	case set::srt_1week:
	case set::srt_2week:
	case set::srt_1month:
	case set::srt_2month:
	case set::srt_3month:
	case set::srt_4month:
	case set::sst_2014_2015:
	case set::srt_2014_2015: return ((DATASETS_FRAMES2014 + DATASETS_FRAMES2015)*DATASETS_SECONDS_PER_FRAME -(DATASETS_SST2014_TIME + DATASETS_SST2015_TIME + DATASETS_SRT2014_TIME + DATASETS_SRT2015_TIME)*DATASETS_DUTYCYCLE); break;
	default: std::cerr << "Error: Unknown data set!" << std::endl; return 0.;
	}
}

inline std::string dataTreeName(set dataSet)
{
	switch(dataSet)
	{
	case set::srt_1week:
	case set::srt_2week:
	case set::srt_1month:
	case set::srt_2month:
	case set::srt_3month:
	case set::srt_4month:
	case set::sst_2014_2015: return std::string("background-sc-sst-only"); break;
	case set::srt_2014_2015: return std::string("background-sc-srt-only"); break;
	default: std::cerr << "Error: Unknown data set!" << std::endl; return std::string("");
	}
}

inline std::string dataFilePath(set dataSet)
{
	switch(dataSet)
	{
	case set::srt_1week:
	case set::srt_2week:
	case set::srt_1month:
	case set::srt_2month:
	case set::srt_3month:
	case set::srt_4month:
	case set::sst_2014_2015:
	case set::srt_2014_2015: return std::string("./resources/background_splitted.2014+2015.root"); break;
	default: std::cerr << "Error: Unknown data set!" << std::endl; return std::string("");
	}
}

inline std::string backgroundTreeName(set dataSet)
{
	switch(dataSet)
	{
	case set::srt_1week:
	case set::srt_2week:
	case set::srt_1month:
	case set::srt_2month:
	case set::srt_3month:
	case set::srt_4month:
	case set::sst_2014_2015:
	case set::srt_2014_2015: return std::string("background-sc-not-only"); break;
	default: std::cerr << "Error: Unknown data set!" << std::endl; return std::string("");
	}
}

inline std::string backgroundFilePath(set dataSet)
{
	switch(dataSet)
	{
	case set::srt_1week:
	case set::srt_2week:
	case set::srt_1month:
	case set::srt_2month:
	case set::srt_3month:
	case set::srt_4month:
	case set::sst_2014_2015:
	case set::srt_2014_2015: return std::string("./resources/background_splitted.2014+2015.root"); break;
	default: std::cerr << "Error: Unknown data set!" << std::endl; return std::string("");
	}
}

inline std::string calibrationFilePath(set dataSet)
{
	switch(dataSet)
	{
	default: return "./resources/calibration-cdl-apr2014.root";
	}
}

}

#endif
