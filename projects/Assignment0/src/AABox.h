#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"

class AABox : public Geometry
{
public:
	glm::vec3 min, max;
	glm::vec3 normal;

	AABox();

	glm::vec3 calculateNormal(Ray* r);

	void computeBoundingBox();

	bool intersect(Ray *r);
};