#include "Camera.h"

Camera::Camera()
{

}

void Camera::CameraInit()
{
	_Ze = glm::normalize(_from - _at);
	_Xe = glm::normalize(glm::cross(_up, _Ze));
	_Ye = glm::normalize(glm::cross(_Ze, _Xe));

	_h = 2 * tan((45 * PI / 180) / 2)*(_from - _at).length();
	_w = _h * 512 / 512;
}