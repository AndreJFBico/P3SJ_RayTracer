#pragma once

#include "includes.h"
#include "Ray.h"
#include "BoundingBox.h"

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

	BoundingBox _boundingBox;

	Geometry();

	BoundingBox getBBox(){ return _boundingBox; }
	virtual bool intersect(Ray *r) = 0;
	virtual glm::vec3 calculateNormal(Ray* r) = 0;
	virtual void computeBoundingBox();
};