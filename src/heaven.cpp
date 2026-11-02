/*
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "support/logger.hpp"
#include "config/config_data.hpp"
#include "data_store/data_store.hpp"
#include "engine/sim_engine.hpp"
#include "gui/gui_main.hpp"

// Constant declaration
const std::string SW_VERSION        = "0.02";
const std::string DEFAULT_LOG_FILE  = "./log.txt";
const std::string DEFAUL_CONFIG_DIR = "./config";


/* Declaration of singleton objects */
ConfigData *ConfigData::instance  = nullptr;
Logger     *Logger::instance      = nullptr;
DataStore  *DataStore::instance   = nullptr;
SimEngine  *SimEngine::instance   = nullptr;
static GUIMain      *guiMain      = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    //Create Logger
    Logger *logger = Logger::CreateInstance(DEFAULT_LOG_FILE);

    // Create GUI
    guiMain    = new GUIMain();

    //Set initial message
    logger->Log(Logger::LogLevel::LOG_INFO,"HEAVEN version " + SW_VERSION);

    //Create Configuration
    Config::ConfigResult configResult;
    ConfigData *configData;
    configData = ConfigData::CreateInstance(DEFAUL_CONFIG_DIR);

 
    //Load Configuration Data
    configResult = configData->Load();
    if (configResult != Config::ConfigResult::CONFIG_OK)
    {
        //SDL_Log("Failed Config Data Load: Error %d", (int)configResult);
        logger->Log(Logger::LogLevel::LOG_CRITICAL,"Failed Config Data Load: Error {0:d}", (int)configResult); 
        return SDL_APP_FAILURE;
    }
    // Loaded configuration
    logger->Log(Logger::LogLevel::LOG_INFO,"Loaded configuration from " + DEFAUL_CONFIG_DIR);   

    // Set log level based on configuration file
    logger->SetLogLevel(Logger::ToLogLevel(configData->gameApp->GetLogLevel()));

    //Create Data Store
    DataStore  *dataStore = DataStore::CreateInstance
                                        (configData->gameApp->GetDataDirectory(), 
                                         configData->gameApp->GetSaveDirectory());


    // Create Sim Engine and Set Initial Configuration
    // This is the main simulation computation loop real time, not related with tick/period of simulation
    SimEngine *simEngine = SimEngine::GetInstance();
    simEngine->SetExecutionTime(1); 
    simEngine->SetExecutionMode(SimEngine::ExecutionModeType::EXEC_SIM_STEP);

    // DEBUG
    // Create a New placeholder game
    dataStore->CreateGame();
    // Make an initial increment to place everything in its place.
    simEngine->Step();
    // DEBUG
     
    // Init GUI
    return guiMain->Init("HEAVEN", SW_VERSION, "com.casals.heaven");

}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    return guiMain->ProcessEvent(appstate, event);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    return guiMain->Iterate(appstate);
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Terminage Game Engine
    SimEngine::GetInstance()->Stop();

    return guiMain->Quit(appstate, result);
}

