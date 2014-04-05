#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"

class Triangle : public Geometry
{
public:
	std::vector<glm::vec3> _vertexes;
	glm::vec3 normal;

	Triangle();

	glm::vec3 calculateNormal(Ray* r);

	void computeBoundingBox();

	bool intersect(Ray *r);
};