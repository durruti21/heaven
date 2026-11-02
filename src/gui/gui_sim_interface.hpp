// -----------------------------------------------------------------------------------------    
// gui_sim_interface
//
// Description: 
//     The component is in charge of drawing and hanling the sim interface menus
//     buttons and dialog boxes.
//
// ----------------------------------------------------------------------------------------- 

#pragma once

#include "gui_map.hpp"

class GuiSimInterface {

public:    

    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    GuiSimInterface();


    // -----------------------------------------------------------------------------------------
    // Init
    //
    // Description: Initializes the Menu
    // -----------------------------------------------------------------------------------------
    void Init(GuiMap *simMap);

    // -----------------------------------------------------------------------------------------    
    // Execute
    //
    // Description: Execute the increment stablished in the mode defined
    // ------------_-----------------------------------------------------------------------------    
    void Draw(int window_width, int window_height);


private:

    //selected period of time
    int selectedPeriod;
    GuiMap *map;

};