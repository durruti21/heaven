//----------------------------------------------------------------------------
// system_body_factory
//
// Description: 
//        This class handles the full list of system bodies in the simulation
//-----------------------------------------------------------------------------

#include <vector>
#include <memory>

#include "system_body.hpp"
#include "data_types.hpp"

#pragma once

using namespace Data;

class SystemBodyFactory {

public:
    //------------------------------------------------------------
    // Constructor
    //------------------------------------------------------------
    SystemBodyFactory();

    //------------------------------------------------------------
    // Create Initial Data
    //
    //  Description:
    //     This function loads the initial data set corresponding
    //     to the sol system.
    //------------------------------------------------------------
    DataResult CreateInitialData(std::string dataFile);


    //------------------------------------------------------------
    // Load Data
    //
    //  Description:
    //     This function loads the saved data for an ongoing sim.
    //------------------------------------------------------------
    DataResult Load(std::string dataFile);

    //------------------------------------------------------------
    // Save Data
    //
    //  Description:
    //     This function saves the data for an ongoing sim.
    //------------------------------------------------------------
    DataResult Save(std::string dataFile);    

    //------------------------------------------------------------
    // UpdatePositions
    //
    // Description:
    //    This function updates all the system bodies positions
    //    based on the provided increment of time
    //------------------------------------------------------------
    void UpdatePositions(Magnitude time);

    //------------------------------------------------------------
    // Get System Bodies
    //
    // Description:
    //   Get the list of system bodies
    //------------------------------------------------------------
    const std::vector<std::shared_ptr<SystemBody>>& GetSystemBodies() 
      {return this->systemBodiesList;};


private:
    
    std::vector<std::shared_ptr<SystemBody>> systemBodiesList;

};