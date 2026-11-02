// -----------------------------------------------------------------------------------------    
// gui_map_camera
//
// Description: 
//     The component is in charge of the camera that control how the map is shown
//
// ----------------------------------------------------------------------------------------- 

#include "gui_map_camera.hpp"

#include "support/logger.hpp"

#define MOVE_RATIO 10

#define MAX_ZOOM_LEVEL 28 // From 0 to 28
#define NUM_ZOOM_LEVELS MAX_ZOOM_LEVEL+1

const Distance ZOOM_LEVELS[NUM_ZOOM_LEVELS] = 
{
    1.0e8,  2.5e8,  5.0e8,  1.0e9,  2.5e9,  5.0e9, 
    1.0e10, 2.5e10, 5.0e10, 1.0e11, 2.5e11, 5.0e11, 1.0e12, 2.5e12, 5.0e12, 
    1.0e13, 2.5e13, 5.0e13, 1.0e14, 2.5e14, 5.0e14, 1.0e15, 2.5e15, 5.0e15, 
    LIGHT_YEAR, 2.5 * LIGHT_YEAR, 5.0 * LIGHT_YEAR, 10.0 * LIGHT_YEAR, 25.0 * LIGHT_YEAR   
};

// -----------------------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------------------
void GuiMapCamera::Init(Vector3D center, int zoom, int window)
{
    this->fovCenter     = center;
    this->zoomLevel     = zoom;
    this->fovWidth      = ZOOM_LEVELS[zoom];
    this->windowWidth   = window;
    CalculateCameraResolution();
}

// -----------------------------------------------------------------------------------------
// Set Camera Center
//
// Description: This function sets the window width
// -----------------------------------------------------------------------------------------
void GuiMapCamera::SetCameraCenter(Vector3D center)
{
    this->fovCenter = center;
}

// -----------------------------------------------------------------------------------------
// Set Window Width
//
// Description: This function sets the window width
// -----------------------------------------------------------------------------------------
void GuiMapCamera::SetWindowWidth(int width)
{
    this->windowWidth   = width;
    CalculateCameraResolution();
}

// -----------------------------------------------------------------------------------------
// Calculate Camera Positions
//
// Description: This function calculates the ratios of camera vs distances
// -----------------------------------------------------------------------------------------
void GuiMapCamera::CalculateCameraResolution()
{
    // ONLY A 2D Calculation is performed with no deformation   
    this->fovRatio.x = this->windowWidth  / this->fovWidth;
    this->fovRatio.y = this->fovRatio.x;
}

// -----------------------------------------------------------------------------------------
// Calculate Camera Positions
//
// Description: This function calculates the position on the camera in pixels
// -----------------------------------------------------------------------------------------
Vector3D GuiMapCamera::CalculateCameraPosition(Vector3D position)
{
    // ONLY A 2D Calculation is performed
    return (position - this->fovCenter) * this->fovRatio;
}

// -----------------------------------------------------------------------------------------
// Set Zoom Level
//
// Description: This function sets the FoV width based on the zoom selected
// -----------------------------------------------------------------------------------------
void GuiMapCamera::SetZoomLevel(int zoom)
{
    if ((zoom >= 0)&&(zoom <= MAX_ZOOM_LEVEL))
    {
        this->zoomLevel = zoom;
        this->fovWidth  = ZOOM_LEVELS[zoom];
        CalculateCameraResolution();

    } else {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_CRITICAL, "GuiMapCamera::SetZoomLevel Invalid value: {0:d}", zoom);
    }
}

// -----------------------------------------------------------------------------------------
// Zoom in
//
// Description: This function performs zoom in of the camera by dividing by two the width
// -----------------------------------------------------------------------------------------
void GuiMapCamera::ZoomIn() 
{
    if (this->zoomLevel > 0)
    {
        this->zoomLevel--;
        this->fovWidth  = ZOOM_LEVELS[this->zoomLevel];
        CalculateCameraResolution();
    }
}


// -----------------------------------------------------------------------------------------
// Zoom Out
//
// Description: This function performs zoom out of the camera by multiplying by two the width
// -----------------------------------------------------------------------------------------    
void GuiMapCamera::ZoomOut()
{
    if (this->zoomLevel < MAX_ZOOM_LEVEL)
    {
        this->zoomLevel++;
        this->fovWidth  = ZOOM_LEVELS[this->zoomLevel];
        CalculateCameraResolution();
    }
}


// -----------------------------------------------------------------------------------------
// Move Up
//
// Description: This function moves the camera center up a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMapCamera::MoveUp()
{
    this->fovCenter.y = this->fovCenter.y + (this->fovWidth/MOVE_RATIO);
}


// -----------------------------------------------------------------------------------------
// Move Down
//
// Description: This function moves the camera center down a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMapCamera::MoveDown()
{
    this->fovCenter.y = this->fovCenter.y - (this->fovWidth/MOVE_RATIO);
}


// -----------------------------------------------------------------------------------------
// Move Left
//
// Description: This function moves the camera center left a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMapCamera::MoveLeft()
{
  this->fovCenter.x = this->fovCenter.x - (this->fovWidth/MOVE_RATIO);
}

// -----------------------------------------------------------------------------------------
// Move Right
//
// Description: This function moves the camera center right a proportion of the FoV
// -----------------------------------------------------------------------------------------    
void GuiMapCamera::MoveRight()
{
  this->fovCenter.x = this->fovCenter.x + (this->fovWidth/MOVE_RATIO);
}