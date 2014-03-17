#include "Sphere.h"

Sphere::Sphere() : Geometry()
{
	
}

bool Sphere::intersect(Ray *r)
{
	//Squared distance between ray origin and sphere center
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
	float b = 2.0f*glm::dot(r->origin - _center, r->direction); // b = 2(o-C)*d
	float c = glm::dot(r->origin - _center, r->origin - _center) - (_radius * _radius); // c = (o-C)*(o-C)-R^2

	//Calculate discriminant
	float disc = (b*b) - (4.0f*a*c);

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
	}

	//If the second intersection has a negative value then the intersections
	//happen behind the ray origin which is not considered. Otherwise t0 is
	//the intersection to be considered
	if (t1<0)
	{
		//std::cout << "No intersection with sphere..." << std::endl;
		return false;
	}
	else if (t0 > 0)
	{
		//std::cout << "Intersection with sphere..." << std::endl;
		r->intersectPoint = r->origin + r->direction*t0;
		r->dToObject = glm::length(r->intersectPoint - r->origin);
		return true;
	}
	else
	{
		//std::cout << "Intersection with sphere..." << std::endl;
		r->intersectPoint = r->origin + r->direction*t1;
		r->dToObject = glm::length(r->intersectPoint - r->origin);
		return true;
	}
}

glm::vec3 Sphere::calculateNormal(Ray* r)
{
	glm::vec3 normal = r->intersectPoint - _center;
	return glm::normalize(normal);
}