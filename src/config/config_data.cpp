// -----------------------------------------------------------------------------------------    
// config_data
// -----------------------------------------------------------------------------------------    
#include "config_data.hpp"

using namespace Config;


// -----------------------------------------------------------------------------------------    
// Private Constructor get reference
// ----------------------------------------------------------------------------------------- 
ConfigData::ConfigData(const std::string & dirPath) {

    gameSetup = std::make_unique<ConfigSetup>(dirPath + "/setup.json"); 
    gameApp   = std::make_unique<ConfigApplication>(dirPath + "/application.json");
    return;
}


// -----------------------------------------------------------------------------------------    
// Create Class
// ----------------------------------------------------------------------------------------- 
ConfigData * ConfigData::CreateInstance(const std::string & dirPath)
{
    if (instance == nullptr)
    {
        instance = new ConfigData(dirPath);
    }
    return instance;
}

// -----------------------------------------------------------------------------------------    
// Get Instance reference
// ----------------------------------------------------------------------------------------- 
ConfigData * ConfigData::GetInstance()
{
    if (instance == nullptr)
    {
        // Set default config directory to be safe
        instance = new ConfigData("./config");
    }
    return instance;
}


// -----------------------------------------------------------------------------------------
// Load Configuration
//
// Description: Loads the set of config files into memory so they are available for the game
// -----------------------------------------------------------------------------------------
Config::ConfigResult ConfigData::Load() {

    Config::ConfigResult result;

    result = gameSetup->Load();

    if (result != ConfigResult::CONFIG_OK)
    {
        return result;
    }


    result = gameApp->Load();

    if (result != ConfigResult::CONFIG_OK)
    {
        return result;
    }


    return ConfigResult::CONFIG_OK;
}

// -----------------------------------------------------------------------------------------    
// Save Configuration
//
// Description: Saves the set of user modifiable files to directory
// -----------------------------------------------------------------------------------------
Config::ConfigResult ConfigData::Save() {
    return ConfigResult::CONFIG_OK;
}
