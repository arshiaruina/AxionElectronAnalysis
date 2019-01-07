#ifndef CHIPREGIONS_H
#define CHIPREGIONS_H 1
//c++
#include <cmath>
//root
#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
//definitions
#define CHIPREGIONS_CHIP_X_MIN 0.
#define CHIPREGIONS_CHIP_X_MAX 14.
#define CHIPREGIONS_CHIP_Y_MIN 0.
#define CHIPREGIONS_CHIP_Y_MAX 14.

#define CHIPREGIONS_CHIP_CENTER_X 7.
#define CHIPREGIONS_CHIP_CENTER_Y 7.

#define CHIPREGIONS_GOLD_X_MIN 4.5
#define CHIPREGIONS_GOLD_X_MAX 9.5
#define CHIPREGIONS_GOLD_Y_MIN 4.5
#define CHIPREGIONS_GOLD_Y_MAX 9.5
/* #define CHIPREGIONS_GOLD_X_MIN 6.5 */
/* #define CHIPREGIONS_GOLD_X_MAX 7.5 */
/* #define CHIPREGIONS_GOLD_Y_MIN 7 */
/* #define CHIPREGIONS_GOLD_Y_MAX 7.8 */


#define CHIPREGIONS_SILVER_RADIUS_MAX 4.5

#define CHIPREGIONS_BRONZE_RADIUS_MAX 5.5


// this namespace provides a class for the chip regions
// it calculates the area of each of the three regions gold, silver and bronze
// proveides functions to get area and string descriptor
namespace chipRegions
{

    enum class region{gold,silver,bronze,goldPlusSilver,goldPlusSilverPlusBronze, chip};

inline Double_t areaGold(){return (CHIPREGIONS_GOLD_X_MAX - CHIPREGIONS_GOLD_X_MIN) * (CHIPREGIONS_GOLD_Y_MAX - CHIPREGIONS_GOLD_Y_MIN);}

inline Double_t areaSilver(){return (TMath::Pi() * CHIPREGIONS_SILVER_RADIUS_MAX * CHIPREGIONS_SILVER_RADIUS_MAX) - areaGold();}

inline Double_t areaBronze(){return (TMath::Pi() * ( CHIPREGIONS_BRONZE_RADIUS_MAX * CHIPREGIONS_BRONZE_RADIUS_MAX - CHIPREGIONS_SILVER_RADIUS_MAX * CHIPREGIONS_SILVER_RADIUS_MAX));}

inline Double_t areaChip(){return (CHIPREGIONS_CHIP_X_MAX - CHIPREGIONS_CHIP_X_MIN) * (CHIPREGIONS_CHIP_Y_MAX - CHIPREGIONS_CHIP_Y_MIN);}

inline Double_t area(region chipRegion)
{
	switch(chipRegion)
	{
	    case region::gold: return areaGold(); break;
	    case region::silver: return areaSilver(); break;
	    case region::bronze: return areaBronze(); break;
	    case region::goldPlusSilver: return areaGold() + areaSilver(); break;
	    case region::goldPlusSilverPlusBronze: return areaGold() + areaSilver() + areaBronze(); break;
	    case region::chip: return areaChip(); break;
	default: std::cerr << "Error: Unknown chip region!" << std::endl; return 0.;
	}
}

inline std::string areaString(region chipRegion)
{
	switch(chipRegion)
	{
	    case region::gold: return std::string("gold"); break;
	    case region::silver: return std::string("silver"); break;
	    case region::bronze: return std::string("bronze"); break;
	    case region::goldPlusSilver: return std::string("gold+silver"); break;
	    case region::goldPlusSilverPlusBronze: return std::string("gold+silver+bronze"); break;
	    case region::chip: return std::string("whole_chip"); break;
	    default: std::cerr << "Error: Unknown chip region!" << std::endl; return std::string("");
	}
}

}

#endif
