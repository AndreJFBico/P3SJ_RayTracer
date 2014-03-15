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

	//Renders the Scene and draws the result to a bmp file for the moment.
	//NOTE: in the future a render window must be used!
	s->drawScene();

	return 0;
}