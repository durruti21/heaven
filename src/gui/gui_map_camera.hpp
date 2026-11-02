// -----------------------------------------------------------------------------------------    
// gui_map_camera
//
// Description: 
//     The component is in charge of the camera that control how the map is shown
//
// ----------------------------------------------------------------------------------------- 

#pragma once

#include "support/types/unit_types.hpp"


class GuiMapCamera {

public:    
    // -----------------------------------------------------------------------------------------
    // Constructor
    // -----------------------------------------------------------------------------------------
    void Init(Vector3D center, int zoom, int window);

    // -----------------------------------------------------------------------------------------
    // Set Camera Center
    //
    // Description: This function sets the window width
    // -----------------------------------------------------------------------------------------
    void SetCameraCenter(Vector3D center);

    // -----------------------------------------------------------------------------------------
    // Set Window Width
    //
    // Description: This function sets the window width
    // -----------------------------------------------------------------------------------------
    void SetWindowWidth(int width);

    // -----------------------------------------------------------------------------------------
    // Get FoV Width
    //
    // Description: This function sets the FoV width
    // -----------------------------------------------------------------------------------------
    Distance GetFoVWidth(){return this->fovWidth;}

    // -----------------------------------------------------------------------------------------
    // Calculate Camera Positions
    //
    // Description: This function calculates the position on the camera in pixels
    // -----------------------------------------------------------------------------------------
    Vector3D CalculateCameraPosition(Vector3D position);

    // -----------------------------------------------------------------------------------------
    // Set Zoom Level
    //
    // Description: This function sets the FoV width based on the zoom selected
    // -----------------------------------------------------------------------------------------
    void SetZoomLevel(int zoom);    

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
    

private:
    // -----------------------------------------------------------------------------------------
    // CalculateResolution
    //
    // Description: This function performs zoom out of the camera by multiplying by two the width
    // ----------------------------------------------------------------------------------------- 
    void CalculateCameraResolution();


    Vector3D  fovCenter; 
    Distance  fovWidth;
    int       windowWidth;
    int       zoomLevel;
    Vector3D fovRatio = {1.0, 1.0, 1.0};

};