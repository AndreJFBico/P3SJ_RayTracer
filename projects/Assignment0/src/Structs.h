#pragma once
class Ray;
class Geometry;

struct intersectVal
{
	bool intersected;
	Geometry* nearest;

	intersectVal(bool i, Geometry* n) : intersected(i), nearest(n){}
};

struct rayPos
{
	Ray* ray;
	/*-----*/
	glm::vec3 pos;
	/*-----*/
	rayPos(Ray * r, glm::vec3 p) : ray(r), pos(p){}
};

struct lightRays
{
	int l;
	/*-----*/
	std::vector<rayPos> rays;
	/*-----*/
	lightRays(int i, std::vector<rayPos> r) : l(i), rays(r){}
};