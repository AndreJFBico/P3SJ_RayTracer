#include "Sphere.h"

Sphere::Sphere() : Geometry()
{
	
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

	if (disc < 0) //If discriminant is negative no intersection happens
	{
		return false;
	}
	else if (disc == 0)
	{
		t0 = t1 = -0.5 * b / a;
	}
	else
	{
		float q;
		if (b > 0)
			q = -0.5 * (b + sqrt(disc));
		else q = -0.5 * (b - sqrt(disc));

		t0 = q / a;
		t1 = c / q;

		if (t0 > t1)
		{
			float tmp = t0;
			t0 = t1;
			t1 = tmp;
		}

		if (t1 < 0 )
			return false;
		t = t0;
		//origin of the ray is inside
		if (t0 < 0 && t1 > 0)
			return false;

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