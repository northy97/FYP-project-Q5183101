#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include <time.h>
#include <stdlib.h>
#include<iostream>
#include"Kernel.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "particle.h"
#include "global.h"
#include <windows.h>

using namespace std;

// imgui graphical opengl interface 
// possible graphics interface https://github.com/ocornut/imgui/releases
void initOpenGL()
{
   
   
	glClearColor(0,0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.0001, 100);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	float ambient[3] = { 1,1,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	float diffuse[3] = { 0.3,0.3,0.7 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

}

void ApplyExternalForce(ParticleSystem& ParticleSystem,
    float xforce,
    float yforce)
{
    vector<Particle>& particles = ParticleSystem.GetParticles();

    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];

        if (xforce > 0)
        {
            if (p.r.x < -0.07)
                p.a.x += xforce;
        }
        else
        {
            if (p.r.x > 0.07)
                p.a.x += xforce;
        }

        if (yforce > 0)
        {
            if (p.r.z > 0.25)
                p.a.z += -yforce;
        }
        else
        {
            if (p.r.z < -0.25)
                p.a.z += -yforce;
        }
    }
}


bool simulate = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A)
	{
		if (!simulate)
		{
			simulate = !simulate;
		}
	}
}

int main(int argc, char* argv[])
{
	GLFWwindow* window;
    glutInit(&argc, argv);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("3d");
    glClearColor(1, 1, 1, 1);
	

	if (!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(640, 480, "", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

    initOpenGL();
 

    
    Camera camera;
    camera.Init(3.14159f / 1.f, 0, 0.6);

    int mouse_lx = 0;
    int mouse_ly = 0;

    int mouse_rx = 0;
    int mouse_ry = 0;


    ParticleSystem ParticleSystem;
   
    
    
    

    bool exforce = false;
    float xforce = 0;
    float yforce = 0;

    double last_update = glfwGetTime();

    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {


        double mx, my;
        glfwGetCursorPos(window, &mx, &my);


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            int dx = mouse_lx - (int)mx;
            int dy = mouse_ly - (int)my;

            mouse_lx = (int)mx;
            mouse_ly = (int)my;


            camera.UpdateAngles(dy / 100.0f, dx / 100.0f);
        }

        // holding mouse right button + mouse move to apply external force
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            int dx = (int)mx - mouse_rx;
            int dy = (int)my - mouse_ry;

            mouse_rx = (int)mx;
            mouse_ry = (int)my;

            exforce = true;
			std::cout << "hit" << std::endl;
            xforce = dx * 3.0f;
            yforce = dy * 3.0f;
        }

        mouse_lx = (int)mx;
        mouse_ly = (int)my;

        mouse_rx = (int)mx;
        mouse_ry = (int)my;


        if (simulate) {


            ParticleSystem.UpdateFluidDensity();



            ParticleSystem.UpdateFluidAcceleration();


            // apply external force to particles applied by user
            if (exforce == true)
            {
                exforce = false;

                ApplyExternalForce(ParticleSystem, xforce, yforce);

                xforce = 0;
                yforce = 0;
            }


            ParticleSystem.UpdateFluidPosition();
          //strick silly sphere cole render here 
          
           


//
            ParticleSystem.UpdateGrid(); // fix update grid

            // lock at 30 fps
            {
                double cur = glfwGetTime();
                while (cur - last_update < 0.033)
                {
                    cur = glfwGetTime();
                }
                last_update = cur;
            
            }
            
          
        }
       /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3d(1, 0, 0);

        glPushMatrix();
        glTranslated(0.0, 1.2, -6);
        glutSolidSphere(1, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glTranslated(0.0, 5, -5);
        glutWireSphere(1, 16, 16);
        glPopMatrix();

        glutSwapBuffers();
        */
        ParticleSystem.Render();
        glfwSwapBuffers(window);
     

        // Poll for and process events
        glfwPollEvents();


    }

    glfwTerminate();
    return 0;
}
