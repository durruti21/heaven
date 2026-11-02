// -----------------------------------------------------------------------------------------    
// gui_item_display
//
// Description: 
//     The component is in charge of displaying a panel with the information of the object
//
// ----------------------------------------------------------------------------------------- 

#include <SDL3/SDL.h>

#include "data_store/system_body.hpp"

#pragma once

class GuiItemDisplay {

public:

    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    GuiItemDisplay(SDL_Renderer *render);

    // -----------------------------------------------------------------------------------------
    // SetSystemBody
    //
    // Description: Asigns the system body
    // -----------------------------------------------------------------------------------------
    void SetSystemBody(std::shared_ptr<SystemBody> object);

    // -----------------------------------------------------------------------------------------    
    // Draw
    //
    // Description: Execute the increment stablished in the mode defined
    // ------------_-----------------------------------------------------------------------------    
    void Draw(int window_width, int window_height);

private:
    SystemBody  displayedBody;
    bool        showPannel;

    SDL_Renderer    *renderer;
    SDL_Texture     *imageTexture;

    bool       firstTime;

};