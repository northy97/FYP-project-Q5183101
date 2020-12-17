#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_


#include <GL/glut.h>

#include "utility.h"

class Particle {
public:
    Particle( float, Vector3f );
    float mMass;
    Vector3f mForce;
    Vector3f mAccel;
    Vector3f mVeloc;
    Vector3f mPos;
    float mDensity;
    float mPressure;
    Vector3f mPressureForce;
    Vector3f mViscoForce;
    Vector3f mGravitationalForce;
    Vector3f mSurfaceNormal;
    Vector3f mSurfaceTensionForce;
};

#endif