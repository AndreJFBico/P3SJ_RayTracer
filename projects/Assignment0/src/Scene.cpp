#include "Scene.h"

Scene::Scene()
{
	_RES = glm::vec2(0.0);
	_backgroundColor = glm::vec3(0.0);
	_lights = *(new std::vector<light>());
	_c = NULL;
	_r = new Ray();
	_dpi = 72;
	_width = 512;
	_height = 512;
	_maxDepth = 3;
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

void Scene::loadScene()
{
	std::cout << "rendering ..." << std::endl;
	
	int n = _width*_height;
	_pixels = new pixel[n];
	_RES.x = (float)_width;
	_RES.y = (float)_height;
	for (int y = 0; y < _RES.y; y++)
	{
		for (int x = 0; x < _RES.x; x++)
		{
			_currentPixel = y*_width + x;
			_r->calculateWCS(glm::vec2(x, y), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);

			glm::vec3 finalColor = glm::vec3(0.0);

			finalColor = trace(_geometry, _r, 0, false);

			_pixels[_currentPixel].RGB.r = finalColor.r;
			_pixels[_currentPixel].RGB.g = finalColor.g;
			_pixels[_currentPixel].RGB.b = finalColor.b;

		}
	}
	std::cout << "ended rendering ..." << std::endl;
	
}

glm::vec3 Scene::trace(std::vector<Geometry*> geometry, Ray* ray, int depth, bool refracted)
{

	Geometry* nearest = NULL;
	float closest = 0;
	float prevD2Obj = INT_MAX;
	glm::vec3 closestintersect = glm::vec3(0.0f, 0.0f, 0.0f);
	ray->dToObject = 0;
			
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DE INTERSECÇÃO +++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++


	//checks if primary ray intersects with any object
	for (std::vector<Geometry*>::iterator it = _geometry.begin(); it != _geometry.end(); it++)
	{
		if ((*it)->intersect(ray))
		{
			//checks if object is closer
			if (ray->dToObject < prevD2Obj)
			{
				closestintersect = ray->intersectPoint;
				nearest = (*it);
				closest = (*it)->_id;
				prevD2Obj = ray->dToObject;
			}
		}
	}
	ray->intersectPoint = closestintersect;
	ray->dToObject = prevD2Obj;

	//if there was an intersection calculates shadowfillers
	if (nearest == NULL)
		return _backgroundColor;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DA COR LOCAL +++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++

	//vector to store all shadowfillers
	std::vector<Ray*> _shadowfillers;
	glm::vec3 normal = nearest->calculateNormal(ray);
	std::unordered_map<Ray*, int> _lightsofSF;

	//for each light in the scene create a shadowfiller if the light might have a contribuition (l.XYZ - intersect . normal) > 0
	int j = 0;
	for (light l : _lights){
		if (glm::dot(normal, l.XYZ - closestintersect) > 0){
			Ray * r = new Ray();
			const float ERR = 0.001f;
			r->origin = closestintersect + normal * ERR;
			r->direction = glm::normalize(l.XYZ - r->origin);
			_shadowfillers.push_back(r);
			_lightsofSF.emplace(r, j);

			//See if it collides with any object in the scene
			for (std::vector<Geometry*>::iterator it = _geometry.begin(); it != _geometry.end(); it++)
			{
				if ((*it)->intersect(r))
					r->shadowfillertype = false;
			}
		}
		j++;
	}

	glm::vec2 LightAttenuation = glm::vec2(0.0f, 0.000001f);
	glm::vec3 lightComp = glm::vec3(0.0);
	int i = 0;
	for (Ray* sf : _shadowfillers){
		light luz = _lights[_lightsofSF.at(sf)];
		float attenuation = 1 / (1.0 + LightAttenuation.x * glm::length(closestintersect - luz.XYZ) + LightAttenuation.y * pow(glm::length(closestintersect - luz.XYZ), 2));
					
		glm::vec3 L = glm::normalize(luz.XYZ - closestintersect);

		glm::vec3 E = glm::normalize(ray->origin - closestintersect);
		glm::vec3 H = glm::normalize(L + E);

		//glm::vec3 u = closestintersect - _c->_from;
		//float diffuse = nearest->_Kd * glm::dot(normal, u);
		glm::vec3 R = (-L) - (2.0f * normal*(glm::dot(normal, (-L))));
		//glm::vec3 H = glm::normalize(closestintersect - luz.XYZ + u);
					
		float NdotL = fmin(fmax(glm::dot(normal, L), 0.0f), 1.0f);

		glm::vec3 diffuse = nearest->_RGB * nearest->_Kd * NdotL;

		float specular = 0.0;
		
		if (NdotL > 0){
			float NdotH = fmin(fmax(glm::dot(R, H), 0.0f), 1.0f);
			float Blinn = pow(NdotH, nearest->_Shine / 8);
			specular = nearest->_Ks * Blinn;
		}

		if (sf->shadowfillertype){
			lightComp.r = (diffuse.r + specular / 2) * attenuation * luz.RGB.r + lightComp.r;
			lightComp.g = (diffuse.g + specular / 2) * attenuation * luz.RGB.g + lightComp.g;
			lightComp.b = (diffuse.b + specular / 2) * attenuation * luz.RGB.b + lightComp.b;
		}
		else{
			lightComp.r = fmax(lightComp.r - (diffuse.r + specular) * attenuation * 0.05f, 0.0);
			lightComp.g = fmax(lightComp.g - (diffuse.r + specular) * attenuation * 0.05f, 0.0);
			lightComp.b = fmax(lightComp.b - (diffuse.r + specular) * attenuation * 0.05f, 0.0);
		}
	}

	if (depth >= _maxDepth) return lightComp;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DA COR DO RAIO REFLECTIDO +++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (nearest->_Ks > 0){
		glm::vec3 rColor;
		Ray* rRay = ray->reflect(normal);
		rColor = trace(geometry, rRay, depth + 1, false);
		lightComp = rColor* nearest->_Ks + lightComp;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DA COR DO RAIO REFRACTADO +++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (nearest->_T > 0){
		glm::vec3 tColor;
		Ray * tRay = ray->refract(normal, nearest->_refract_index);
		tColor = trace(geometry, tRay, depth + 1, true);
		lightComp = tColor* nearest->_T + lightComp;
	}

	//Clamp
	lightComp.r = fmin(fmax(lightComp.r, 0.0f), 1.0f);
	lightComp.g = fmin(fmax(lightComp.g, 0.0f), 1.0f);
	lightComp.b = fmin(fmax(lightComp.b, 0.0f), 1.0f);
	
	return lightComp;
	
}