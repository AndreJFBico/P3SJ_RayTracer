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

	if (disc < 0.0) //If discriminant is negative no intersection happens
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

		t1 = q / a;
		t0 = c / q;

		//t0 = (-b - sqrt(disc)) / 2 * a;
		//t1 = (-b + sqrt(disc)) / 2 * a;

		if (t1 < 0)
			return false;
		
		if (t0 < 0 && t1 > 0)
		{
			t = t1;
		}
		else t = t0;

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