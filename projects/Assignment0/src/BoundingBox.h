#pragma once

#include "Ray.h"
#include "includes.h"
#include "Structs.h"

class BoundingBox
{
public:
	glm::vec3 min, max;
	glm::vec3 _tmax, _tmin;
	float _tminf;

	BoundingBox();

	void computeBoundingBox();

	intersectVal intersect(Ray *r);

};

