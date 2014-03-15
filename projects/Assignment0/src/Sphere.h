#pragma once

#include "includes.h"
#include "Geometry.h"

class Sphere : public Geometry
{
public:
	glm::vec3 _center;
	float _radius;

	Sphere();
	void calcIntersect();
};