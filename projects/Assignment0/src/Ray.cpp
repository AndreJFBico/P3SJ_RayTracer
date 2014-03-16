#include "Ray.h"


Ray::Ray()
{
	direction = glm::vec3(0.0);
	dToObject = 0;
	shadowfillertype = true;
}

glm::vec3 Ray::calculateWCS(
	glm::vec2 pixel, glm::vec3 at, 
	glm::vec3 eye, glm::vec3 up, 
	float w, float h,
	glm::vec3 Ze, glm::vec3 Xe,
	glm::vec3 Ye)
{
	float u, v;
	glm::vec3 o;
	glm::vec3 p;

	o = eye;
	u = (w * pixel.x) / 511;
	v = (h * pixel.y) / 511;

	p = o + Xe*u + Ye*v;
	//std::cout << p.x << "  " << p.y << std::endl;

	float a = glm::length(eye - at);
	direction = -a*Ze + h*((pixel.y / 511) - 0.5f)*Ye + w*((pixel.x / 511) - 0.5f)*Xe;
	direction = glm::normalize(direction);
	origin = eye;
	return direction;
}

void Ray::trace()
{

}

Ray::~Ray()
{
}
