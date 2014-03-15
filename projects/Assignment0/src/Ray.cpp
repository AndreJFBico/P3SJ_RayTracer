#include "Ray.h"


Ray::Ray()
{
	direction = glm::vec3(0.0);
}

glm::vec3 Ray::calculateWCS(glm::vec2 pixel, glm::vec3 eye)
{
	return direction;
}

Ray::~Ray()
{
}
