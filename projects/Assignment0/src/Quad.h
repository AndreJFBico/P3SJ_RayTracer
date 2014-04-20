#pragma once

#include "includes.h"
#include "Geometry.h"
#include "Ray.h"

class Quad : public Geometry
{
public:
	std::vector<glm::vec3> _vertexes;
	glm::vec3 normal;

	Quad();

	glm::vec3 calculateNormal(Ray *r);

	void computeBoundingBox();

	intersectVal intersect(Ray *r);
};