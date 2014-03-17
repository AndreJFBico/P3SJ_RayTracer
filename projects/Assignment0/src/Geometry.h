#pragma once

#include "includes.h"
#include "Ray.h"

class Geometry
{
public:
	//Material information.
	glm::vec3 _RGB;
	float _Kd;
	float _Ks; 
	float _Shine;
	//Transmitance.
	float _T;
	float _refract_index;
	float _id;

	Geometry();

	virtual bool intersect(Ray *r) = 0;
	virtual glm::vec3 calculateNormal(Ray* r) = 0;
};