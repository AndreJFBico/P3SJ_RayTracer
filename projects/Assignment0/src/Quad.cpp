#include "Quad.h"

Quad::Quad() : Geometry()
{
	_boundingBox = *(new BoundingBox());
}

intersectVal Quad::intersect(Ray *r)
{
	normal = glm::cross(_vertexes[0] - _vertexes[1], _vertexes[1] - _vertexes[2]);
	glm::normalize(normal);
	static const float eps = 10e-06;

	float t, u, v;

	glm::vec3 E_01 = _vertexes[1] - _vertexes[0];
	glm::vec3 E_03 = _vertexes[3] - _vertexes[0];

	glm::vec3 P = glm::cross(r->direction, E_03);

	float det = glm::dot(E_01, P);

	if (std::abs(det) < eps)
		return intersectVal(false, NULL);
	
	glm::vec3 T = r->origin - _vertexes[0];
	float alpha = glm::dot(T, P) / det;

	if (alpha < float(0.0))
		return intersectVal(false, NULL);

	glm::vec3 Q = glm::cross(T, E_01);
	float beta = glm::dot(r->direction, Q) / det;

	if (beta < float(0.0))
		return intersectVal(false, NULL);

	if ((alpha + beta) > float(1.0f))
	{
		glm::vec3 E_23 = _vertexes[3] - _vertexes[2];
		glm::vec3 E_21 = _vertexes[1] - _vertexes[2];
		glm::vec3 P_prime = glm::cross(r->direction, E_21);
		float det_prime = glm::dot(E_23, P_prime);

		if (std::abs(det_prime) < eps)
			return intersectVal(false, NULL);

		glm::vec3 T_prime = r->origin - _vertexes[2];
		float alpha_prime = glm::dot(T_prime, P_prime) / det_prime;

		if (alpha_prime < float(0.0))
			return intersectVal(false, NULL);

		glm::vec3 Q_prime = glm::cross(T_prime, E_23);
		float beta_prime = glm::dot(r->direction, Q_prime) / det_prime;
		if (beta_prime < float(0.0))
			return intersectVal(false, NULL);
	}

	t = glm::dot(E_03, Q) / det;
	if (t < float(0.0))
		return intersectVal(false, NULL);

	float alpha_11, beta_11;
	glm::vec3 E_02 = _vertexes[2] - _vertexes[0];
	glm::vec3 n = glm::cross(E_01, E_03);

	if ((std::abs(n.x)) >= std::abs(n.y) && 
		(std::abs(n.x) >= std::abs(n.z)))
	{
		alpha_11 = ((E_02.y * E_03.z) - (E_02.z * E_03.y)) / n.x;
		beta_11 = ((E_01.y * E_02.z) - (E_01.z * E_02.y)) / n.x;
	}
	else
	if ((std::abs(n.y)) >= std::abs(n.x) &&
		(std::abs(n.y) >= std::abs(n.z)))
	{
		alpha_11 = ((E_02.z * E_03.x) - (E_02.x * E_03.z)) / n.y;
		beta_11 = ((E_01.z * E_02.z) - (E_01.x * E_02.y)) / n.y;
	}
	else
	{
		alpha_11 = ((E_02.x * E_03.y) - (E_02.y * E_03.x)) / n.z;
		beta_11 = ((E_01.x * E_02.y) - (E_01.y * E_02.x)) / n.z;
	}

	if (std::abs(alpha_11 - float(1.0)) < eps)
	{
		u = alpha;
		if (std::abs(beta_11 - float(1.0)) < eps)
			v = beta;//parallelogram
		else v = beta / ((u * (beta_11 - float(1.0))) + double(1.0));//trapezium
	}
	else if (std::abs(beta_11 - double(1.0)) < eps)
	{
		v = beta;
		u = alpha / ((v * (alpha_11 - double(1.0))) + double(1.0));

	}
	else
	{
		float A = float(1.0) - beta_11;
		float B = (alpha * (beta_11 - float(1.0)))
			- (beta * (alpha_11 - float(1.0))) - float(1.0);
		float C = alpha;
		float D = (B * B) - (float(4.0) * A * C);
		float Q = float(-0.5) * (B + ((B < float(0.0) ? float(-1.0) : float(1.0))
			* std::sqrt(D)));
		u = Q / A;
		if ((u < float(0.0)) || (u > float(1.0)))
			u = C / Q;
		v = beta / ((u * (beta_11 - float(1.0))) + float(1.0));
	}
	r->t = t;
	r->intersectPoint = r->origin + r->direction*t;
	r->dToObject = glm::length(r->intersectPoint - r->origin);
	return intersectVal(true, this);
}

glm::vec3 Quad::calculateNormal(Ray *r)
{
	return glm::normalize(normal);
}

void Quad::computeBoundingBox()
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