#include "Scene.h"

Scene::Scene()
{
	_RES = glm::vec2(0.0);
	_backgroundColor = glm::vec3(0.0);
	_lights = *(new std::vector<light>());
	_c = NULL;
	_r = new Ray();
}

void Scene::loadNFF(std::string fpath)
{
	NFFLoader::getInstance().clear();
	NFFLoader::getInstance().load(fpath);
	_backgroundColor = NFFLoader::getInstance().getBackgroundColor();
	_RES = NFFLoader::getInstance().getRes();
	_lights = NFFLoader::getInstance().getLight();
	_c = NFFLoader::getInstance().getCamera();
	_geometry = NFFLoader::getInstance().getGeometry();
}

void Scene::drawScene()
{
	std::cout << "rendering ..." << std::endl;
	
	int dpi = 72;
	int width = 512;
	int height = 512;
	int n = width*height;
	pixel *pixels = new pixel[n];
	_RES.x = (float)width;
	_RES.y = (float)height;
	for (int y = 0; y < _RES.y; y++)
	{
		for (int x = 0; x < _RES.x; x++)
		{
			_currentPixel = y*width + x;
			_r->calculateWCS(glm::vec2(x, y), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);

			Geometry* nearest = NULL;
			float closest = 0;
			float prevD2Obj = INT_MAX;
			
			//checks if primary ray intersects with any object
			for (std::vector<Geometry*>::iterator it = _geometry.begin(); it != _geometry.end(); it++)
			{
				if ((*it)->intersect(_r))
				{
					//checks if object is closer
					if (_r->dToObject < prevD2Obj)
					{
						nearest = (*it);
						closest = (*it)->_id;
						prevD2Obj = _r->dToObject;
					}
				}
			}

			//if there was an intersection calculates shadowfillers
			if (nearest != NULL){

				//vector to store all shadowfillers
				std::vector<Ray*> _shadowfillers;
				Sphere* s = (Sphere*)nearest;
				glm::vec3 normal = _r->intersectPoint - s->_center;
				std::unordered_map<Ray*, int> _lightsofSF;


				//for each light in the scene create a shadowfiller if the light might have a contribuition (l.XYZ - intersect . normal) > 0
				int j = 0;
				for (light l : _lights){
					if (glm::dot(l.XYZ - _r->intersectPoint, normal) > 0){
						Ray * r = new Ray();
						r->origin = _r->intersectPoint;
						r->direction = l.XYZ - r->origin;
						_shadowfillers.push_back(r);
						_lightsofSF.emplace(r, j);
					}
					j++;
				}

				//for each shadowfiller see if it collides with any object in the scene
				float prevD2Obj2 = INT_MAX;
				for (std::vector<Geometry*>::iterator it = _geometry.begin(); it != _geometry.end(); it++)
				{
					int i = 0;
					if (!_shadowfillers.empty()){
						for (std::vector<Ray*>::iterator it2 = _shadowfillers.begin(); it2 != _shadowfillers.end(); ){

							if ((*it)->intersect(*it2))
								it2 = _shadowfillers.erase(_shadowfillers.begin() + i);

							else{
								it2++;
								i++;
							}
							
						}
					}
				}

				glm::vec3 lightComp = glm::vec3(1.0);
				for (Ray* sf : _shadowfillers){
					light luz = _lights[_lightsofSF.at(sf)];
					lightComp.r = lightComp.r * luz.RGB.r;
					lightComp.g = lightComp.g * luz.RGB.g;
					lightComp.b = lightComp.b * luz.RGB.b;
				}
				pixels[_currentPixel].RGB.r = nearest->_RGB.r * lightComp.r * nearest->_Kd;
				pixels[_currentPixel].RGB.g = nearest->_RGB.g * lightComp.g * nearest->_Kd;
				pixels[_currentPixel].RGB.b = nearest->_RGB.b * lightComp.b * nearest->_Kd;
			}
			else{
				pixels[_currentPixel].RGB.r = _backgroundColor.r;
				pixels[_currentPixel].RGB.g = _backgroundColor.g;
				pixels[_currentPixel].RGB.b = _backgroundColor.b;
			}
			_r->dToObject = 0;
		}
	}
	std::cout << "ended rendering ..." << std::endl;
	OutConverter::getInstance().saveBMP("scene.bmp", width, height, dpi, pixels);
	std::cout << "saved bmp ..." << std::endl;
}