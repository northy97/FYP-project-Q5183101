#pragma once
#include<glm/glm.hpp>
#include<gl/freeglut.h>
using namespace glm;

class Particle
{

	


public:
	float Pressure;
	float Density;

	float Mass;
	vec3 Position;
	vec3 Velocity;
	vec3 acceleration;
	Particle()
	{
		vec3 Position = vec3(0.0f, 0.0f, 0.0f);
		vec3 Velocity = vec3(0.0f, 0.0f, 0.0f);
		vec3 acceleration = vec3(0.0f,0.0f,0.0f);

		Density = 0.0f;
		Mass = 0.0f;

	}


	Particle(vec3 nPosition, vec3 nVelocity, float nDensity, float nMass)
	{
		Position = nPosition;
		Velocity = nVelocity;
		acceleration = vec3(0.0f, 0.0f, 0.0f);
		Density = nDensity;
		Mass = nMass;
	}

	void Display()
	{
		glLoadIdentity();
		glTranslatef(Position.x, Position.y, -20.0);
		glColor3f(0.9, 0.3, 0.2);
		glutWireSphere(0.2, 20, 20);
		
	
	}


	~Particle()
	{

	}
	
};

