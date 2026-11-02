//----------------------------------------------------------------------------
// system_body_factory
//
// Description: 
//        This class handles the full list of system bodies in the simulation
//-----------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <sqlite3.h>

#include "system_body_factory.hpp"
#include "support/types/unit_types.hpp"
#include "support/logger.hpp"

//---------------------------------------------------------------------
// Generic Callback function to retrieve colums
// Move later to data_types.cpp to allow retrieval from diferent files
//--------------------------------------------------------------------
static int queryCallback(void *data, int argc, char** argv, char**azColName)
{
    Data::QueryResults *results = static_cast<QueryResults *>(data);

    //Store colum names for the first row
    if (results->columnNames.empty()) {
        for (int i=0; i< argc; i++)
            results->columnNames.push_back(azColName[i] ? azColName[i] : "");
    }

    // Store row values
    Row row;
    for (int i=0; i< argc; i++)
    {
        row.values.push_back(argv[i] ? argv[i] : "");
    }
    results->rows.push_back(row);

    return 0;
}

//------------------------------------------------------------
// Constructor
//------------------------------------------------------------
SystemBodyFactory::SystemBodyFactory()
{
    return;
}

//------------------------------------------------------------
// Load Initial Data
//
//  Description:
//     This function loads the initial data set corresponding
//     to the sol system.
//------------------------------------------------------------
DataResult SystemBodyFactory::CreateInitialData(std::string dataFile)
{
    // Sun is inserted on first position
    std::shared_ptr<SystemBody> star = std::make_shared<SystemBody>(nullptr);
    star->name       = "Sol";
    star->bodyType   = SystemBodyType::SB_STAR;
    star->bodyMass   = M_SUN_CONST;
    star->position   = {0.0, 0.0, 0.0};
    star->velocity   = {0.0, 0.0, 0.0};
    systemBodiesList.push_back(star);


    // Open SQL File to read rest of Solar System items
    sqlite3* db;
    char*    errorMsg = 0;
    int      result;
    Data::QueryResults data;

    
    result = sqlite3_open(dataFile.c_str(),&db);
    if (result)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_CRITICAL,"SystemBodyFactory::CreateInitialData failed to open " + dataFile);
        return DataResult::DATA_MISSING_FILE;
    }

    // Retrieve information
    std::string sqlSentence = "SELECT Name, PlanetNumber, OrbitNumber, BodyClass, Xcor, Ycor, Mass FROM DIM_SolSystemBodies ORDER BY PlanetNumber ASC, OrbitNumber ASC;";
    result = sqlite3_exec(db, sqlSentence.c_str(), queryCallback, &data, &errorMsg);
    if (result != SQLITE_OK)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_CRITICAL,"SystemBodyFactory::CreateInitialData failed to execute query to retrieve sol system data");
        sqlite3_free(errorMsg);
        sqlite3_close(db);
        return DataResult::DATA_CORRUPT_FILE;
    }

    // Go all the list around to create the proper planets
    // Due the querie is ordered by PlanetNumber and Orbit Number, all the moons shall be ordered after its parent planet, so we can
    // just gather the parent planet of a moon by looking at the last planet.
    std::shared_ptr<SystemBody> last_planet  = nullptr;
    std::shared_ptr<SystemBody> current_body = nullptr; 
    int rawPlanetType = 0;
    for (const Data::Row& record : data.rows)
    {
        // Create object with proper parent
        rawPlanetType = std::stoi(record.values[3]);
        if (rawPlanetType == 1) //Planet
        {
           current_body = std::make_shared<SystemBody>(star);
           last_planet  = current_body;    
        }
        else if ((rawPlanetType == 2) && (last_planet != nullptr)) //Moon
        {
            current_body = std::make_shared<SystemBody>(last_planet);
        }
        else { // do not process for the moment asteroids and comets
            current_body = nullptr;
        } 

        // Fill up values
        if (current_body != nullptr)
        {
            current_body->name = record.values[0]; 
            switch (rawPlanetType)
            {
               case 1 :
                   current_body->bodyType = SystemBodyType::SB_PLANET;
                   break;
               case 2 :
                   current_body->bodyType = SystemBodyType::SB_MOON;
                   break;
               case 3 :
                   current_body->bodyType = SystemBodyType::SB_ASTEROID;
                   break;
               case 5 :
                   current_body->bodyType = SystemBodyType::SB_COMET;
                   break;
               default :
                   Logger::GetInstance()->Log(Logger::LogLevel::LOG_CRITICAL,"SystemBodyFactory::CreateInitialData unknown system body type for " + record.values[0]);
                   current_body->bodyType = SystemBodyType::SB_COMET;
            }
            current_body->bodyMass = std::stod(record.values[6]) * M_EARTH_CONST; // cames relative to the mass of the earth
            current_body->position = {std::stod(record.values[4])*1000.0, std::stod(record.values[5])*1000.0, 0.0};
            if ((current_body->bodyType == SystemBodyType::SB_MOON) && (last_planet != nullptr))
            {
                // Moons position is relative to parent
                current_body->position = current_body->position - last_planet->position;
            }
            // calculate orbital velocity
            current_body->CalculateVelocityVector(); 

            // put on the list
            systemBodiesList.push_back(current_body);
        }
    }


    // Close Database
    sqlite3_close(db);    
    return DataResult::DATA_OK;
}

//------------------------------------------------------------
// Load Saved Data
//
//  Description:
//     This function loads the daved data for an ongoing sim.
//------------------------------------------------------------
DataResult SystemBodyFactory::Load(std::string dataFile)
{
    // TBD when Load/Save is implemented
    return DataResult::DATA_OK;
}

//------------------------------------------------------------
// Save Data
//
//  Description:
//     This function saves the data for an ongoing sim.
//------------------------------------------------------------
DataResult SystemBodyFactory::Save(std::string dataFile)
{
    // TBD when Load/Save is implemented
    return DataResult::DATA_OK;
}



//------------------------------------------------------------
// UpdatePositions
//
// Description:
//    This function updates all the system bodies positions
//    based on the provided increment of time
//------------------------------------------------------------
void SystemBodyFactory::UpdatePositions(Magnitude time)
{
    for (std::shared_ptr<SystemBody> body : this->systemBodiesList)
    {
        body->UpdateKinematicState(time);
        body->UpdateAbsolutePosition();
       // if (body->name == "Luna")
       // Logger::GetInstance()->Log(Logger::LogLevel::LOG_DEBUG,body->name + " position x={:.3} y={:.3} mod={:.3}",body->position.x, body->position.y, body->position.modulus());
    }

}