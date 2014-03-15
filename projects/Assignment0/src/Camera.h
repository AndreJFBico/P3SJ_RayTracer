#pragma once

#include "includes.h"

class Camera
{
public:
	glm::vec3 _from;
	glm::vec3 _at;
	glm::vec3 _up;
	float _angle;
	float _hither;

	Camera();

};