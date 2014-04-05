#pragma once

#include "includes.h"
#include "Cell.h"
#include "BoundingBox.h"
#include "NotObjects.h"

class Grid : public NotObjects
{
	std::vector<Cell*> _cells;
	BoundingBox _bbox;
	int _numCells, _numObjects, _mFactor;
	int _nx, _ny, _nz;
	std::vector<Geometry*> _intersected;

	public:

		Grid(int m);
		void computeBbox(std::vector<Geometry*> geo);
		void cellsSetup();
		void cellObjectAttribution(std::vector<Geometry*> geo);
		std::vector<Geometry*> gridTraversal(int ix, int iy, int iz);
		intersectVal intersect(Ray* r);
		int getCellArrayIndex(int ix, int iy, int iz);

		//GETTERS
		int getNumCells(){ return _numCells; }
		std::vector<Cell*> getCells(){ return _cells; }
		BoundingBox getBbox(){ return _bbox; }

};