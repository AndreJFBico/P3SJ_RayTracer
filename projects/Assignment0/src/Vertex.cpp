#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(glm::vec4 xyzw, glm::vec4 rgba, glm::vec4 normal, glm::vec2 uv)
{
	XYZW = xyzw;
	RGBA = rgba;
	NORMAL = normal;
	UV = uv;
}