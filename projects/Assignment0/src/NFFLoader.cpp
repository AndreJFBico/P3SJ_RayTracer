#include "NFFLoader.h"

void NFFLoader::load(std::string fpath)
{
	std::ifstream in(fpath, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << fpath << std::endl; exit(1); }
	std::string line;

	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "b ") {
			std::istringstream s(line.substr(2));
			s >> _backgroundColor.r; s >> _backgroundColor.g; s >> _backgroundColor.b;
			//prints the result
			std::cout << "BackgroundColor " << _backgroundColor.r << " " << _backgroundColor.g << " " << _backgroundColor.b << std::endl;
		}
		else
			if (line.substr(0, 1) == "v") {
				std::istringstream s(line.substr(1));
				_c = new Camera();
				std::cout << "v" << std::endl;
				while (std::getline(in, line))
				{
					if (line.substr(0, 5) == "from ") {
						std::istringstream s(line.substr(5));
						s >> _c->_from.x; s >> _c->_from.y; s >> _c->_from.z;
						std::cout << "from " << _c->_from.x << " " << _c->_from.y << " " << _c->_from.z << std::endl;
					}
					else
						if (line.substr(0, 3) == "at ") {
							std::istringstream s(line.substr(3));
							s >> _c->_at.x; s >> _c->_at.y; s >> _c->_at.z;
							std::cout << "at " << _c->_at.x << " " << _c->_at.y << " " << _c->_at.z << std::endl;
						}
						else
							if (line.substr(0, 3) == "up ") {
								std::istringstream s(line.substr(3));
								s >> _c->_up.x; s >> _c->_up.y; s >> _c->_up.z;
								std::cout << "up " << _c->_up.x << " " << _c->_up.y << " " << _c->_up.z << std::endl;
							}
							else
								if (line.substr(0, 6) == "angle ") {
									std::istringstream s(line.substr(6));
									s >> _c->_angle;
									std::cout << "angle " << _c->_angle << std::endl;
								}
								else
									if (line.substr(0, 7) == "hither ") {
										std::istringstream s(line.substr(7));
										s >> _c->_hither;
										std::cout << "hither " << _c->_hither << std::endl;
									}
									else
										if (line.substr(0, 11) == "resolution ") {
											std::istringstream s(line.substr(11));
											s >> _RES.x; s >> _RES.y;
											std::cout << "resolution " << _RES.x << " " << _RES.y << std::endl;
											break;
										}
				}
			}
			else
				if (line.substr(0, 2) == "l ") {
					std::istringstream s(line.substr(2));
					light l;
					s >> l.XYZ.x; s >> l.XYZ.y; s >> l.XYZ.z;

					s >> l.RGB.r; s >> l.RGB.g; s >> l.RGB.b;
					std::cout << "light " << l.XYZ.x << " " << l.XYZ.y << " " << l.XYZ.z
						<< l.RGB.r << " " << l.RGB.g << " " << l.RGB.b << std::endl;
					_lights.push_back(l);
				}
				else
					if (line.substr(0, 2) == "f ") {
						std::istringstream s(line.substr(2));
						//TODO
					}
					else
						if (line.substr(0, 3) == "pl ") {
							std::istringstream s(line.substr(3));
							//TODO
						}
						else
							if (line.substr(0, 2) == "s ") {
								std::istringstream s(line.substr(2));
								//TODO
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
