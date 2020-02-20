#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include "Particle.h"


// next step is to start implementing the physics system will look 2d until camera setup but will use 3d calculations
GLfloat xRotated, yRotated, zRotated;
GLdouble Radius = 1;
std::vector<Particle>particles;
const int MaximumParticles = 2;
bool generateParticles = false;
bool toggleSim = false;
const float radius = 0.1f;
//Smoothing length
const float smooth_length = 0.45;
//The ideal density. This is the density of water
const float rho0 = 10.0f;
//The speed of sound in water
const float c = 100.0f;
const float epsilon = 0.01f;
//timestep value
const float dt = 0.033f;
float total_time = 0.0f;



void display(void);
void reshape(int x, int y);
void addParticle(vec3 Position, vec3 Velocity);
void TimeStep();
void initParticles();

void idle(void)
{

	int timems = glutGet(GLUT_ELAPSED_TIME);
	
	if (timems % 10 == 0) {
	
			
		TimeStep();
		
	}
}

void initParticles()
{
	vec3 position(-4, 0, -20);
	vec3 velocity(0, 0, 0 );
	
	
	for (int y = 0; y < 5; y ++)
	{
		for (int x= 0; x < 5; x++)
		{
			addParticle(vec3(position.x + x, position.y+y,position.z), velocity);
			std::cout << particles.size() << std::endl;
		}

	}


}
void addParticle(vec3 position, vec3 velocity)
{
	float density = rho0 + epsilon;
	float mass = .1f * smooth_length * smooth_length * smooth_length * rho0;
	Particle tempParticle(position, velocity,density,mass);
	particles.push_back(tempParticle);
}

void TimeStep()
{
	std::cout << particles.size() << std::endl;

	

	//std::cout << "Grid updated" << std::endl;
	//physics.update_density(particles);
	//std::cout << "Density updated" << std::endl;
	//physics.update_forces(particles, spheres);
	//std::cout << "Forces updated" << std::endl;
	//std::cout << "Pos/Vel updated" << std::endl;
	//std::cout << "Step done" << std::endl;
	total_time += dt;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(350, 350);
	glutCreateWindow("Solid Sphere");
	xRotated = yRotated = zRotated = 30.0;
	xRotated = 43;
	yRotated = 50;
	initParticles();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void display(void)
{

	glMatrixMode(GL_MODELVIEW);
	// clear the drawing buffer.
	glClear(GL_COLOR_BUFFER_BIT);
	
	for (int x = 0; x < particles.size(); x++)
	{
		particles[x].Display();
	}

	glFlush();
	// sawp buffers called because we are using double buffering 
   // glutSwapBuffers();
}

void reshape(int x, int y)
{
	if (y == 0 || x == 0) return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(39.0, (GLdouble)x / (GLdouble)y, 0.6, 21.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);  //Use the whole window for rendering
}