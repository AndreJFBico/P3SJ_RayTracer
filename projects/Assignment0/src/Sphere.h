#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"

class Sphere : public Geometry
{
public:
	glm::vec3 _center;
	float _radius;

	Sphere();

	glm::vec3 calculateNormal(Ray* r);

	bool intersect(Ray *r);
};