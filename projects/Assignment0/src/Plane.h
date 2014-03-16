#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"

class Plane : public Geometry
{
public:
	std::vector<glm::vec3> _vertexes;

	Plane();
	bool intersect(Ray *r);
};