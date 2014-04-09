///////////////////////////////////////////////////////////////////////
//	P3SJ - Assignment 2
// 
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h> 
#include <GL/glut.h> 
#include <iostream> 
#include <stdio.h> 
#include "Includes.h"
#include "Scene.h" 

#define MAX_DEPTH 6 

Scene* scene = NULL;
int RES_X, RES_Y;

void reshape(int w, int h)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, RES_X - 1, 0, RES_Y - 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Draw function by primary ray casting from the eye towards the 
void drawScene()
{
	int n = RES_X * RES_Y;
	pixel* pixels = scene->getPixels();
	for (int i = 0; i < n; i++){
		glBegin(GL_POINTS);
		glColor3f(pixels[i].RGB.r, pixels[i].RGB.g, pixels[i].RGB.b);
		glVertex2f((i % 512), (i / 512));
		glEnd();
	}
	glFlush();
	glutPostRedisplay();
}

int glutInitv(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutInitWindowSize(RES_X, RES_Y);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("JAP Ray Tracing");
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glDisable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}

void loadScene(Scene *s)
{
	scene->loadNFF("..\\nff\\jap.nff");
	scene->loadObj("..\\objs\\LEET_DRAGON_FINAL.obj");
	scene->loadScene();
}

int main(int argc, char**argv)
{
	scene = new Scene();

	RES_X = scene->getWidth();
	RES_Y = scene->getHeight();

	std::thread thr(glutInitv, std::ref(argc), std::ref(argv));
	std::thread thr1(loadScene, std::ref(scene));
	thr.join();
	thr1.join();
}
