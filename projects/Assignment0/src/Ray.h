#pragma once

#include "Includes.h"

class Ray
{

	glm::vec3 direction;

public:
	
	Ray();

	//receives pixel coord and eye coord and calculates and returns ray direction vector
	glm::vec3 calculateWCS(glm::vec2 pixel, glm::vec3 at, glm::vec3 eye, glm::vec3 up);

	void trace();

	virtual ~Ray();
};

