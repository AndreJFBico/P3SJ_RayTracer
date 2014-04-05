#pragma once

#include "Ray.h"
#include "Structs.h"

class NotObjects
{
public:
	NotObjects();

	virtual intersectVal intersect(Ray *r) = 0;

	~NotObjects();
};

