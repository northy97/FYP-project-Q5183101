#include "fluid.hpp"
#include <vector>
#include <time.h>

// press s to start the simulation 
// press a to add a force through the material 
// press z to add particles

using namespace std;

// need to comment more and adjust all code names and structure
//adjust interactivity maybe add 3d camera.
Vector3f lookat{ 0.2,0.3,0.6 };
Fluid fluid;
bool start = false;


void displayCallback(void) { // display basic set background colour and look 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(lookat.x, lookat.y, lookat.z, 0.0, 0.0, 0.0, 0.0, 0.9, 0.0);
    
    if (start){
        fluid.runSimulation();
    }
    
    fluid.drawScene();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1024.0 / 768.0, 0.05, 100.0);
    glutSwapBuffers();


}

void keyboardCallback(unsigned char key, int x, int y) {//handle keyboard input
    if (key == 's') // start the simulation 
        start = true;

    if (key == 'a')// to interact applies force along x axis 
    {
        for (int i = 0; i < fluid.m_Particles.size()/2; i++)
        {
            fluid.m_Particles[i].mVeloc.x -=0.5;
        }
    }

    if (key == 'z')// generate particle added to simulation 
    {
       fluid.m_Particles.push_back(Particle(mass, Vector3f(fluid.particleRadius * -3, fluid.particleRadius * 5, fluid.particleRadius * -4)));
       fluid.m_Particles.push_back(Particle(mass, Vector3f(fluid.particleRadius * 3, fluid.particleRadius * 5, fluid.particleRadius * 4)));
     
    }
}

void mouseClick(int button, int status, int x, int y)//handle mouse input
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (status == GLUT_DOWN)
        {
            
        }
    }
}

void setLighting(void) { // set basic lighting 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float matAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float matDif[] = { 0.1f, 0.2f, 0.9f, 1.0f };
    float matSpc[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float matShi[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpc);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShi);

    float lightPos[] = { 1.0f, 0.0f, 1.0f, 0.0f };
    float lightAmb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float lightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lightSpc[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpc);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("final year project");
    glutDisplayFunc(displayCallback);
    glutIdleFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseClick);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    setLighting();
    glutMainLoop();
    return 0;
}

