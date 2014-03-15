#pragma once

#include "includes.h"

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

	Geometry();

	virtual void calcIntersect() = 0;
};