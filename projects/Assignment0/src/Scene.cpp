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
			glm::vec3 closestintersect;
			
			//checks if primary ray intersects with any object
			for (std::vector<Geometry*>::iterator it = _geometry.begin(); it != _geometry.end(); it++)
			{
				if ((*it)->intersect(_r))
				{
					//checks if object is closer
					if (_r->dToObject < prevD2Obj)
					{
						closestintersect = _r->intersectPoint;
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
				glm::vec3 normal = nearest->calculateNormal(_r);
				std::unordered_map<Ray*, int> _lightsofSF;


				//for each light in the scene create a shadowfiller if the light might have a contribuition (l.XYZ - intersect . normal) > 0
				int j = 0;
				for (light l : _lights){
					if (glm::dot(l.XYZ - closestintersect, normal) > 0){
						Ray * r = new Ray();
						const float ERR = 0.001f;
						r->origin = closestintersect + normal * ERR;
						r->direction = l.XYZ - r->origin;
						_shadowfillers.push_back(r);
						_lightsofSF.emplace(r, j);
					}
					j++;
				}

				//for each shadowfiller see if it collides with any object in the scene
				for (std::vector<Geometry*>::iterator it = _geometry.begin(); it != _geometry.end(); it++)
				{
					if (!_shadowfillers.empty()){
						for (std::vector<Ray*>::iterator it2 = _shadowfillers.begin(); it2 != _shadowfillers.end(); it2++){

							if ((*it)->intersect(*it2))
								(*it2)->shadowfillertype = false;
							
						}
					}
				}

				glm::vec2 LightAttenuation = glm::vec2(0.0f, 0.0005);
				glm::vec3 lightComp = glm::vec3(0.0);
				int i = 0;
				for (Ray* sf : _shadowfillers){
					light luz = _lights[_lightsofSF.at(sf)];
					float attenuation = 1 / (1.0 + LightAttenuation.x * glm::length(closestintersect - luz.XYZ) + LightAttenuation.y * pow(glm::length(closestintersect - luz.XYZ), 2));
					
					glm::vec3 L = glm::normalize(luz.XYZ - closestintersect);

					glm::vec3 E = glm::normalize(_r->origin - closestintersect);
					glm::vec3 H = glm::normalize(L + E);

					//glm::vec3 u = closestintersect - _c->_from;
					//float diffuse = nearest->_Kd * glm::dot(normal, u);
					glm::vec3 R = (-L) - (2.0f * normal*(glm::dot(normal, (-L))));
					//glm::vec3 H = glm::normalize(closestintersect - luz.XYZ + u);
					
					float NdotL = fmin(fmax(glm::dot(normal, L), 0.0f), 1.0f);

					float diffuse = nearest->_Kd * NdotL;

					float specular = 0.0;

					if (NdotL > 0){
						float NdotH = fmin(fmax(glm::dot(R, H), 0.0f), 1.0f);
						float Blinn = pow(NdotH, nearest->_Shine);
						specular = nearest->_Ks * Blinn;
					}

					if (sf->shadowfillertype){
						lightComp.r = (diffuse + specular) * attenuation * luz.RGB.r + lightComp.r;
						lightComp.g = (diffuse + specular) * attenuation * luz.RGB.g + lightComp.g;
						lightComp.b = (diffuse + specular) * attenuation * luz.RGB.b + lightComp.b;
					}
					else{
						lightComp.r = fmax(lightComp.r - 0.1f, 0.0) ;
						lightComp.g = fmax(lightComp.g - 0.1f, 0.0);
						lightComp.b = fmax(lightComp.b - 0.1f, 0.0);
					}
				}

				pixels[_currentPixel].RGB.r = lightComp.r;
				pixels[_currentPixel].RGB.g = lightComp.g;
				pixels[_currentPixel].RGB.b = lightComp.b;
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