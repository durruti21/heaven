// DataStore
//
// Description: 
// 
//      The component that stores all the information realted to a specific game run. The component provides structured access to all the
//      information related to a specific game and also provides functions to save or load games into/from the hard disk.
//      This information is provided to the active components of the application (game engine and game gui).
//
#pragma once

#include <string>
#include <memory>

#include "data_types.hpp"
#include "data_time.hpp"
#include "system_body_factory.hpp"

class DataStore {

private:
    static DataStore* instance;    

    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    DataStore(const std::string dataDir, const std::string saveDir); 


public:
    // -----------------------------------------------------------------------------------------    
    // Create Class
    // ----------------------------------------------------------------------------------------- 
    static DataStore * CreateInstance(const std::string dataDir, const std::string saveDir);

    // -----------------------------------------------------------------------------------------    
    // Get Instance reference
    // ----------------------------------------------------------------------------------------- 
    static DataStore * GetInstance();

    // -----------------------------------------------------------------------------------------    
    // Create Game
    //
    // Description: This procedure creates a new game
    // -----------------------------------------------------------------------------------------    
    void CreateGame();

    // -----------------------------------------------------------------------------------------    
    // Load Game
    //
    // Description: This procedure loads a Game from hard disk
    // -----------------------------------------------------------------------------------------    
    Data::DataResult LoadGame(const std::string fileName);

    // -----------------------------------------------------------------------------------------    
    // Save Game
    //
    // Description: This procedure saves a game in the hard disk
    // -----------------------------------------------------------------------------------------    
    Data::DataResult SaveGame(const std::string fileName);

    // Aggregate components
    std::unique_ptr<DataTime>          simTime;
    std::unique_ptr<SystemBodyFactory> systemRepository;

private:
    std::string dataDirectory;
    std::string saveDirectory;

};
