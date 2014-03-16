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

	glm::vec3 _Ze;
	glm::vec3 _Ye;
	glm::vec3 _Xe;
	float _h, _w;

	Camera();
	void CameraInit();

};