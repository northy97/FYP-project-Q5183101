#pragma once
#include<glm/glm.hpp>
#include<gl/freeglut.h>
using namespace glm;

class Particle
{

private:
	float Pressure;
	float Density;
	float Temperature;
	float Mass;
	vec3 Position;
	vec3 Acceleration;
	vec3 Velocity;


public:

	Particle()
	{
		vec3 Position = vec3(0.0f, 0.0f, 0.0f);
		vec3 Acceleration = vec3(0.0f, 0.0f, 0.0f);
		vec3 Velocity = vec3(0.0f, 0.0f, 0.0f);
		Pressure = 0.0f;
		Density = 0.0f;
		Temperature = 0.0f;
		Mass = 0.0f;

	}


	Particle(vec3 nPosition, vec3 nAcceleration, vec3 nVelocity, float nPressure, float nDensity, float nTemperature, float nMass)
	{
		Position = nPosition;
		Acceleration = nAcceleration;
		Velocity = nVelocity;
		Pressure = nPressure;
		Density = nDensity;
		Temperature = nTemperature;
		Mass = nMass;
	}

	void Display()
	{
		glutSolidSphere(1, 10, 10);
	}


	~Particle()
	{

	}
	
};

