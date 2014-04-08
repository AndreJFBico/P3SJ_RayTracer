#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
}

intersectVal BoundingBox::intersect(Ray *r)
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
		return intersectVal(false, NULL);

	if (tmin > tmax)
		return intersectVal(false, NULL);

	if (tmin < 0 && tmax > 0)
	{
		t = tmax;
	}
	else
	{
		t = tmin;
	}

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
	_tmin = r->origin + r->direction*tmin;
	_tmax = r->origin + r->direction*tmax;
	_tminf = tmin;
	r->t = t;
	r->intersectPoint = r->origin + r->direction*t;
	r->dToObject = glm::length(r->intersectPoint - r->origin);
	
	return intersectVal(true, NULL);
}
