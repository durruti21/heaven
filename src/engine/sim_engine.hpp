// Engine
//
// Description: 
// 
//    The game engine is in charge of running the game simulation, by inspecting the data provided by data_store and updating it.
//    The game engine uses the system config dataset with the rules and parameters for executing the simulation.
//    The game engine is driven by the game graphical user interface. The engine will receive the actions/decisions the player
//    will take in the game run and will update acordingly the data store state. 
//
#pragma once

#include <thread>
#include <atomic>


class SimEngine {

private:
    static SimEngine* instance;

    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    SimEngine();

    
    // -----------------------------------------------------------------------------------------    
    // Execute Increment
    // ----------------------------------------------------------------------------------------- 
    void ExecuteIncrement();

public:

    enum class ExecutionModeType {
             EXEC_SIM_STEP, 
             EXEC_SIM_CONTINUOUS
    };


    // -----------------------------------------------------------------------------------------    
    // Get Instance reference
    // ----------------------------------------------------------------------------------------- 
    static SimEngine * GetInstance();


    // -----------------------------------------------------------------------------------------    
    // Execute
    //
    // Description: Set the machine on Continuous execution
    // -----------------------------------------------------------------------------------------    
    void Execute();


    // -----------------------------------------------------------------------------------------    
    // Step
    //
    // Description: Execute one increment on STEP mode.
    // -----------------------------------------------------------------------------------------    
    void Step();

    // -----------------------------------------------------------------------------------------    
    // Pause
    //
    // Description: Pause the game engine when executing in continuous mode
    // -----------------------------------------------------------------------------------------    
    void Pause();

    // -----------------------------------------------------------------------------------------    
    // Stop
    //
    // Description: Stops the game engine
    // -----------------------------------------------------------------------------------------    
    void Stop();

    // -----------------------------------------------------------------------------------------
    // Set Exection Time
    //
    // Description: Defines the execution time in ticks for next increments in seconds
    // -----------------------------------------------------------------------------------------
    void SetExecutionTime(unsigned int seconds) {this->execTime = seconds;};

    // -----------------------------------------------------------------------------------------    
    // Set/Get Execution Mode
    //
    // Description: Set The Mode Of Execution
    // -----------------------------------------------------------------------------------------    
    void SetExecutionMode(ExecutionModeType mode) {this->modeSim = mode;};
    ExecutionModeType GetExecutionMode() {return this->modeSim;};

    

private:

    // -----------------------------------------------------------------------------------------    
    // Sim Execution Loop
    //
    // Description: This function contains the main simulation execution Loop
    // -----------------------------------------------------------------------------------------    
    static void ExecutionLoop(SimEngine *simulation);


    ExecutionModeType modeSim;
    unsigned int      simTick;
    unsigned int      execTime;
    bool              execute;

    std::thread       *execThread;
    std::atomic<bool> stopFlag;

};
