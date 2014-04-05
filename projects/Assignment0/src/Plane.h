#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"
#include "NotObjects.h"

class Plane : public Geometry, /*interface*/public NotObjects
{
public:
	std::vector<glm::vec3> _vertexes;
	glm::vec3 normal;

	Plane();

	glm::vec3 calculateNormal(Ray* r);

	void computeBoundingBox();

	intersectVal intersect(Ray *r);
};