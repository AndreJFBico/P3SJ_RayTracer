#include "Sphere.h"

Sphere::Sphere() : Geometry()
{
	_boundingBox = *(new BoundingBox());
}

bool Sphere::intersect(Ray *r)
{
	//Will hold solution to quadratic equation
	float t0, t1;

	//Calculating the coefficients of the quadratic equation
	float a = glm::dot(r->direction, r->direction); // a = d*d
	float b = 2.0f*glm::dot(r->direction, r->origin - _center); // b = 2(o-C)*d
	float c = glm::dot(r->origin - _center, r->origin - _center) - (_radius * _radius); // c = (o-C)*(o-C)-R^2

	//Calculate discriminant
	//float disc = (b*b) - (4.0f*a*c);
	float disc = (b*b) - (4.0f*a*c), t;

	if (disc < 0.0) //If discriminant is negative no intersection happens
	{
		return false;
	}
	else if (disc == 0)
	{
		t0 = t1 = -0.5f * b / a;
	}
	else
	{
		float q;
		if (b > 0)
			q = -0.5f * (b + sqrt(disc));
		else q = -0.5f * (b - sqrt(disc));
		
		if (c < 0)
		{
			t = q / a;
		}
		else {
			t1 = q / a;
			t0 = c / q;

			if (t1 < 0)
				return false;

			t = t0;
		}

		r->intersectPoint = r->origin + r->direction*t;
		r->dToObject = glm::length(r->intersectPoint - r->origin);
		return true;
	}
	return false;
}

glm::vec3 Sphere::calculateNormal(Ray* r)
{
	glm::vec3 normal = r->intersectPoint - _center;
	return glm::normalize(normal);
}

void Sphere::computeBoundingBox()
{
	glm::vec3 localmin, localmax;

	_boundingBox.min.x = _center.x - (_radius + 0.1f);
	_boundingBox.min.y = _center.y - (_radius + 0.1f);
	_boundingBox.min.z = _center.z - (_radius + 0.1f);

	_boundingBox.max.x = _center.x + (_radius + 0.1f);
	_boundingBox.max.y = _center.y + (_radius + 0.1f);
	_boundingBox.max.z = _center.z + (_radius + 0.1f);
}