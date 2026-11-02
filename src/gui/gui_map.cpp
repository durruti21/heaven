// -----------------------------------------------------------------------------------------    
// gui_map
//
// Description: 
//     The component is in charge of drawing the system map
//
// ----------------------------------------------------------------------------------------- 

#include <cmath>

#include "gui_map.hpp"
#include "support/logger.hpp"

#define HIT_SCALING 8

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
GuiMap::GuiMap()
{
    return;
}


// -----------------------------------------------------------------------------------------
// Init
//
// Description: Initializes the Map
// -----------------------------------------------------------------------------------------
void GuiMap::Init(SDL_Renderer *render, int window_width, int window_height)
{
    this->renderer = render;
    this->wWidth   = window_width;
    this->wHeight  = window_height;
    // Init Hitmap
    this->hitMap.resize((window_height/HIT_SCALING)*(window_width/HIT_SCALING));
    // Init camera
    camera.Init({0.0, 0.0, 0.0}, 11, window_width); // Center on Star, 500 m Km zoom level

    // Precalculate Body Pattern
    PreCalculateBodyPattern();

}

// -----------------------------------------------------------------------------------------    
// In screen calculation
//
// Description: This determines whenever a position is in the screen
// -----------------------------------------------------------------------------------------
bool GuiMap::IsInScreen(Vector3D pos)
{
    return ((pos.x >= -this->wWidth/2)&&(pos.y >= -this->wHeight/2)&&(pos.x <= this->wWidth/2)&&(pos.y <= this->wHeight/2));
}

// -----------------------------------------------------------------------------------------    
// Execute
//
// Description: Execute the increment stablished in the mode defined
// ------------_-----------------------------------------------------------------------------    
void GuiMap::Draw(int window_width, int window_height)
{
    // Window Center Point
    const int CENTER_WIDTH  = window_width/2;
    const int CENTER_HEIGHT = window_height/2;
    int       circleSize;

    //Clear Hit Map
    this->hitMap.clear();

    // Set camera resolution
    camera.SetWindowWidth(window_width);

    // Get system bodies list
    const std::vector<std::shared_ptr<SystemBody>> bodies = DataStore::GetInstance()->systemRepository->GetSystemBodies();

    // Draw System Bodies Loop
    Vector3D screenPosition;

    for (size_t i = 0; i<bodies.size(); i++)
    {
        screenPosition = camera.CalculateCameraPosition(bodies[i]->absPosition);
        
        if (IsInScreen(screenPosition))
        {
            switch (bodies[i]->bodyType)
            {
                case Data::SystemBodyType::SB_STAR:
                    circleSize = 8;
                    break;
                case Data::SystemBodyType::SB_PLANET:
                    circleSize = 4;
                    break;
                default:
                    circleSize = 2;
                    break;
            }
            //  Moons are only shown at certain distance
            if ((bodies[i]->bodyType != Data::SystemBodyType::SB_MOON) || (camera.GetFoVWidth()<= 2.5e11))
            {
                StorePosition(CENTER_WIDTH + screenPosition.x, CENTER_HEIGHT - screenPosition.y, bodies[i]);
                DrawBody(CENTER_WIDTH + screenPosition.x, CENTER_HEIGHT - screenPosition.y, circleSize);
                SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
                SDL_RenderDebugText(this->renderer, CENTER_WIDTH + screenPosition.x, CENTER_HEIGHT - screenPosition.y - 16, bodies[i]->name.c_str());
            }
        }
    }

    return;
}

// -----------------------------------------------------------------------------------------    
// Precalculate Body Pattern
//
// Description: This function draws a body
// -----------------------------------------------------------------------------------------
void GuiMap::PreCalculateBodyPattern()
{
    double angleIncrement = 2 * M_PI/BODY_VERT_LEN;

    for (int i=0; i<BODY_VERT_LEN; i++)
    {
        double angle= i*angleIncrement; 
        bodyPattern[i].position.x = 1.0 * cos(angle);
        bodyPattern[i].position.y = 1.0 * sin(angle);
        bodyPattern[i].color.r = 1.0;
        bodyPattern[i].color.g = 1.0;
        bodyPattern[i].color.b = 1.0;
        bodyPattern[i].color.a = 1.0;
    }
    // Set origin point
    bodyPattern[BODY_VERT_LEN].position.x = 0.0;
    bodyPattern[BODY_VERT_LEN].position.y = 0.0;
    bodyPattern[BODY_VERT_LEN].color.r = 1.0;
    bodyPattern[BODY_VERT_LEN].color.g = 1.0;
    bodyPattern[BODY_VERT_LEN].color.b = 1.0;
    bodyPattern[BODY_VERT_LEN].color.a = 1.0;

    // Calculate Indexes for Drawing
    int idx = 0;
    for (int i=0; i<BODY_VERT_LEN; i++)
    {
        bodyPatternIdx[idx]   = i;
        bodyPatternIdx[idx+1] = i+1;
        bodyPatternIdx[idx+2] = BODY_VERT_LEN;
        idx = idx + 3;
    }
    //Insert last roundup of circle
    bodyPatternIdx[idx-2] = 0;

}

// -----------------------------------------------------------------------------------------    
// Draw Body
//
// Description: This function draws a body
// -----------------------------------------------------------------------------------------
void GuiMap::DrawBody(int x, int y, int r)
{
    SDL_Vertex vert[BODY_VERT_LEN+1];
    // add the displacement and expansion of the pattern
    for (int i=0; i<BODY_VERT_LEN+1; i++)
    {
        vert[i] = this->bodyPattern[i];
        vert[i].position.x = (vert[i].position.x * r) + x;
        vert[i].position.y = (vert[i].position.y * r) + y;
    }
    // Draw the geometry
    bool result = SDL_RenderGeometry(this->renderer, NULL, vert, BODY_VERT_LEN+1, bodyPatternIdx, BODY_VERT_IDX);
    if (!result)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_CRITICAL, "GuiMap::DrawBody Couldn't draw body: " + std::string(SDL_GetError()));
    }
}


// -----------------------------------------------------------------------------------------    
// Store Screen Position
//
// Description: This function draws a body
// -----------------------------------------------------------------------------------------
void GuiMap::StorePosition(int x, int y, std::shared_ptr<SystemBody> object)
{
    // Store system body reference on the Hit Map
    // to allow beter fine, the position is contracted in a ratio of 1/4
    int index = (x/HIT_SCALING) + ((y/HIT_SCALING) * (this->wWidth/HIT_SCALING));
    this->hitMap[index] = object;

}


// -----------------------------------------------------------------------------------------
// Get Object At
//
// Description: This function returns the object at certain position
// -----------------------------------------------------------------------------------------
std::shared_ptr<SystemBody> GuiMap::GetObjectAt(int x, int y)
{
    //calculate index
    int index = (x/HIT_SCALING) + ((y/HIT_SCALING) * (this->wWidth/HIT_SCALING));
    return this->hitMap[index];
}


// -----------------------------------------------------------------------------------------
// Zoom in
//
// Description: This function performs zoom in of the camera by dividing by two the width
// -----------------------------------------------------------------------------------------
void GuiMap::ZoomIn() 
{
    camera.ZoomIn();
}

// -----------------------------------------------------------------------------------------
// Zoom Out
//
// Description: This function performs zoom out of the camera by multiplying by two the width
// -----------------------------------------------------------------------------------------    
void GuiMap::ZoomOut() 
{
    camera.ZoomOut();
}

// -----------------------------------------------------------------------------------------
// Move Up
//
// Description: This function moves the camera center up a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMap::MoveUp() 
{
    camera.MoveUp();
}

// -----------------------------------------------------------------------------------------
// Move Down
//
// Description: This function moves the camera center down a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMap::MoveDown() 
{
    camera.MoveDown();
}

// -----------------------------------------------------------------------------------------
// Move Left
//
// Description: This function moves the camera center left a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMap::MoveLeft() 
{
    camera.MoveLeft();
}

// -----------------------------------------------------------------------------------------
// Move Right
//
// Description: This function moves the camera center right a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMap::MoveRight() 
{
    camera.MoveRight();
}
