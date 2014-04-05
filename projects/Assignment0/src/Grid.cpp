#include "Grid.h"

Grid::Grid(int m)
{
	_mFactor = m;
}

void Grid::computeBbox(std::vector<Geometry*> objs){
	
	BoundingBox objBbox;
	glm::vec3 p0 = glm::vec3(HUGE_VALUE);
	glm::vec3 p1 = glm::vec3(SMALL_VALUE);
	
	for (int i = 0; i < _numObjects; i++){
		objBbox = objs[i]->getBBox();

		if(objBbox.min.x < p0.x)
			p0.x = objBbox.min.x;
		if(objBbox.min.y < p0.y)
			p0.y = objBbox.min.y;
		if(objBbox.min.z < p0.z)
			p0.z = objBbox.min.z;
	}

	p0.x -= KEPSILON; p0.y -= KEPSILON; p0.z -= KEPSILON;

	for (int i = 0; i < _numObjects; i++){
		objBbox = objs[i]->getBBox();

		if(objBbox.max.x > p1.x)
			p1.x = objBbox.max.x;
		if(objBbox.max.y > p1.y)
			p1.y = objBbox.max.y;
		if(objBbox.max.z > p1.z)
			p1.z = objBbox.max.z;
	}

	p1.x -= KEPSILON; p1.y -= KEPSILON; p1.z -= KEPSILON;

	
}