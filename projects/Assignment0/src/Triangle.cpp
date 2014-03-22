#include "Triangle.h"

Triangle::Triangle() : Geometry()
{
	
}

bool Triangle::intersect(Ray *r)
{
	//Calculating normal.
	glm::vec3 N = glm::cross((_vertexes.at(1) - _vertexes.at(0)), (_vertexes.at(2)- _vertexes.at(1)));
	int i0, i1, i2;
	//Calculating indices.
	if (N.x > N.y && N.x > N.z)
	{
		i0 = 0;
		i1 = 1;
		i2 = 2;
	}
	else if (N.y > N.x && N.y > N.z)
	{
		i1 = 0;
		i0 = 1;
		i2 = 2;
	}
	else if (N.z > N.x && N.z > N.y)
	{
		i1 = 0;
		i2 = 1;
		i0 = 2;
	}

	glm::vec3 O = r->origin;
	glm::vec3 D = r->direction;
	glm::vec3 P = r->intersectPoint;
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
		return true;
	return false;
}

glm::vec3 Triangle::calculateNormal(Ray* r)
{
	return glm::vec3(0.0, 0.0, 0.0);
}
