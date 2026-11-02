// -----------------------------------------------------------------------------------------    
// data_time
//
// Description: 
//     The component stores the data related to the simulation time
//
// ----------------------------------------------------------------------------------------- 

#include "config/config_data.hpp"
#include "data_time.hpp"

using namespace Data;

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
DataTime::DataTime()
{
    return;
}


// -----------------------------------------------------------------------------------------    
// Initialize
//
// Description: This procedure is called each time a new simulation is started
// -----------------------------------------------------------------------------------------    
void DataTime::Initialize(const std::string dataDir)
{
    //Load Initial values from configuration data
    ConfigData *configData = ConfigData::GetInstance();

    struct tm configDate = configData->gameSetup->GetStartDate();
    this->currentDate    = std::mktime(&configDate);
    this->simulationTick = configData->gameSetup->GetSimulationTick();
    this->simulationPeriod = this->simulationTick; 

    return;
}

// -----------------------------------------------------------------------------------------    
// Load
//
// Description: This procedure loads the time data from a saved file
// -----------------------------------------------------------------------------------------    
DataResult DataTime::Load(const std::string file)
{
    return DataResult::DATA_OK;
}

// -----------------------------------------------------------------------------------------    
// Save Game
//
// Description: This procedure saves the data in the file
// -----------------------------------------------------------------------------------------    
DataResult DataTime::Save(const std::string file)
{
    return DataResult::DATA_OK;
}

// -----------------------------------------------------------------------------------------    
// Get Formated Time
//
// Description: This procedure returns the formated time
// -----------------------------------------------------------------------------------------  
std::string DataTime::GetFormatedDate(std::string format)
{
    char buffer[80];
    struct tm *date = std::localtime(&this->currentDate);
    strftime(buffer, sizeof(buffer), format.c_str(), date);
    return std::string(buffer);
}

// -----------------------------------------------------------------------------------------
// Calculate Date Increment
//
// Description: This function performs the period increment over the date
// -----------------------------------------------------------------------------------------
void DataTime::CalculateIncrement()
{
    this->currentDate = this->currentDate + this->simulationPeriod;
}