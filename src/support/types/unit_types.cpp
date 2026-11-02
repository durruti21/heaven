
// unit_types
//
// Description: 
//        This file defines the units used on the simulation
#include <iomanip>

#include "unit_types.hpp"

// -----------------------------------------------------------------------------------------
// Format Range
//
// Description: this function formats the range
// NOTA: 
// -----------------------------------------------------------------------------------------
std::string UnitTypes::ToString(Distance range)
{
    
    Distance map_range; // in Km
    std::stringstream ss;
    std::string       units;

    if (range >= LIGHT_YEAR)
    {
       map_range = range/LIGHT_YEAR;
       units     = " ly";
    }
    else if (range >= 1.0e12)
    {
       map_range = range/1.0e12;
       units     = "b Km";
    }
    else if (range >= 1.0e9)
    {
       map_range = range/1.0e9;
       units     = "m Km";
    }
    else 
    {
       map_range = range/1.0e3;
       units     = " Km";
    }
    ss << std::fixed << std::setprecision(1) << map_range;
    return ss.str() + units;
}

// -----------------------------------------------------------------------------------------
// Format Size
//
// Description: this function formats the size of a system body
// NOTA: 
// -----------------------------------------------------------------------------------------
std::string UnitTypes::BodyMassToString(Mass size, bool inAU)
{
    std::stringstream ss;
    std::string       units;
    Mass converted;

    if (inAU)
    {
       converted = size/M_EARTH_CONST; // Relative to Earths Mass
       units = " M";
       if (converted < 0.0001)
       {
         ss << "< 0.0001";
       }
       else if (converted < 0.01)
       {
          ss << std::fixed << std::setprecision(4) << converted; 
       } 
       else 
       {
          ss << std::fixed << std::setprecision(2) << converted; 
       }
    }
    else {
       converted = size/1000; // Metric tons
       units = " tn";
       ss << std::fixed << std::setprecision(0) << converted;       
    }

    return ss.str() + units;    
}