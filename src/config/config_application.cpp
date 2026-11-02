// -----------------------------------------------------------------------------------------    
// config_data
// -----------------------------------------------------------------------------------------    
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <time.h>

#include "config_application.hpp"
#include "support/logger.hpp"



using namespace Config;
using json = nlohmann::json;


// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
ConfigApplication::ConfigApplication(const std::string & filePath) {

    this->appFile = filePath;

    // Init variables
    this->logLevel        = "info";
    this->dataDirectory   = "./data";
    this->saveDirectory   = "./save";
    this->systemBodiesRefImageDir = "./data/reference/images/sbodies";
    this->stellarInfoFile = "./data/reference/stellar_info.db";
    this->fontsFile       = "./data/fonts/typicons.ttf";
   return;
}


// -----------------------------------------------------------------------------------------
// Load Configuration
//
// Description: Loads the set of config files into memory so they are available for the game
// -----------------------------------------------------------------------------------------
Config::ConfigResult ConfigApplication::Load() {

    Logger *logger = Logger::GetInstance();

    std::ifstream file(this->appFile);
    json config;
    try {
        config = json::parse(file);
    }
    catch (...)
    {
        return ConfigResult::CONFIG_MISSING_FILE;
    }
    // Log Level
    if (config.count("log_level")) 
    {
        this->logLevel = config["log_level"];
    }
    else {
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigApplication::Load missing key 'log_level'. Using default value " + this->logLevel); 
    }

    // Data Directories
    if (config.count("data_directory")) 
    {
        this->dataDirectory = config["data_directory"];
    }
    else {
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigApplication::Load missing key 'data_directory'. Using default value " + this->dataDirectory); 
    }

    // Save Directories
    if (config.count("save_directory")) 
    {
        this->saveDirectory = config["save_directory"];
    }
    else {
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigApplication::Load missing key 'save_directory'. Using default value " + this->saveDirectory);  
    }
    

    // System Bodies Directory
    if (config.count("sbody_img_directory")) 
    {
        this->systemBodiesRefImageDir = config["sbody_img_directory"];
    }
    else {
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigApplication::Load missing key 'sbody_img_directory'. Using default value " + this->systemBodiesRefImageDir);  
    }


    // Stellar info file
    if (config.count("stellar_info_file")) 
    {
        this->stellarInfoFile = config["stellar_info_file"];
    }
    else {
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigApplication::Load missing key 'stellar_info_file'. Using default value " + this->stellarInfoFile);  
    }

    // Fonts file
    if (config.count("fonts_file")) 
    {
        this->fontsFile = config["fonts_file"];
    }
    else {
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigApplication::Load missing key 'fonts_file'. Using default value " + this->fontsFile);  
    }

    return ConfigResult::CONFIG_OK;
}

