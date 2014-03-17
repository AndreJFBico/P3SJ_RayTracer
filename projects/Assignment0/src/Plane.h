#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"

class Plane : public Geometry
{
public:
	std::vector<glm::vec3> _vertexes;
	glm::vec3 normal;

	Plane();

	glm::vec3 calculateNormal(Ray* r);

	bool intersect(Ray *r);
};