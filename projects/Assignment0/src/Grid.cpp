#include "Grid.h"

Grid::Grid(int m)
{
	_mFactor = m;
}

void Grid::computeBbox(std::vector<Geometry*> objs)
{
	_numObjects = objs.size();
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
	objBbox.min = p0;
	objBbox.max = p1;

	_bbox = objBbox;
}

void Grid::cellsSetup()
{
	glm::vec3 w = _bbox.max - _bbox.min;
	float s = cbrt(w.x * w.y * w.z / _numObjects);
	_nx = _mFactor*w.x / s + 1;
	_ny = _mFactor*w.y / s + 1;
	_nz = _mFactor*w.z / s + 1;

	_numCells = _nx*_ny*_nz;
	for (int i = 0; i < _numCells; i++)
		_cells.push_back(new Cell());
}

int Grid::getCellArrayIndex(int ix, int iy, int iz)
{
	return ix + _nx * iy + _nx * _ny * iz;
}

float clamp(float x, float min, float max)
{
	return fmin(fmax(x, min), max);
}

void Grid::cellObjectAttribution(std::vector<Geometry*> geo)
{
	for each (Geometry* g in geo)
	{
		//Cell c1 = _cells[getCellArrayIndex(g->getBBox().min.x, g->getBBox().min.y, g->getBBox().min.z)];
		//Cell c2 = _cells[getCellArrayIndex(g->getBBox().max.x, g->getBBox().max.y, g->getBBox().max.z)];

		int ixmin = clamp((g->_boundingBox.min.x - _bbox.min.x) * _nx / (_bbox.max.x - _bbox.min.x), 0, _nx - 1);
		int iymin = clamp((g->_boundingBox.min.y - _bbox.min.y) * _ny / (_bbox.max.y - _bbox.min.y), 0, _ny - 1);
		int izmin = clamp((g->_boundingBox.min.z - _bbox.min.z) * _nz / (_bbox.max.z - _bbox.min.z), 0, _nz - 1);
		int ixmax = clamp((g->_boundingBox.max.x - _bbox.min.x) * _nx / (_bbox.max.x - _bbox.min.x), 0, _nx - 1);
		int iymax = clamp((g->_boundingBox.max.y - _bbox.min.y) * _ny / (_bbox.max.y - _bbox.min.y), 0, _ny - 1);
		int izmax = clamp((g->_boundingBox.max.z - _bbox.min.z) * _nz / (_bbox.max.z - _bbox.min.z), 0, _nz - 1);

		/*int temp = 0;
		if (ixmin > ixmax) temp = ixmin, ixmin = ixmax, ixmax = temp;
		if (iymin > iymax) temp = iymin, iymin = iymax, iymax = temp;
		if (izmin > izmax) temp = izmin, izmin = izmax, izmax = temp;
		*/

		for (int iz = izmin; iz <= izmax; iz++){
			for (int iy = iymin; iy <= iymax; iy++){
				for (int ix = ixmin; ix <= ixmax; ix++){
					_cells[getCellArrayIndex(ix, iy, iz)]->addObj(g);
				}
			}
		}
	}
}

intersectVal Grid::intersect(Ray* r)
{
	int ix, iy, iz;
	Cell* intersectedcell;
	if (!(_bbox.intersect(r).intersected))
		return intersectVal(false, NULL);
	r->intersectedGrid = true;
	if (_bbox._tminf < 0)
		r->insideGrid = true;
	if (r->insideGrid){
		ix = clamp((r->origin.x - _bbox.min.x) * _nx / (_bbox.max.x - _bbox.min.x), 0, _nx - 1);
		iy = clamp((r->origin.y - _bbox.min.y) * _ny / (_bbox.max.y - _bbox.min.y), 0, _ny - 1);
		iz = clamp((r->origin.z - _bbox.min.z) * _nz / (_bbox.max.z - _bbox.min.z), 0, _nz - 1);
	}
	else{
		ix = clamp((r->intersectPoint.x - _bbox.min.x) * _nx / (_bbox.max.x - _bbox.min.x), 0, _nx - 1);
		iy = clamp((r->intersectPoint.y - _bbox.min.y) * _ny / (_bbox.max.y - _bbox.min.y), 0, _ny - 1);
		iz = clamp((r->intersectPoint.z - _bbox.min.z) * _nz / (_bbox.max.z - _bbox.min.z), 0, _nz - 1);
	}
	
	std::vector<Geometry*> intersected = gridTraversal(ix, iy, iz, r);
	Geometry* nearest = NULL;
	float prevD2Obj = (float)INT_MAX;
	glm::vec3 closestintersect = glm::vec3(0.0f, 0.0f, 0.0f);

	if (intersected.size() > 0)
		for each (Geometry* g in intersected)
		{
			intersectVal v = g->intersect(r);
			if (v.intersected)
			{
				//checks if object is closer
				if (r->dToObject < prevD2Obj)
				{
					closestintersect = r->intersectPoint;
					nearest = v.nearest;
					prevD2Obj = r->dToObject;
				}
			}
		}
	else{
		r->intersectedGrid = false;
		return intersectVal(false, NULL);
	}
	r->intersectPoint = closestintersect;
	r->dToObject = prevD2Obj;

	if (nearest == NULL)
		return intersectVal(false, NULL);
	else
		return intersectVal(true, nearest);
	
}

std::vector<Geometry*> Grid::gridTraversal(int ix, int iy, int iz, Ray* r)
{
	//std::vector<Geometry*> intersected;
	float justOutX = _nx;
	float justOutY = _ny;
	float justOutZ = _nz;

	float tDeltaX = (_bbox._tmax.x - _bbox._tmin.x) / _nx;
	float tDeltaY = (_bbox._tmax.y - _bbox._tmin.y) / _ny;
	float tDeltaZ = (_bbox._tmax.z - _bbox._tmin.z) / _nz;

	float tx_next = _bbox._tmin.x + (ix + 1) + tDeltaX;
	float ty_next = _bbox._tmin.y + (iy + 1) + tDeltaY;
	float tz_next = _bbox._tmin.z + (iz + 1) + tDeltaZ;

	float stepX = 1;
	float stepY = 1;
	float stepZ = 1;
	
	bool hit;
	Ray ray;
	std::vector<Geometry*> intersected;
	std::vector<Geometry*> objs;

	while (true)
	{
		ray = *r;
		hit = false;
		objs = _cells[getCellArrayIndex(ix, iy, iz)]->getObjs();
		
		
		if (tx_next < ty_next && tx_next < tz_next)
		{
			if (objs.size()){
				for each (Geometry* g in objs)
				{
					intersectVal v = g->intersect(&ray);
					if (v.intersected)
					{
						if (ray.intersectPoint.x < tx_next){
							intersected.push_back(v.nearest);
							hit = true;
						}
					}
				}
				if (hit)
					return intersected;
			}
			tx_next += tDeltaX;
			ix += stepX;

			if (ix == justOutX)
				return intersected;
		}

		else if (ty_next < tz_next)
		{
			if (objs.size()){
				for each (Geometry* g in objs)
				{
					intersectVal v = g->intersect(&ray);
					if (v.intersected)
					{
						if (ray.intersectPoint.y < ty_next){
							intersected.push_back(v.nearest);
							hit = true;
						}
					}
				}
				if (hit)
					return intersected;
			}
			ty_next += tDeltaY;
			iy += stepY;

			if (iy == justOutY)
				return intersected;
		}

		else{
			if (objs.size()){
				for each (Geometry* g in objs)
				{
					intersectVal v = g->intersect(&ray);
					if (v.intersected)
					{
						if (ray.intersectPoint.z < tz_next){
							intersected.push_back(v.nearest);
							hit = true;
						}
					}
				}
				if (hit)
					return intersected;
			}
			tz_next += tDeltaZ;
			iz += stepZ;

			if (iz == justOutZ)
				return intersected;
		}
	}
	
	/*while (intersected.empty())
	{
		if (tx_next < ty_next) {
			if (tx_next < tz_next) {
				ix = ix + stepX;
				if (ix == justOutX)
					break; /* outside grid *
				tx_next = tx_next + tDeltaX;
			}
			else {
				iz = iz + stepZ;
				if (iz == justOutZ)
					break;
				tz_next = tz_next + tDeltaZ;
			}
		}
		else {
			if (ty_next < tz_next) {
				iy = iy + stepY;
				if (iy == justOutY)
					break;
				ty_next = ty_next + tDeltaY;
			}
			else {
				iz = iz + stepZ;
				if (iz == justOutZ)
					break;
				tz_next = tz_next + tDeltaZ;
			}
		}
		for each(Geometry* g in _cells[getCellArrayIndex(ix, iy, iz)]->getObjs())
		{
			intersected.push_back(g);
		}
	}
	
	return(intersected);*/
}