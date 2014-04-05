#include "Triangle.h"

Triangle::Triangle() : Geometry()
{
	_boundingBox = *(new BoundingBox());
}

bool Triangle::intersect(Ray *r)
{
	//Calculating normal.
	normal = glm::cross(_vertexes[0] - _vertexes[1], _vertexes[1] - _vertexes[2]);
	glm::normalize(normal);

	float d = glm::dot(_vertexes[1] - r->origin, normal) / glm::dot(r->direction, normal);

	glm::vec3 P = r->origin + r->direction*d;
	
	if (d<0)
		return false;


	int i0, i1, i2;
	//Calculating indices.
	if (normal.x > normal.y && normal.x > normal.z)
	{
		i0 = 0;
		i1 = 1;
		i2 = 2;
	}
	else if (normal.y > normal.x && normal.y > normal.z)
	{
		i1 = 0;
		i0 = 1;
		i2 = 2;
	}
	else if (normal.z > normal.x && normal.z > normal.y)
	{
		i1 = 0;
		i2 = 1;
		i0 = 2;
	}

	glm::vec3 O = r->origin;
	glm::vec3 D = r->direction;
	
	//_vertexes

	glm::vec3 u, v;
	u[0] = P[i1] - (_vertexes.at(0))[i1];
	v[0] = P[i2] - (_vertexes.at(0))[i2];
	u[1] = (_vertexes.at(1))[i1] - (_vertexes.at(0))[i1];
	u[2] = (_vertexes.at(2))[i1] - (_vertexes.at(0))[i1];
	v[1] = (_vertexes.at(1))[i2] - (_vertexes.at(0))[i2];
	v[2] = (_vertexes.at(2))[i2] - (_vertexes.at(0))[i2];

	float Beta = 0.0f, Alfa = 0.0f;

	if (u[1] == 0.0f)
	{
		Beta = u[0] / u[2];
		if (Beta <= 1.0f && Beta >= 0.0f)
		{
			Alfa = (v[0] - Beta*v[2]) / v[1];
		}
	}
	else {
		Beta = (v[0]*u[1] - u[0]*v[1]) / (v[2]*u[1]- u[2]*v[1]);
		if (Beta <= 1.0f && Beta >= 0.0f)
		{
			Alfa = (u[0] - Beta*u[2]) / u[1];
		}
	}
	if (Beta >= 0.0f && Alfa >= 0.0f && (Alfa + Beta) <= 1)
	{
		r->intersectPoint = P;
		r->dToObject = glm::length(r->intersectPoint - r->origin);
		return true;
	}
	return false;
}

glm::vec3 Triangle::calculateNormal(Ray* r)
{
	return glm::normalize(normal);
}

void Triangle::computeBoundingBox()
{
	glm::vec3 localmin = _vertexes[0];
	glm::vec3 localmax = _vertexes[0];

	for each (glm::vec3 vertex in _vertexes)
	{
		if (vertex.x < localmin.x)
			localmin.x = vertex.x;
		else
			if (vertex.x > localmax.x)
				localmax.x = vertex.x;
		if (vertex.y < localmin.y)
			localmin.y = vertex.y;
		else
			if (vertex.y > localmax.y)
				localmax.y = vertex.y;
		if (vertex.z < localmin.z)
			localmin.z = vertex.z;
		else
			if (vertex.z > localmax.z)
				localmax.z = vertex.z;

	}
	_boundingBox.min = localmin;
	_boundingBox.max = localmax;
}