#include "Ray.h"


Ray::Ray()
{
	direction = glm::vec3(0.0);
}

glm::vec3 Ray::calculateWCS(glm::vec2 pixel, glm::vec3 at, glm::vec3 eye, glm::vec3 up)
{
	glm::vec3 Ze;
	glm::vec3 Ye;
	glm::vec3 Xe;
	float u, v;
	glm::vec3 o;
	glm::vec3 p;
	float h, w;
	const float  PI = 3.14159265358979f;

	Ze = glm::normalize(eye - at);
	Xe = glm::normalize(glm::cross(up, Ze));
	Ye = glm::normalize(glm::cross(Ze, Xe));

	h = 2 * tan((45*PI / 180) / 2)*(eye - at).length();
	w = h * 512 / 512;

	o = eye;
	u = (w * pixel.x) / 511;
	v = (h * pixel.y) / 511;

	p = o + Xe*u + Ye*v;
	std::cout << p.x << "  " << p.y << std::endl;

	return direction;
}

Ray::~Ray()
{
}
