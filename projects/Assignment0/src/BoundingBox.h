#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"

class BoundingBox : public Geometry
{
public:
	glm::vec3 min, max;
	glm::vec3 normal;

	BoundingBox();

	glm::vec3 calculateNormal(Ray* r);

	bool intersect(Ray *r);
};