#pragma once
#include <GL/freeglut.h>

float r, g, b, x, y;
bool check = true;

void mouse(int button, int state, int mouseX, int mouseY)
{
	if (button = GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		check = true;
		x = mouseX;
		y = mouseY;
	}
}