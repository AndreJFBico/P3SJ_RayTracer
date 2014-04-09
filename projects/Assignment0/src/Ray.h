#pragma once

#include "Includes.h"

class Ray
{

public:
	glm::vec3 direction;
	glm::vec3 origin;
	float dToObject;
	glm::vec3 intersectPoint;
	bool shadowfillertype;
	bool insideGrid;
	float refractionIndex;
	bool intersectedGrid;
	float t;

	Ray();

	//receives pixel coord and eye coord and calculates and returns ray direction vector
	glm::vec3 calculateWCS(
		glm::vec2 pixel, glm::vec3 at,
		glm::vec3 eye, glm::vec3 up,
		float w, float h,
		glm::vec3 Ze, glm::vec3 Xe,
		glm::vec3 Ye);

	void trace();

	Ray* reflect(glm::vec3 normal);
	Ray* refract(glm::vec3 normal, float refract);

	virtual ~Ray();
};

