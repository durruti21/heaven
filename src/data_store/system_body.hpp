// system_body
//
// Description: 
//        This class defines a system body and includes all the kinetic data of the system body

#pragma once

#include "support/types/unit_types.hpp"
#include "data_store/data_types.hpp"

#include <vector>
#include <memory>
 

class SystemBody {

private:
    std::shared_ptr<SystemBody> parentBody;

public:
    std::string           name;
    Data::SystemBodyType  bodyType;
    Mass                  bodyMass;
    Vector3D              position;    // Relative position with respect its parent body
    Vector3D              absPosition; // Absolute position regarding system reference
    Vector3D              velocity;
    Vector3D              acceleration;

    //------------------------------------------------------------------------------
    // Constructor
    //
    // Description:
    //   This function is the constructor of the class
    //------------------------------------------------------------------------------
    SystemBody(std::shared_ptr<SystemBody> parent = nullptr); 

    //-------------------------------------------------------------------------------
    // Update Kinematic State
    //
    // Description:
    //    This function updates position and velocity based on acceleration and time 
    //-------------------------------------------------------------------------------  
    void UpdateKinematicState(Magnitude time);

    //-------------------------------------------------------------------------------
    // Update Kinematic State
    //
    // Description:
    //    This function updates the absolute position based on its relative position
    //    and absolute position of its parent 
    //-------------------------------------------------------------------------------  
    void UpdateAbsolutePosition();

    //-------------------------------------------------------------------------------
    // Calculate Velocity Vector
    //
    // Description:
    //    This function calculates the velocity vector based on position and
    //    parent mass 
    //-------------------------------------------------------------------------------  
    void CalculateVelocityVector();
};