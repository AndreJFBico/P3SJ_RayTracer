#include "Sphere.h"

Sphere::Sphere() : Geometry()
{
	
}

bool Sphere::intersect(Ray *r)
{
	/*glm::vec3 el = _center - r->origin;
	float d = glm::dot(el, r->direction);
	float els = glm::dot(el, el);
	float rs = _radius * _radius;

	if (d < 0 && els > rs)
	{
		return false;
	}

	float ms = els - d * d;

	if (ms > rs)
	{
		return false;
	}

	float q = sqrt(rs - ms);
	float t;

	if (els > rs)
	{
		t = d - q;
	}
	else
	{
		t = d + q;
	}

	glm::vec3 p = r->origin + r->direction * t;
	r->intersectPoint = p;
	r->dToObject = glm::length(p - r->origin);
	return true;*/




	///Squared distance between ray origin and sphere center
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

	//< 0
	//> 0
	// == 0
	if (disc < 0) //If discriminant is negative no intersection happens
	{
		//std::cout << "No intersection with sphere..." << std::endl;
		return false;
	}
	else //If discriminant is positive one or two intersections (two solutions) exists
	{
		float sqrt_disc = glm::sqrt(disc);
		t0 = (-b - sqrt_disc) / (2.0f * a);
		t1 = (-b + sqrt_disc) / (2.0f * a);

		if (t0 > t1)
		{
			// if t0 is bigger than t1 swap them around
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}

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
		}
	}

	r->intersectPoint = r->origin + r->direction*t0;
	r->dToObject = glm::length(r->intersectPoint - r->origin);
	return true;
}