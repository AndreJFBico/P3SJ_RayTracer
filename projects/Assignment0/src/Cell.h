#pragma once

#include "Includes.h"
#include "Geometry.h"

class Cell
{
	std::vector<Geometry*> _objects;

public:
	Cell();

	void addObj(Geometry* geo);
	std::vector<Geometry*> getObjs() { return _objects; }
	
	~Cell();
};

