#pragma once

#include "includes.h"
#include "Camera.h"

class Scene
{
	int currentPixel;
	int RES_X, RES_Y;

	std::vector<light> lights;

	Camera *c;

public:
	Scene();

	void loadNFF(const char *filename);

	void drawScene();

	void savebmp(const char *filename, int w, int h, int dpi, pixel *data);
};