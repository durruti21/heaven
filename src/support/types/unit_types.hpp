// unit_types
//
// Description: 
//        This file defines the units used on the simulation

#pragma once

#include <cmath>
#include <string>

using Magnitude    = double;  //64 bits precision
using Distance     = double;  //meters 
using Speed        = double;  //meters per second
using Acceleration = double;  //meters per second square
using Mass         = double;  //kg

const Magnitude GRAV_CONST    = 6.67430e-11;
const Magnitude M_SUN_CONST   = 1.989e30;
const Magnitude LIGHT_YEAR    = 9.461e15;
const Magnitude M_EARTH_CONST = 5.972e24;
const Magnitude DAY_DURATION  = 86400.0;
// Constant GxM only applicable to the solar system. 
// Rest of systems can be measured as a relation with the mass of the SUN
const Magnitude GM_CONST   = GRAV_CONST * M_SUN_CONST; 

class Vector3D {

public:

    Magnitude x,y,z;

    Vector3D operator+(const Vector3D& other) const {
        return {x+other.x, y+other.y, z+other.z};
    }

    Vector3D operator-(const Vector3D& other) const {
        return {x-other.x, y-other.y, z-other.z};
    }

    Vector3D operator*(const Vector3D& other) const {
        return {x*other.x, y*other.y, z*other.z};
    }

    Vector3D operator*(const Magnitude d) const {
        return {x*d, y*d, z*d};
    }

    Vector3D operator/(const Magnitude d) const {
        return {x/d, y/d, z/d};
    }

    Magnitude modulus() const {
        return std::sqrt(x*x + y*y + z*z);
    }


};

// Helper functions
namespace UnitTypes {
    
// Function to convert from Distance to String
std::string ToString(Distance range);
// Function to convert from System Body Size to String
std::string BodyMassToString(Mass size, bool inAU); 

}