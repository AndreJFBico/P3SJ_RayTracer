#include "Sphere.h"

Sphere::Sphere() : Geometry()
{
	
}

bool Sphere::intersect(Ray *r)
{
	glm::vec3 L = _center - r->origin;
	float tca = glm::dot(L, r->direction);

	if (tca < 0)
		return false;

	float d2 = glm::dot(L, L) - tca * tca;

	if (d2 >(_radius * _radius))
		return false;

	float thc = sqrt(_radius * _radius)- d2;

	float t0 = tca - thc;
	float t1 = tca + thc;
	float t = 0.0;
	/*///Squared distance between ray origin and sphere center
	float squaredDist = glm::dot(r->origin - _center, r->origin - _center);

	//If the distance is less than the squared radius of the sphere...
	if (squaredDist <= (_radius * _radius))
	{
		//Point is in sphere, consider as no intersection existing
		//std::cout << "Point inside sphere..." << std::endl;
		return false;
	}

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
		//std::cout << "No intersection with sphere..." << std::endl;
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
		*/
		// if t1 is less than zero, the object is in the ray's negative direction
		// and consequently the ray misses the sphere
		if (t1 < 0)
			return false;

		// if t0 is less than zero, the intersection point is at t1
		if (t0 < 0)
		{
			t = t1;
		}
		// else the intersection point is at t0
		else
		{
			t = t0;
		}/*
	}
	*/
	r->intersectPoint = r->origin + r->direction*t;
	r->dToObject = glm::length(r->intersectPoint - r->origin);
	return true;
}

glm::vec3 Sphere::calculateNormal(Ray* r)
{
	glm::vec3 normal = r->intersectPoint - _center;
	return glm::normalize(normal);
}