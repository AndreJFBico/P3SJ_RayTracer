#include "NFFLoader.h"

NFFLoader::NFFLoader()
{
	_id = 0;
}

void NFFLoader::load(std::string fpath)
{
	glm::vec3 RGB = glm::vec3(0.0);
	float Kd = 0.0;
	float Ks = 0.0;
	float Shine = 0.0;
	float T = 0.0;
	float refract_index = 0.0;

	std::ifstream in(fpath, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << fpath << std::endl; exit(1); }
	std::string line;

	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "b ")
		{
			std::istringstream s(line.substr(2));
			s >> _backgroundColor.r; s >> _backgroundColor.g; s >> _backgroundColor.b;
			//prints the result
			//std::cout << "BackgroundColor " << _backgroundColor.r << " " << _backgroundColor.g << " " << _backgroundColor.b << std::endl;
		}
		else
		if (line.substr(0, 1) == "v") {
			std::istringstream s(line.substr(1));
			_c = new Camera();
			std::cout << "v" << std::endl;
			while (std::getline(in, line))
			{
				if (line.substr(0, 5) == "from ")
				{
					std::istringstream s(line.substr(5));
					s >> _c->_from.x; s >> _c->_from.y; s >> _c->_from.z;
					//std::cout << "from " << _c->_from.x << " " << _c->_from.y << " " << _c->_from.z << std::endl;
				}
				else
				if (line.substr(0, 3) == "at ")
				{
					std::istringstream s(line.substr(3));
					s >> _c->_at.x; s >> _c->_at.y; s >> _c->_at.z;
					//std::cout << "at " << _c->_at.x << " " << _c->_at.y << " " << _c->_at.z << std::endl;
				}
				else
				if (line.substr(0, 3) == "up ")
				{
					std::istringstream s(line.substr(3));
					s >> _c->_up.x; s >> _c->_up.y; s >> _c->_up.z;
					//std::cout << "up " << _c->_up.x << " " << _c->_up.y << " " << _c->_up.z << std::endl;
				}
				else
				if (line.substr(0, 6) == "angle ")
				{
					std::istringstream s(line.substr(6));
					s >> _c->_angle;
					//std::cout << "angle " << _c->_angle << std::endl;
				}
				else
				if (line.substr(0, 7) == "hither ")
				{
					std::istringstream s(line.substr(7));
					s >> _c->_hither;
					//std::cout << "hither " << _c->_hither << std::endl;
				}
				else
				if (line.substr(0, 11) == "resolution ")
				{
					std::istringstream s(line.substr(11));
					s >> _RES.x; s >> _RES.y;
					//std::cout << "resolution " << _RES.x << " " << _RES.y << std::endl;
					break;
				}
			}
			_c->CameraInit();
		}
		else
		if (line.substr(0, 2) == "l ")
		{
			std::istringstream s(line.substr(2));
			light l;
			s >> l.XYZ.x; s >> l.XYZ.y; s >> l.XYZ.z;

			s >> l.RGB.r; s >> l.RGB.g; s >> l.RGB.b;
			//std::cout << "light " << l.XYZ.x << " " << l.XYZ.y << " " << l.XYZ.z
			//	<< l.RGB.r << " " << l.RGB.g << " " << l.RGB.b << std::endl;
			_lights.push_back(l);
		}
		else
		if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(2));
			RGB; s >> RGB.r; s >> RGB.g; s >> RGB.b;
			Kd; s >> Kd;
			Ks; s >> Ks;
			Shine; s >> Shine;
			//Transmitance.
			T; s >> T;
			refract_index; s >> refract_index;

			//std::cout << "f " << RGB.r << " " << RGB.g << " " << RGB.b <<
			//	" " << Kd << " " << Ks << " " << Shine << " " <<
			//	T << " " << refract_index << " " << std::endl;
		}
		else
		if (line.substr(0, 3) == "pl ")
		{
			std::istringstream s(line.substr(3));
			Plane *p = new Plane();
			glm::vec3 ver0, ver1, ver2;
			p->_RGB = RGB;
			p->_Kd = Kd;
			p->_Ks = Ks;
			p->_Shine = Shine;
			p->_T = T;
			p->_refract_index = refract_index;

			s >> ver0.x; s >> ver0.y >> ver0.z;
			s >> ver1.x; s >> ver1.y >> ver1.z;
			s >> ver2.x; s >> ver2.y >> ver2.z;

			p->_vertexes.push_back(ver0);
			p->_vertexes.push_back(ver1);
			p->_vertexes.push_back(ver2);

			//std::cout << "plane v0 " << ver0.x << " " << ver0.y << " " << ver0.z << std::endl;
			//std::cout << "plane v1 " << ver1.x << " " << ver1.y << " " << ver1.z << std::endl;
			//std::cout << "plane v2 " << ver2.x << " " << ver2.y << " " << ver2.z << std::endl;
			p->_id = _id;
			_id += 0.1f;
			_planes.push_back(p);
		}
		else
		if (line.substr(0, 2) == "s ")
		{
			std::istringstream s(line.substr(2));
			Sphere * S = new Sphere();
			glm::vec3 center;
			S->_RGB = RGB;
			S->_Kd = Kd;
			S->_Ks = Ks;
			S->_Shine = Shine;
			S->_T = T;
			S->_refract_index = refract_index;
			s >> center.x; s >> center.y; s >> center.z;
			S->_center = center;
			s >> S->_radius;
			//std::cout << "sphere " << S->_center.x << " " << S->_center.y << " " << S->_center.z << std::endl;
			//std::cout << "sphere radius " << S->_radius << std::endl;
			S->_id = _id;
			_id += 0.1f;
			_geometry.push_back(S);
		}
		else
		if (line.substr(0, 2) == "p ")
		{
			std::istringstream s(line.substr(2));
			int vertex_count = 0;
			Triangle * t = new Triangle();
			glm::vec3 vertex;
			t->_RGB = RGB;
			t->_Kd = Kd;
			t->_Ks = Ks;
			t->_Shine = Shine;
			t->_T = T;
			t->_refract_index = refract_index;
			s >> vertex_count;
			//std::cout << "Triangle " << vertex_count << std::endl;
			for (int i = 0; i < vertex_count; i ++)
			{
				std::getline(in, line);
				std::istringstream s(line);
				s >> vertex.x; s >> vertex.y; s >> vertex.z; 
				t->_vertexes.push_back(vertex);
				//std::cout << "triangle vertex " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
			}

			t->_id = _id;
			_id += 0.1f;
			_geometry.push_back(t);
		}
		else
		if (line.substr(0, 3) == "bb ")
		{
			std::istringstream s(line.substr(3));
			glm::vec3 min, max;
			AABox * b = new AABox();
			glm::vec3 vertex;
			b->_RGB = RGB;
			b->_Kd = Kd;
			b->_Ks = Ks;
			b->_Shine = Shine;
			b->_T = T;
			b->_refract_index = refract_index;
			s >> min.x, s >> min.y, s >> min.z , s >> max.x, s >> max.y, s >> max.z;
			b->min = min;
			b->max = max;
			std::cout << "bounding box: " << min.x << " " << min.y << " "<< min.z << " " << max.x << " " << max.y << " " << max.z << std::endl;

			b->_id = _id;
			_id += 0.1f;
			_geometry.push_back(b);
		}
	}
}

glm::vec2 NFFLoader::getRes()
{
	return _RES;
}

glm::vec3 NFFLoader::getBackgroundColor()
{
	return _backgroundColor;
}

std::vector<light> NFFLoader::getLight()
{
	return _lights;
}

Camera * NFFLoader::getCamera()
{
	return _c;
}

std::vector<Geometry*> NFFLoader::getGeometry()
{
	return _geometry;
}

std::vector<NotObjects*> NFFLoader::getPlanes()
{
	return _planes;
}

void NFFLoader::clear()
{
	_RES = glm::vec2(0.0);
	_backgroundColor = glm::vec3(0.0);
	_lights.clear();
	_c = NULL;
}

NFFLoader::~NFFLoader()
{
}
