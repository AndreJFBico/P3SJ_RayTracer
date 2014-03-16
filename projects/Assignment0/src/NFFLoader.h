#pragma once

#include "includes.h"
#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"

class NFFLoader
{
	//Float representing Geometry id.
	float _id;

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

	NFFLoader();
	NFFLoader(NFFLoader const&);

public:
	static NFFLoader& getInstance()
	{
		static NFFLoader instance;
		return instance;
	}

	void load(std::string fpath);
	
	glm::vec2 getRes();
	glm::vec3 getBackgroundColor();
	std::vector<light> getLight();
	Camera * getCamera();
	std::vector<Geometry*> getGeometry();

	void clear();

	virtual ~NFFLoader();
};

