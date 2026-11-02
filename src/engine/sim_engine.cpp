// -----------------------------------------------------------------------------------------   
// sim_engine
// ----------------------------------------------------------------------------------------- 
#include "sim_engine.hpp"
#include <iostream>
#include "data_store/data_store.hpp"
#include "data_store/system_body.hpp"
#include "support/logger.hpp"

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
SimEngine::SimEngine()
{
    //default values
    this->modeSim  = ExecutionModeType::EXEC_SIM_CONTINUOUS;
    this->execTime = 1;

    //Start the loop
    this->stopFlag.store(false);
    this->execThread = new std::thread(SimEngine::ExecutionLoop,this);

    return;
}


// -----------------------------------------------------------------------------------------    
// Get Instance reference
// ----------------------------------------------------------------------------------------- 
SimEngine * SimEngine::GetInstance()
{
    if (instance == nullptr)
    {
        // Set default config directory to be safe
        instance = new SimEngine();
    }
    return instance;
}

// -----------------------------------------------------------------------------------------    
// Execute Increment
// ----------------------------------------------------------------------------------------- 
void SimEngine::ExecuteIncrement()
{
    DataStore *data = DataStore::GetInstance();
    // Increment time
    data->simTime->CalculateIncrement();
    // Update system body kinematics 
    unsigned int period = data->simTime->GetSimulationPeriod();
    data->systemRepository->UpdatePositions(period);

    //Loop log
    Logger::GetInstance()->Log(Logger::LogLevel::LOG_DEBUG,"Sim engine loop start");

}




// -----------------------------------------------------------------------------------------    
// Execute
//
// Description: Execute the increment stablished in the mode defined
// -----------------------------------------------------------------------------------------    
void SimEngine::Execute()
{
    this->modeSim  = ExecutionModeType::EXEC_SIM_CONTINUOUS;
    return;
}

// -----------------------------------------------------------------------------------------    
// Pause
//
// Description: Pauses the game engine continuous execution
// -----------------------------------------------------------------------------------------    
void SimEngine::Pause()
{
    this->modeSim  = ExecutionModeType::EXEC_SIM_STEP;
    return;
}


// -----------------------------------------------------------------------------------------    
// Step
//
// Description: Execute one increment on STEP mode.
// -----------------------------------------------------------------------------------------    
void SimEngine::Step()
{
    if (this->modeSim  == ExecutionModeType::EXEC_SIM_STEP)
    {
        this->ExecuteIncrement();
    }

}


// -----------------------------------------------------------------------------------------    
// Stop
//
// Description: Stops the game engine
// -----------------------------------------------------------------------------------------    
void SimEngine::Stop()
{
    this->stopFlag.store(true);
    return;
}

// -----------------------------------------------------------------------------------------    
// Sim Execution Loop
//
// Description: This function contains the main simulation execution Loop
// -----------------------------------------------------------------------------------------    
void SimEngine::ExecutionLoop(SimEngine *simulation)
{

    while (!simulation->stopFlag.load())
    {
        if (simulation->modeSim  == ExecutionModeType::EXEC_SIM_CONTINUOUS)
        {
            simulation->ExecuteIncrement();
        }
        // wait next cycle
        std::this_thread::sleep_for(std::chrono::seconds(simulation->execTime));

    }

}

