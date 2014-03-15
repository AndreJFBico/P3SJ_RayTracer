#pragma once

#include "includes.h"

class Scene
{

public:
	Scene();

	void savebmp(const char *filename, int w, int h, int dpi, pixel *data);
};