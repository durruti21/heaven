// -----------------------------------------------------------------------------------------    
// data_time
//
// Description: 
//     The component stores the data related to the simulation time
//
// ----------------------------------------------------------------------------------------- 


#ifndef DATATIME_HPP
#define DATATIME_HPP

#include <string>
#include <ctime>

#include "data_types.hpp"

using namespace Data;

class DataTime {

public:


    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    DataTime();


    // -----------------------------------------------------------------------------------------    
    // Initialize
    //
    // Description: This procedure is called each time a new simulation is started
    // -----------------------------------------------------------------------------------------    
    void Initialize(const std::string dataDir);

    // -----------------------------------------------------------------------------------------    
    // Load
    //
    // Description: This procedure loads the time data from a saved file
    // -----------------------------------------------------------------------------------------    
    DataResult Load(const std::string file);

    // -----------------------------------------------------------------------------------------    
    // Save Game
    //
    // Description: This procedure saves the data in the file
    // -----------------------------------------------------------------------------------------    
    DataResult Save(const std::string file);
   
    // -----------------------------------------------------------------------------------------
    // Calculate Date Increment
    //
    // Description: This function performs the period increment over the date
    // -----------------------------------------------------------------------------------------
    void CalculateIncrement();

    // -----------------------------------------------------------------------------------------
    // Get/Set Current Date
    //
    // Description: Access functions for the Current Date of the simulation
    // -----------------------------------------------------------------------------------------
    std::time_t GetCurrentDate() {return this->currentDate;};
    void      SetCurrentDate(std::time_t date) {this->currentDate = date;};
    std::string GetFormatedDate(std::string format);

    // -----------------------------------------------------------------------------------------
    // Get/Set Simulation Tick
    //
    // Description: This function returns the Simulation Tick Defined
    // -----------------------------------------------------------------------------------------
    unsigned int GetSimulationTick() {return this->simulationTick;};
    void         SetSimulationTick(unsigned int tick) {this->simulationTick= tick;};


    // -----------------------------------------------------------------------------------------
    // Get/Set Simulation Period
    //
    // Description: This function returns the Simulation Period Defined
    // -----------------------------------------------------------------------------------------
    unsigned int GetSimulationPeriod() {return this->simulationPeriod;};
    void         SetSimulationPeriod(unsigned int time) {this->simulationPeriod= time;};    

private:
    //start date and time
    std::time_t currentDate;
    
    // increments
    unsigned int simulationTick;
    unsigned int simulationPeriod;
};

#endif //DATATIME_HPP