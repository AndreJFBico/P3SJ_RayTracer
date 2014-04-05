#include "Cell.h"


Cell::Cell()
{
}

void Cell::addObj(Geometry* geo)
{
	_objects.push_back(geo);
}

Cell::~Cell()
{
}
