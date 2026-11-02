// -----------------------------------------------------------------------------------------    
// config_data
// -----------------------------------------------------------------------------------------    
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <time.h>

#include "config_setup.hpp"
#include "support/logger.hpp"

#define START_DATE "2030-01-01 00:00:00"


using namespace Config;
using json = nlohmann::json;


// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
ConfigSetup::ConfigSetup(const std::string & filePath) {

    this->setupFile = filePath;

    // Init variables
    strptime(START_DATE, "%Y-%m-%d %H:%M:%S", &this->startDate);
    this->simulationTick = 1;
    return;
}


// -----------------------------------------------------------------------------------------
// Load Configuration
//
// Description: Loads the set of config files into memory so they are available for the game
// -----------------------------------------------------------------------------------------
Config::ConfigResult ConfigSetup::Load() 
{
    Logger *logger = Logger::GetInstance();

    std::ifstream file(this->setupFile);
    json config;
    try {
      config = json::parse(file);
    }
    catch (...)
    {
        return ConfigResult::CONFIG_MISSING_FILE;
    }
    // start date
    std::string startDateStr = config["start_date"]["date"];
    std::string startTimeStr = config["start_date"]["time"];

    const std::string fullDateStr = startDateStr + " " + startTimeStr;
    if (strptime(fullDateStr.c_str(), "%Y-%m-%d %H:%M:%S", &this->startDate) == NULL) 
    {
       strptime(START_DATE, "%Y-%m-%d %H:%M:%S", &this->startDate);
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigSetup::Invalid Start Date on . Using default value " + std::string(START_DATE)); 
    }

    // increments
    if (config["increments"]["tick"].is_number_unsigned())
    {
        this->simulationTick = config["increments"]["tick"].get<unsigned int>();
    }
    else
    {
       logger->Log(Logger::LogLevel::LOG_WARNING,"ConfigSetup::Invalid increment/ticks key . Using default value {0:d}", this->simulationTick);
    }


    return ConfigResult::CONFIG_OK;
}

