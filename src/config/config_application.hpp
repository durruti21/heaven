// -----------------------------------------------------------------------------------------    
// config_application
//
// Description: 
//     The component that stores all the application parameters that control the structure
//     and user personalizations of the application. 
//
// -----------------------------------------------------------------------------------------    
#pragma once

#include <string>

#include "config_types.hpp"

class ConfigApplication {

public:

    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    ConfigApplication(const std::string & filePath);


    // -----------------------------------------------------------------------------------------
    // Load Configuration
    //
    // Description: Loads the set of config files into memory so they are available for the game
    // -----------------------------------------------------------------------------------------
    Config::ConfigResult Load();


    // Get Parameters
    // -----------------------------------------------------------------------------------------
    // Get Log Level
    //
    // Description: This function returns the Log Level from config file
    // -----------------------------------------------------------------------------------------
    const std::string GetLogLevel() {return this->logLevel;};

    // -----------------------------------------------------------------------------------------
    // Get Data Directory
    //
    // Description: This function returns the Data Directory from config file
    // -----------------------------------------------------------------------------------------
    const std::string GetDataDirectory() {return this->dataDirectory;};

    // -----------------------------------------------------------------------------------------
    // Get Save Directory
    //
    // Description: This function returns the Save Directory from the config file
    // -----------------------------------------------------------------------------------------
    const std::string GetSaveDirectory() {return this->saveDirectory;};

    // -----------------------------------------------------------------------------------------
    // Get SystemBody Reference Image Directories
    //
    // Description: This function returns the SBodies Reference Image Directory
    // -----------------------------------------------------------------------------------------
    const std::string GetSBRefImageDirectory() {return this->systemBodiesRefImageDir;};

    // -----------------------------------------------------------------------------------------
    // Get Stellar Info File
    //
    // Description: This function returns the Data Directory from config file
    // -----------------------------------------------------------------------------------------
    const std::string GetStellarInfoFile() {return this->stellarInfoFile;};    

    // -----------------------------------------------------------------------------------------
    // Get Fonts File
    //
    // Description: This function returns the font file 
    // -----------------------------------------------------------------------------------------
    const std::string GetFontsFile() {return this->fontsFile;};        

private:
    std::string appFile;

    // Log level
    std::string logLevel;
    // Data Directory
    std::string dataDirectory;
    // Save Directory
    std::string saveDirectory;
    // SBodies Reference Images Directory
    std::string systemBodiesRefImageDir;
    // Stellar Info File
    std::string stellarInfoFile;
    // Fonts File
    std::string fontsFile;

};
