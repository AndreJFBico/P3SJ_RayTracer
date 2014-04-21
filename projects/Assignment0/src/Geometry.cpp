#include "Geometry.h"

Geometry::Geometry()
{
	_boundingBox = new BoundingBox();
}

Geometry::~Geometry()
{
	delete _boundingBox;
}