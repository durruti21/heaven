// -----------------------------------------------------------------------------------------    
// gui_map
//
// Description: 
//     The component is in charge of drawing the system map
//
// ----------------------------------------------------------------------------------------- 

#pragma once

#include <vector>
#include <SDL3/SDL.h>

#include "gui_map_camera.hpp"
#include "data_store/data_store.hpp"

#define BODY_VERT_LEN 8
#define BODY_VERT_IDX 3 * BODY_VERT_LEN

class GuiMap {

public:    

    // -----------------------------------------------------------------------------------------    
    // Constructor
    // ----------------------------------------------------------------------------------------- 
    GuiMap();


    // -----------------------------------------------------------------------------------------
    // Init
    //
    // Description: Initializes the Map
    // -----------------------------------------------------------------------------------------
    void Init(SDL_Renderer *render, int window_width, int window_height);

    // -----------------------------------------------------------------------------------------    
    // Execute
    //
    // Description: Execute the increment stablished in the mode defined
    // -----------------------------------------------------------------------------------------   
    void Draw(int window_width, int window_height);


    // -----------------------------------------------------------------------------------------
    // Zoom in
    //
    // Description: This function performs zoom in of the camera by dividing by two the width
    // -----------------------------------------------------------------------------------------
    void ZoomIn();

    // -----------------------------------------------------------------------------------------
    // Zoom Out
    //
    // Description: This function performs zoom out of the camera by multiplying by two the width
    // -----------------------------------------------------------------------------------------    
    void ZoomOut();

    // -----------------------------------------------------------------------------------------
    // Move Up
    //
    // Description: This function moves the camera center up a proportion of the FoV
    // -----------------------------------------------------------------------------------------    
    void MoveUp();


    // -----------------------------------------------------------------------------------------
    // Move Down
    //
    // Description: This function moves the camera center down a proportion of the FoV
    // -----------------------------------------------------------------------------------------    
    void MoveDown();
    

    // -----------------------------------------------------------------------------------------
    // Move Left
    //
    // Description: This function moves the camera center left a proportion of the FoV
    // -----------------------------------------------------------------------------------------    
    void MoveLeft();

    // -----------------------------------------------------------------------------------------
    // Move Right
    //
    // Description: This function moves the camera center right a proportion of the FoV
    // -----------------------------------------------------------------------------------------    
    void MoveRight();


    // -----------------------------------------------------------------------------------------
    // Get FoV Width
    //
    // Description: This function sets the FoV width
    // -----------------------------------------------------------------------------------------
    Distance GetMapWidth() {return camera.GetFoVWidth();}

    // -----------------------------------------------------------------------------------------
    // Get Object At
    //
    // Description: This function returns the object at certain position
    // -----------------------------------------------------------------------------------------
    std::shared_ptr<SystemBody> GetObjectAt(int x, int y);


private:
    // -----------------------------------------------------------------------------------------    
    // Precalculate Body Pattern
    //
    // Description: This function draws a body
    // -----------------------------------------------------------------------------------------
    void PreCalculateBodyPattern();

    // -----------------------------------------------------------------------------------------    
    // In screen calculation
    //
    // Description: This determines whenever a position is in the screen
    // -----------------------------------------------------------------------------------------
    bool IsInScreen(Vector3D pos);

    // -----------------------------------------------------------------------------------------    
    // Draw Body
    //
    // Description: This function draws a body
    // -----------------------------------------------------------------------------------------
    void DrawBody(int x, int y, int r);

    // -----------------------------------------------------------------------------------------    
    // Store Screen Position
    //
    // Description: This function draws a body
    // -----------------------------------------------------------------------------------------
    void StorePosition(int x, int y, std::shared_ptr<SystemBody> object);


   
    SDL_Renderer    *renderer;
    GuiMapCamera    camera;    
    // window sizes
    int wWidth; 
    int wHeight;

    // Hit map
    //bool reCheckHitMap; 
    // This feature has been removed because can not control when items are updated on the hitmap
    // But it will be wise to came again to this problem because it can be a performance penalty
    std::vector<std::shared_ptr<SystemBody>> hitMap;

    // System Body precalculated pattern
    SDL_Vertex bodyPattern[BODY_VERT_LEN+1];
    int        bodyPatternIdx[BODY_VERT_IDX];



};