#pragma once

#include "Includes.h"
#include "Geometry.h"

class Cell
{
	std::vector<Geometry*> _objects;

public:
	Cell();

	
	
	virtual ~Cell();
};

