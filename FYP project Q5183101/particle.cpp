#include "particle.hpp"
// setting default state for particles
Particle::Particle( float mass, Vector3f pos ) {
    mPos = pos;
    mForce = Vector3f(0.0f, 0.0f, 0.0f);
    mDensity = 0.0f;
    mPressure = 0.0f;
    mPressureForce = Vector3f(0.0f, 0.0f, 0.0f);
    mViscoForce = Vector3f(0.0f, 0.0f, 0.0f);
    mGravitationalForce = Vector3f(0.0f, 0.0f, 0.0f);
    mSurfaceNormal = Vector3f(0.0f, 0.0f, 0.0f);
    mSurfaceTensionForce = Vector3f(0.0f, 0.0f, 0.0f);
    mMass = mass;
    mAccel = Vector3f(0.0f, 0.0f, 0.0f);
    mVeloc = Vector3f(0.0f, 0.0f, 0.0f);
}