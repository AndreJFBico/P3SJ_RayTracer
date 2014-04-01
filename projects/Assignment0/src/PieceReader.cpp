#include "PieceReader.h"

void PieceReader::init()
{
	hasTex = false;
}

void PieceReader::readObject(std::string fpath)
{
	vertices = new std::vector<Vertex>();
	indices = new std::vector<unsigned int>();
	std::vector<glm::vec4>* normals = new std::vector<glm::vec4>();
	std::vector<glm::vec4>* verts = new std::vector<glm::vec4>();
	std::vector<glm::vec2>* text = new std::vector<glm::vec2>();
	int k = 0;

	std::ifstream in(fpath, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << fpath << std::endl; exit(1); }
	hasTex = false;
	std::string line;
	while (std::getline(in, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream s(line.substr(2));
			glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			verts->push_back(v);
		}
		else if (line.substr(0, 3) == "vn ") {
			std::istringstream s(line.substr(2));
			glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			normals->push_back(v);
		}
		else if (line.substr(0, 3) == "vt ") {
			std::istringstream s(line.substr(2));
			glm::vec2 v; s >> v.x; s >> v.y;
			text->push_back(v);
			
			//TODO Comented due to faces without texture causing problems
			//hasTex = true;
		}
		else if (line.substr(0, 2) == "f ") {
			std::istringstream s(line.substr(2));
			unsigned short int a, b, c;
			Vertex v;

			s >> a;
			a--;
			v.XYZW = verts->at(a);
			indices->push_back(k++);
			v.RGBA = glm::vec4(1.0f);
			
			s.ignore(256, '/');
			if (s.peek() != '/')
			{
				s >> a;
				a--;
				v.UV = text->at(a);
			}

			s.ignore(256, '/');
			s >> a;
			a--;
			v.NORMAL = normals->at(a);
			vertices->push_back(v);

			s.ignore(256, ' ');
			s >> b;
			b--;
			v.XYZW = verts->at(b);
			indices->push_back(k++);
			v.RGBA = glm::vec4(1.0f);

			s.ignore(256, '/');
			if (s.peek() != '/')
			{
				s >> b;
				b--;
				v.UV = text->at(b);
			}

			s.ignore(256, '/');
			s >> b;
			b--;
			v.NORMAL = normals->at(b);
			vertices->push_back(v);

			s.ignore(256, ' ');
			s >> c;
			c--;
			v.XYZW = verts->at(c);
			indices->push_back(k++);
			v.RGBA = glm::vec4(1.0f);

			s.ignore(256, '/');
			if (s.peek() != '/')
			{
				s >> c;
				c--;
				v.UV = text->at(c);
			}

			s.ignore(256, '/');
			s >> c;
			c--;
			v.NORMAL = normals->at(c);

			s.ignore(256, ' ');
			v.TANG = glm::vec4(0.0, 0.0, 0.0, 0.0);
			vertices->push_back(v);
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}
}

std::vector<Vertex> PieceReader::getVertices()
{
	return *vertices;
}

std::vector<unsigned int> PieceReader::getIndices()
{
	return *indices;
}

bool PieceReader::getHasTex()
{
	return hasTex;
}

void PieceReader::clearAll()
{
	vertices->clear();
	indices->clear();
	hasTex = false;
}

PieceReader::~PieceReader()
{
}
