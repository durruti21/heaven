// gui_main
//
// Description: 
//        The graphical user interface is the main interface with the user, by providing the information of the game and capturing the
//        interactions of the user. Those interactions are grouped in:
//        - config management: Modify the user config dataset to be used.
//        - game management: Load or Save a game run into/from the data_store object
//        - engine management: Control the execution of the game engine
//        - display management: Show all the information related to the data store to the user.
//        - control management: translate the interactions of the player to actions/decisions for the game engine.
//
#pragma once

#include <SDL3/SDL.h>
#include <string>

#include "gui_sim_interface.hpp"
#include "gui_item_display.hpp"
#include "gui_console.hpp"
#include "gui_map.hpp"

class GUIMain {

public:


    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    GUIMain();


    // -----------------------------------------------------------------------------------------
    // Init
    //
    // Description: Initializes the GUI
    // -----------------------------------------------------------------------------------------
    SDL_AppResult Init(std::string name, std::string version, std::string url);

    // -----------------------------------------------------------------------------------------    
    // ProcessEvent
    //
    // Description: Process and Event Interaction
    // -----------------------------------------------------------------------------------------    
    SDL_AppResult ProcessEvent(void *appstate, SDL_Event *event);

    // -----------------------------------------------------------------------------------------    
    // Iterate
    //
    // Description: Periodic Call of GUI for updating status
    // -----------------------------------------------------------------------------------------    
    SDL_AppResult Iterate(void *appstate);

    // -----------------------------------------------------------------------------------------    
    // Quit
    //
    // Description: Call for Quit GUI
    // -----------------------------------------------------------------------------------------    
    void Quit(void *appstate, SDL_AppResult result);    

    
private:
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    const SDL_DisplayMode *displayMode;

    // subcomponents
    GuiSimInterface  guiSimIntf;
    GuiConsole       guiConsole;
    GuiMap           guiMap;
    std::shared_ptr<GuiItemDisplay> guiItemDisplay;
    // State Variables
    bool showConsole;

};
