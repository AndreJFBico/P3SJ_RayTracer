#include "Ray.h"


Ray::Ray()
{
	direction = glm::vec3(0.0);
	dToObject = 0;
	shadowfillertype = true; 
	refractionIndex = 1.0f;
	insideGrid = false;
	intersectedGrid = false;
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

Ray* Ray::reflect(glm::vec3 normal)
{
	Ray* rRay = new Ray();
	const float ERR = 0.003f;
	rRay->direction = direction - 2.0f * (normal*(glm::dot(normal, direction)));
	rRay->direction = glm::normalize(rRay->direction);
	rRay->origin = intersectPoint + rRay->direction *ERR;

	return rRay;
}

Ray* Ray::refract(glm::vec3 normal, float refract)
{
	Ray* rRay = new Ray();

	glm::vec3 vT = (glm::dot(-direction, normal))*normal - (-direction);
	float senThetaI = glm::length(vT);
	float senThetaT = (refractionIndex / refract) * senThetaI;
	float cosThetaT = 0.0;
	if (senThetaT*senThetaT < 1.0f)
		cosThetaT = sqrt(1 - (senThetaT*senThetaT));
	glm::vec3 t = glm::normalize(vT);

	const float ERR = 0.003f;
	rRay->direction = senThetaT * t + cosThetaT * (-normal);
	rRay->direction = glm::normalize(rRay->direction);
	rRay->origin = intersectPoint + (rRay->direction) *ERR;
	rRay->refractionIndex = refract;
	

	return rRay;
}

Ray::~Ray()
{
}
