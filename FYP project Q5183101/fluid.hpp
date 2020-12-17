#define _USE_MATH_DEFINES

#ifndef _FLUID_HPP_
#define _FLUID_HPP_
#include "particle.hpp"
#include <vector>



////#define restDensity               998.29f
////#define mass                       0.02f
////#define viscosity                  3.5f 
////#define surfaceTension            0.0728f
////#define thresholdBoundary                  7.065f
////#define gasStiffness              3.0f
////#define restitution                0.0f
////#define SUPPORT_RADIUS             0.0457f
////#define kernalParticleSize           20
////#define gravitationalConstant Vector3f(0.0f, -9.82f, 0.0f)
////#define TimeStep                  0.01f
////#define containerSize                   0.4f

//default is set to slime change to water setting written above 
// comment out below settings uncomment the ones above to change material

#define restDensity               1000.0f// rest density of water is  kg/mg
#define mass                       0.04f
#define viscosity                  10.0f 
#define surfaceTension            6.0f//tension (72.8 millinewtons (mN) per meter

#define gasStiffness              5.0f
#define restitution                0.5f// ratio of the initial velocity to the final velocity after two objects colide
#define SUPPORT_RADIUS             0.0726f
#define kernalParticleSize           40
#define gravitationalConstant Vector3f(0.0f, -9.82f, 0.0f)//force of gravity on earth
#define TimeStep                  0.01f
#define containerSize                   0.4f
#define thresholdBoundary                  5.0f



class Fluid {

public:

    vector<Particle> m_Particles;
    Fluid( void );
    const float fluidVolume = 1000 * mass / restDensity;// volume of the fluid calculation 
    const float particleDiameter = powf(fluidVolume, 1.0f / 3.0f) / 10;
    const float particleRadius = particleDiameter / 2;

    void runSimulation( void );
    void drawScene(void);

    //calculate standard physics forces 
    float calcDensity( Vector3f );
    float calcPressure( float );
    Vector3f calcPressureForce( int, float, float, Vector3f );
    Vector3f calcViscosityForce( int, Vector3f, Vector3f );
    Vector3f calcGravitationalForce( float );
    Vector3f calcSurfaceNormal( Vector3f );
    Vector3f calcSurfaceTensionForce( Vector3f, Vector3f );

    //collision check and physics update
    void employEulerIntegrator( Particle &, Vector3f );
    bool collisionCheck( Particle, Vector3f &, Vector3f & );
    void velocityUpdate( Vector3f &, Vector3f, float );

    //kernal used for mesh free computation since no grid and all particle based
    Vector3f DefaultKernalGradiant( Vector3f, float );
    Vector3f pressureKernelGradiant(Vector3f, float);
    float useKernal(Vector3f, float);
    float defaultKernelLap(Vector3f, float);
    float viscosKernalLap(Vector3f, float);
   
};

#endif