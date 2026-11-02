// -----------------------------------------------------------------------------------------   
// data_store
// ----------------------------------------------------------------------------------------- 
#include "data_store.hpp"
#include "config/config_data.hpp"
#include "support/logger.hpp"

using namespace Data;

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
DataStore::DataStore(const std::string dataDir, const std::string saveDir)
{
    this->dataDirectory = dataDir;
    this->saveDirectory = saveDir;
    this->simTime = std::make_unique<DataTime>();
    this->systemRepository = std::make_unique<SystemBodyFactory>();
    return;
}


// -----------------------------------------------------------------------------------------    
// Create Class
// ----------------------------------------------------------------------------------------- 
DataStore * DataStore::CreateInstance(const std::string dataDir, const std::string saveDir)
{
    if (instance == nullptr)
    {
        instance = new DataStore(dataDir, saveDir);
    }
    return instance;
}

// -----------------------------------------------------------------------------------------    
// Get Instance reference
// ----------------------------------------------------------------------------------------- 
DataStore * DataStore::GetInstance()
{
    if (instance == nullptr)
    {
        // Set default config directory to be safe
        instance = new DataStore("./data", "./save");
    }
    return instance;
}


// -----------------------------------------------------------------------------------------    
// Create Game
//
// Description: This procedure creates a new game
// -----------------------------------------------------------------------------------------    
void DataStore::CreateGame()
{
    ConfigData *configData = ConfigData::GetInstance();

    this->simTime->Initialize(this->dataDirectory);
    this->systemRepository->CreateInitialData(configData->gameApp->GetStellarInfoFile());
    
    return;
}

// -----------------------------------------------------------------------------------------    
// Load Game
//
// Description: This procedure loads a Game from hard disk
// -----------------------------------------------------------------------------------------    
Data::DataResult  DataStore::LoadGame(const std::string fileName)
{
    DataResult result;

    result = this->simTime->Load(this->saveDirectory + fileName);
    if (result != DataResult::DATA_OK)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_WARNING,"DataStore::LoadGame error loading time from  " + fileName);
        return result;
    }

    result = this->systemRepository->Load(this->saveDirectory + fileName);
    if (result != DataResult::DATA_OK)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_WARNING,"DataStore::LoadGame error loading System Data from " + fileName);
        return result;
    }

    return DataResult::DATA_OK;
}

// -----------------------------------------------------------------------------------------    
// Save Game
//
// Description: This procedure saves a game in the hard disk
// -----------------------------------------------------------------------------------------    
Data::DataResult  DataStore::SaveGame(const std::string fileName)
{
     DataResult result;

    result = this->simTime->Save(this->saveDirectory + fileName);
    if (result != DataResult::DATA_OK)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_WARNING,"DataStore::SaveGame error storing time on " + fileName);
        return result;
    }

    result = this->systemRepository->Save(this->saveDirectory + fileName);
    if (result != DataResult::DATA_OK)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_WARNING,"DataStore::SaveGame error storing system info on " + fileName);
        return result;
    }    

    return DataResult::DATA_OK;
}


