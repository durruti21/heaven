// -----------------------------------------------------------------------------------------    
// config_setup
//
// Description: 
//     The component that stores all the simulation initial parameters that are set at the
//     start of the game. 
//
// -----------------------------------------------------------------------------------------    
#ifndef CONFIGSETUP_HPP
#define CONFIGSETUP_HPP

#include <string>
#include <ctime>

#include "config_types.hpp"

class ConfigSetup {

public:

    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    ConfigSetup(const std::string & filePath);


    // -----------------------------------------------------------------------------------------
    // Load Configuration
    //
    // Description: Loads the set of config files into memory so they are available for the game
    // -----------------------------------------------------------------------------------------
    Config::ConfigResult Load();


    // Get Parameters

    // -----------------------------------------------------------------------------------------
    // Get Start Date
    //
    // Description: This function returns the Start Date loaded from config file
    // -----------------------------------------------------------------------------------------
    struct tm GetStartDate() {return this->startDate;};

    // -----------------------------------------------------------------------------------------
    // Get Simulation Tick
    //
    // Description: This function returns the Simulation Tick Defined
    // -----------------------------------------------------------------------------------------
    unsigned int GetSimulationTick() {return this->simulationTick;};


private:
    std::string setupFile;
    
    //start date and time
    struct tm startDate;
    
    // increments
    unsigned int simulationTick;

};

#endif //CONFIGDATA_HPP