///////////////////////////////////////////////////////////////////////
//	P3SJ - Assignment 2
// 
//
///////////////////////////////////////////////////////////////////////
#include "Includes.h"
#include "Scene.h"

int main(int argc, char* argv[])
{
	Scene *s = new Scene();

	//Parses NFF file information.
	s->loadNFF("jap.nff");

	//Renders the Scene.
	s->drawScene();

	return 0;
}