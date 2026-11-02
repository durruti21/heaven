// -----------------------------------------------------------------------------------------    
// config_data
//
// Description: 
//     The component that stores all the game configuration parameters. The parameters control the behaviour of the rest of components. 
//        The config component will read the configuration parameters from the file system configuration files and load them on initialization.
//        The config dataset will be split on two subsets:
//        - system config dataset -> composed of parameters always readed from file and not modifiable by user (only by MOD)
//        - user config dataset   -> composed of parameters modifiable by user
//
// -----------------------------------------------------------------------------------------    
#pragma once

#include <string>
#include <memory>

#include "config_types.hpp"
#include "config_setup.hpp"
#include "config_application.hpp"

class ConfigData {

private:
    static ConfigData* instance;    

    // -----------------------------------------------------------------------------------------    
    // Private Constructor get reference
    // ----------------------------------------------------------------------------------------- 
    ConfigData(const std::string & dirPath);    


public:
    // -----------------------------------------------------------------------------------------    
    // Create Class
    // ----------------------------------------------------------------------------------------- 
    static ConfigData * CreateInstance(const std::string & dirPath);

    // -----------------------------------------------------------------------------------------    
    // Get Instance reference
    // ----------------------------------------------------------------------------------------- 
    static ConfigData * GetInstance();


    // -----------------------------------------------------------------------------------------
    // Load Configuration
    //
    // Description: Loads the set of config files into memory so they are available for the game
    // -----------------------------------------------------------------------------------------
    Config::ConfigResult Load();

    // -----------------------------------------------------------------------------------------    
    // Save Configuration
    //
    // Description: Saves the set of user modifiable files to directory
    // -----------------------------------------------------------------------------------------    
    Config::ConfigResult Save();

    // Get Parameters
    // TBD

    // User Modifiable Set parameters functions
    // TBD

    // -----------------------------------------------------------------------------------------    
    // Object composition declaration
    std::unique_ptr<ConfigSetup> gameSetup;
    std::unique_ptr<ConfigApplication>  gameApp;



};






