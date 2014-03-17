#include "Plane.h"

Plane::Plane() : Geometry()
{
	
}

bool Plane::intersect(Ray *r)
{
	glm::vec3 normal = glm::cross(_vertexes[0] - _vertexes[1], _vertexes[1] - _vertexes[2]);

	float d = glm::dot(_vertexes[1] - r->origin, normal) / glm::dot(r->direction, normal);

	r->intersectPoint = r->origin + r->direction*d;
	r->dToObject = glm::length(r->intersectPoint - r->origin);

	if (d>0)
		return true;

	return false;
}
