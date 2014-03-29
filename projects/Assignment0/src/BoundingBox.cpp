#include "BoundingBox.h"

BoundingBox::BoundingBox() : Geometry()
{
	
}

bool BoundingBox::intersect(Ray *r)
{
	glm::vec3 dirfrac;
	// r.dir is unit direction vector of ray
	dirfrac.x = 1.0f / r->direction.x;
	dirfrac.y = 1.0f / r->direction.y;
	dirfrac.z = 1.0f / r->direction.z;
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (min.x - r->origin.x)*dirfrac.x;
	float t2 = (max.x - r->origin.x)*dirfrac.x;
	float t3 = (min.y - r->origin.y)*dirfrac.y;
	float t4 = (max.y - r->origin.y)*dirfrac.y;
	float t5 = (min.z - r->origin.z)*dirfrac.z;
	float t6 = (max.z - r->origin.z)*dirfrac.z;

	float t = 0;

	//definir tmin e tmax
	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmin < 0 && tmax < 0)
		return false;

	if (tmin > tmax)
		return false;

	if (tmin < 0 && tmax > 0)
		t = tmax;
	else t = tmin;

	/*// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;*/
	
	r->intersectPoint = r->origin + r->direction*t;
	r->dToObject = glm::length(r->intersectPoint - r->origin);

	const float ERR = 0.00001f;

	glm::vec3 center = min + ((max-min)  / 2.0f);
	if (r->intersectPoint.x > min.x - ERR && r->intersectPoint.x < min.x + ERR)
	{
		normal = glm::vec3(-1.0, 0.0, 0.0);
	}
	else if (r->intersectPoint.x > max.x - ERR && r->intersectPoint.x < max.x + ERR)
	{
		normal = glm::vec3(1.0, 0.0, 0.0);
	}
	else if (r->intersectPoint.y > min.y - ERR && r->intersectPoint.y < min.y + ERR)
	{
		normal = glm::vec3(0.0, -1.0, 0.0);
	}
	else if (r->intersectPoint.y > max.y - ERR && r->intersectPoint.y < max.y + ERR)
	{
		normal = glm::vec3(0.0, 1.0, 0.0);
	}
	else if (r->intersectPoint.z > min.z - ERR && r->intersectPoint.z < min.z + ERR)
	{
		normal = glm::vec3(0.0, 0.0, -1.0);
	}
	else if (r->intersectPoint.z > max.z - ERR && r->intersectPoint.z < max.z + ERR)
	{
		normal = glm::vec3(0.0, 0.0, 1.0);
	}
	else normal = r->intersectPoint;
	
	return true;
}

glm::vec3 BoundingBox::calculateNormal(Ray* r)
{
	return normal;
}
