#pragma once
#include <iostream>
#include <vector>



using namespace std;

struct Vector3
{
	float x, y, z;
};

class Particle
{

public:

	

private:
	vector<Particle*>neighbour;
	float ID;
	float mass;
	float density;
	float radius;
	//float colour;
	float pressure;
	Vector3 lastPos;
	Vector3 currentPos;
	Vector3 lastVelo;
	Vector3 currentVelo;
	Vector3 lastAccel;
	Vector3 currentAccel;
	Vector3 surfaceNorm;




};

