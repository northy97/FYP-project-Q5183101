#include "Camera.h"
#include<cmath>
#include<GL/freeglut.h>

Camera::Camera() :
    theta_(3.14159f / 2.f),
    phi_(0),
    r_(0.6)
{}

void Camera::init(float theta, float phi, float r)
{
    theta_ = theta;
    phi_ = phi;
    r_ = r;

    // constraint theta to a range
    if (theta_ < 3.14159f * 0.1f)
        theta_ = 3.14159f * 0.1f;
    else if (theta_ > 3.14159f * 0.9f)
        theta_ = 3.14159f * 0.9f;

    Vector3f pos = sphericalToXYZ();

    glLoadIdentity();
   // gluLookAt(pos.x, pos.y, pos.z, 0, 0, 0, 0, 0, 1);
}

void Camera::updateAngles(float dTheta, float dPhi)
{
    theta_ += dTheta;

    // constraint theta to a range
    if (theta_ < 3.14159f * 0.1f)
        theta_ = 3.14159f * 0.1f;
    else if (theta_ > 3.14159f * 0.9f)
        theta_ = 3.14159f * 0.9f;

    phi_ += dPhi;

    Vector3f pos = sphericalToXYZ();
    tempPos = pos;
    
    
}

Vector3f Camera::sphericalToXYZ()
{
    float x = r_ * sin(theta_) * cos(phi_);
    float y = r_ * sin(theta_) * sin(phi_);
    float z = r_ * cos(theta_);
    return Vector3f(x,y,z);
}
