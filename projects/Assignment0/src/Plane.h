#pragma once

#include "includes.h"
#include "Geometry.h"

class Plane : public Geometry
{
public:
	std::vector<glm::vec3> _vertexes;

	Plane();
	void calcIntersect();
};