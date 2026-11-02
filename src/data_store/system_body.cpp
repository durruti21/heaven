// system_body
//
// Description: 
//        This class defines a system body and includes all the kinetic data of the system body

#include <cmath>

#include "system_body.hpp"


//------------------------------------------------------------------------------
// Constructor
//
// Description:
//   This function is the constructor of the class
//------------------------------------------------------------------------------
SystemBody::SystemBody(std::shared_ptr<SystemBody> parent)
{
    parentBody = parent;
    acceleration = {0.0, 0.0, 0.0};

}

//-------------------------------------------------------------------------------
// Update Kinematic State
//
// Description:
//    This function updates position and velocity based on acceleration and time 
//-------------------------------------------------------------------------------  
void SystemBody::UpdateKinematicState(Magnitude time)
{
    if (parentBody != nullptr) // system starts do not update
    {
        // This is done because long time intervals can introduce large errors on the aproximation
        if (time > DAY_DURATION) // one day
        {
            int numiter = time/DAY_DURATION;
            for (int i=0; i<numiter; i++)
            {
                acceleration =  (position * -(GRAV_CONST * parentBody->bodyMass))/ std::pow(position.modulus(),3);
                velocity     = velocity + (acceleration * DAY_DURATION);
                position     = position + (velocity * DAY_DURATION);
            }
        }
        else {
            acceleration =  (position * -(GRAV_CONST * parentBody->bodyMass))/ std::pow(position.modulus(),3);
            velocity     = velocity + (acceleration * time);
            position     = position + (velocity * time);
        }
    }
}

//-------------------------------------------------------------------------------
// Update Absolute Position
//
// Description:
//    This function updates the absolute position based on its relative position
//    and absolute position of its parent 
//-------------------------------------------------------------------------------  
void SystemBody::UpdateAbsolutePosition()
{
    if (parentBody != nullptr) // system starts do not update
    {
        // WARNING. It is assumed that the parent position has been updated before
        absPosition  = parentBody->absPosition + position;
    }
}


//-------------------------------------------------------------------------------
// Calculate Velocity Vector
//
// Description:
//    This function calculates the velocity vector based on position and
//    parent mass 
//-------------------------------------------------------------------------------  
void SystemBody::CalculateVelocityVector()
{
    double velModulus = 0.0;
    double posModulus = position.modulus();
    if (parentBody != nullptr) // system starts do not update
    {
        velModulus = sqrt((GRAV_CONST * parentBody->bodyMass) / posModulus);
        // normalized velocity vector
        velocity   =  {-position.y * (velModulus/posModulus), position.x * (velModulus/posModulus), 0.0};
    } 
}

