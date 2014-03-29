#pragma once

#include "includes.h"
#include "Camera.h"
#include "NFFLoader.h"
#include "OutConverter.h"
#include "Ray.h"
#include <thread>

class Scene
{
	int _currentPixel, _id, _dpi, _width, _height;

	//Resolution: in pixels, in x and in y.
	glm::vec2 _RES;

	//BackgroundColor read from nff file.
	glm::vec3 _backgroundColor;

	//Vector with the number of lights read from the nff file.
	//Struct light defined in Includes.h.
	std::vector<light> _lights;
	

	//Camera that stores the folowing parameters:
	/*	From: the eye location in XYZ.
		At: a position to be at the center of the image, in XYZ world
		coordinates. A.k.a. "lookat".
		Up: a vector defining which direction is up, as an XYZ vector.
		Angle: in degrees, defined as from the center of top pixel row to
		bottom pixel row and left column to right column.
		Hither: distance of the hither plane (if any) from the eye. Mostly
		needed for hidden surface algorithms.	*/
	Camera *_c;
	std::vector<Geometry*> _geometry;

	//structure to store all the pixels after the rendering
	pixel *_pixels;
	int _maxDepth, _maxDiv;

public:
	Scene();

	void loadNFF(std::string filename);

	void partialSceneCalculation(int initX, int initY, float endX, float endY);
	void loadScene();

	glm::vec3 trace(std::vector<Geometry*> geometry, Ray* ray, int depth, bool refracted);

	glm::vec3 Scene::monteCarloSampling(int x, int y, glm::vec3* colors, int iter, int epsilon);

	void savebmp(const char *filename, int w, int h, int dpi, pixel *data);

	pixel* getPixels(){ return _pixels; };

	int getWidth(){ return _width; }

	int getHeight() { return _height; }

	int getDpi(){ return _dpi; }

};