#pragma once

#include "includes.h"
#include "Camera.h"

class Scene
{
	int currentPixel;

	//Resolution: in pixels, in x and in y.
	int RES_X, RES_Y;

	//BackgroundColor read from nff file.
	glm::vec3 backgroundColor;

	//Vector with the number of lights read from the nff file.
	//Struct light defined in Includes.h.
	std::vector<light> lights;

	//Camera that stores the folowing parameters:
	/*	From: the eye location in XYZ.
		At: a position to be at the center of the image, in XYZ world
		coordinates. A.k.a. "lookat".
		Up: a vector defining which direction is up, as an XYZ vector.
		Angle: in degrees, defined as from the center of top pixel row to
		bottom pixel row and left column to right column.
		Hither: distance of the hither plane (if any) from the eye. Mostly
		needed for hidden surface algorithms.	*/
	Camera *c;

public:
	Scene();

	void loadNFF(const char *filename);

	void drawScene();

	void savebmp(const char *filename, int w, int h, int dpi, pixel *data);
};