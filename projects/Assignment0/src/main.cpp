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
#include <time.h>
#include <string>

#define MAX_DEPTH 6 

Scene* scene = NULL;
int RES_X, RES_Y;
int sceneNum = 1;
int numDepth = 1;
int numSF = 1;

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

void loadScene(Scene *s, int key, int scenenum)
{

	clock_t begin, begin2, end;
	begin = clock();
	
	std::cout << "loading assets ..." << std::endl;

	switch (scenenum)
	{
	case 1:
		scene->loadNFF("..\\nff\\balls_low.nff");
		break;
	case 2:
		scene->loadNFF("..\\nff\\balls_high.nff");
		break;
	case 3:
		scene->loadNFF("..\\nff\\mount_low.nff");
		break;
	case 4:
		scene->loadNFF("..\\nff\\mount_very_high.nff");
		break;
	case 5:
		scene->loadNFF("..\\nff\\jap.nff");
		scene->loadObj("..\\objs\\LEET_DRAGON_FINAL.obj", true, glm::vec3(0.0f, 0.0f, 1.0f) , 0.0f, 0.0f);
		break;
	case 6:
		scene->loadNFF("..\\nff\\jap.nff");
		scene->loadObj("..\\objs\\main_terrain.obj", true, glm::vec3(204.0f / 255.0f, 102.0f / 255.0f, 51.0f / 255.0f), 0.1f, 0.0f);
		scene->loadObj("..\\objs\\trees.obj", true, glm::vec3(0.077f, 0.223f, 0.03f), 0.0f, 0.0f);
		scene->loadObj("..\\objs\\snow.obj", true, glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.0f);
		scene->loadObj("..\\objs\\green.obj", true, glm::vec3(90.0f / 255.0f, 118.0f / 255.0f, 69.0f / 255.0f), 0.1f, 0.0f);
		scene->loadObj("..\\objs\\dark.obj", true, glm::vec3(204.0f / 255.0f, 102.0f / 255.0f, 51.0f / 255.0f), 0.2f, 0.0f);
		scene->loadObj("..\\objs\\water.obj", true, glm::vec3(32.0f / 255.0f, 4.0f / 255.0f, 247.0f / 255.0f), 0.2f, 1.0f);
		scene->loadObj("..\\objs\\base.obj", false, glm::vec3(34.0f / 255.0f, 13.0f / 255.0f, 12.0f / 255.0f), 0.2f, 0.0f);
		scene->loadObj("..\\objs\\mount2.obj", true, glm::vec3(204.0f / 255.0f, 102.0f / 255.0f, 51.0f / 255.0f), 0.1f, 0.0f);
		break;
	default:
		break;
	}
	
	
	std::cout << "finished loading assets ..." << std::endl;
	begin2 = clock();
	std::cout << "Assets loading time: " << (double)(begin2 - begin) / CLOCKS_PER_SEC;
	scene->loadScene(key, numDepth, numSF);
	end = clock();
	std::cout << "Render Time: " << (double)(end - begin2) / CLOCKS_PER_SEC;
	std::cout << "Total Time taken: " << (double)(end - begin) / CLOCKS_PER_SEC;
}

void keyboard(unsigned char key, int x, int y)
{
	std::cout << key << std::endl;
	if (key == 'q')
	{
		std::thread thr1(loadScene, std::ref(scene), 1, sceneNum);
		thr1.detach();

	}
	if (key == 'w')
	{
		std::thread thr1(loadScene, std::ref(scene), 2, sceneNum);
		thr1.detach();
	}
	if (key == 'e')
	{
		std::thread thr1(loadScene, std::ref(scene), 3, sceneNum);
		thr1.detach();

	}
	if (key == 'r')
	{
		std::thread thr1(loadScene, std::ref(scene), 4, sceneNum);
		thr1.detach();

	}
	if (key == 't')
	{
		std::thread thr1(loadScene, std::ref(scene), 5, sceneNum);
		thr1.detach();

	}
	if (key == 'y')
	{
		std::thread thr1(loadScene, std::ref(scene), 6, sceneNum);
		thr1.detach();
	}
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

	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glDisable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}

int main(int argc, char**argv)
{
	scene = new Scene();
	int renderingMode = 5;

	RES_X = scene->getWidth();
	RES_Y = scene->getHeight();


	std::cout << "Please choose your scene from (enter the number):" << std::endl;
	std::cout << "1 - Balls_low" << std::endl;
	std::cout << "2 - Balls_high" << std::endl;
	std::cout << "3 - Mount_Low" << std::endl;
	std::cout << "4 - Mount_high" << std::endl;
	std::cout << "5 - Solo dragon" << std::endl;
	std::cout << "6 - Custom scene" << std::endl;

	std::string s;
	getline(std::cin, s);
	sceneNum = atoi(s.c_str());

	std::cout << "Please choose your rendering mode from (enter the number):" << std::endl;
	std::cout << "1 - Aliasing with DOF" << std::endl;
	std::cout << "2 - Monte Carlo with DOF" << std::endl;
	std::cout << "3 - Aliasing only" << std::endl;
	std::cout << "4 - Monte Carlo only" << std::endl;
	std::cout << "5 - No DOF or Aliasing" << std::endl;
	std::cout << "6 - DOF only" << std::endl;

	getline(std::cin, s);
	renderingMode = atoi(s.c_str());

	std::cout << "Please choose the number of Shadow Rays:" << std::endl;

	getline(std::cin, s);
	numSF = atoi(s.c_str());

	std::cout << "Please choose the number of Depth Rays:" << std::endl;

	getline(std::cin, s);
	numDepth = atoi(s.c_str());


	std::thread thr(glutInitv, std::ref(argc), std::ref(argv));
	std::thread thr1(loadScene, std::ref(scene ), renderingMode, sceneNum);
	//threads[2] = std::thread(loadScene, std::ref(scene), 3);
	//threads[3] = std::thread(loadScene, std::ref(scene), 4);
	//threads[4] = std::thread(loadScene, std::ref(scene), 5);
	//threads[5] = std::thread(loadScene, std::ref(scene), 6);

	//aux[0] = std::thread(loadScene, std::ref(scene), 1);
	//aux[1] = std::thread(loadScene, std::ref(scene), 2);
	//aux[2] = std::thread(loadScene, std::ref(scene), 3);
	//aux[3] = std::thread(loadScene, std::ref(scene), 4);
	//aux[4] = std::thread(loadScene, std::ref(scene), 5);
	//aux[5] = std::thread(loadScene, std::ref(scene), 6);

	thr.join();
	thr1.join();
}
