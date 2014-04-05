#pragma once

#include "Ray.h"
#include "includes.h"

class BoundingBox
{
public:
	glm::vec3 min, max;

	BoundingBox();

	void computeBoundingBox();

	bool intersect(Ray *r);

};

