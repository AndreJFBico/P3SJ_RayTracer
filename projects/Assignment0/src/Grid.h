#pragma once

#include "includes.h"
#include "Cell.h"
#include "BoundingBox.h"
#include "NotObjects.h"

class Grid : public NotObjects
{
	Cell *_cells;
	BoundingBox _bbox;
	int _numCells, _numObjects, _mFactor;

	public:

		Grid(int m);
		void computeBbox(std::vector<Geometry*> geo);
		void cellsSetup();
		void gridTraversal();

		//GETTERS
		int getNumCells(){ return _numCells; }
		Cell *getCells(){ return _cells; }
		BoundingBox getBbox(){ return _bbox; }

};