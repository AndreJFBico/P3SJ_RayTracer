#pragma once

#include "includes.h"



class Scene
{
	int currentPixel;

public:
	Scene();

	void loadNFF(const char *filename);

	void drawScene();

	void savebmp(const char *filename, int w, int h, int dpi, pixel *data);
};