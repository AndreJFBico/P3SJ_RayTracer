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
	_maxDepth = 4;
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

glm::vec3 calculateRayObjectIntersection(std::vector<Geometry*> geometry, Ray*& ray, Geometry*& nearest)
{
	glm::vec3 closestintersect = glm::vec3(0.0f, 0.0f, 0.0f);
	float prevD2Obj = INT_MAX;

	for (std::vector<Geometry*>::iterator it = geometry.begin(); it != geometry.end(); it++)
	{
		if ((*it)->intersect(ray))
		{
			//checks if object is closer
			if (ray->dToObject < prevD2Obj)
			{
				closestintersect = ray->intersectPoint;
				nearest = (*it);
				prevD2Obj = ray->dToObject;
			}
		}
	}
	ray->intersectPoint = closestintersect;
	ray->dToObject = prevD2Obj;

	return closestintersect;
}

void calculateShadowFillers(std::vector<Ray*>& shadowfillers, glm::vec3 normal, 
							std::unordered_map<Ray*, int>& lightsOfSF, std::vector<light> lights,
							bool refracted, glm::vec3 closestintersect, std::vector<Geometry*> geometry)
{
	int j = 0;
	for (light l : lights){
		if (glm::dot(normal, l.XYZ - closestintersect) > 0){
			Ray * r = new Ray();
			const float ERR = 0.001f;
			if (refracted)
				r->origin = closestintersect - normal * ERR;
			else r->origin = closestintersect + normal * ERR;
			r->direction = glm::normalize(l.XYZ - r->origin);
			shadowfillers.push_back(r);
			lightsOfSF.emplace(r, j);

			//See if it collides with any object in the scene
			for (std::vector<Geometry*>::iterator it = geometry.begin(); it != geometry.end(); it++)
			{
				if ((*it)->intersect(r))
					r->shadowfillertype = false;
			}
		}
		j++;
	}
}

void calculateLocalColor(glm::vec3& lightComp, std::vector<Ray*> shadowfillers, glm::vec3 normal,
						std::unordered_map<Ray*, int> lightsOfSF, std::vector<light> lights,
						glm::vec3 closestintersect, Ray* ray, Geometry* nearest)
{
	glm::vec2 LightAttenuation = glm::vec2(0.0f, 0.00000f);
	int i = 0;
	for (Ray* sf : shadowfillers){
		light luz = lights[lightsOfSF.at(sf)];
		float attenuation = 1 / (1.0 + LightAttenuation.x * glm::length(closestintersect - luz.XYZ) + LightAttenuation.y * pow(glm::length(closestintersect - luz.XYZ), 2));

		glm::vec3 L = glm::normalize(luz.XYZ - closestintersect);

		glm::vec3 dE = ray->origin - closestintersect, E;
		if (dE == glm::vec3(0.0, 0.0, 0.0))
			E = dE;
		else E = glm::normalize(ray->origin - closestintersect);
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
			lightComp.r = fmax(lightComp.r - (diffuse.r + specular) * attenuation * 0.1f, 0.0);
			lightComp.g = fmax(lightComp.g - (diffuse.r + specular) * attenuation * 0.1f, 0.0);
			lightComp.b = fmax(lightComp.b - (diffuse.r + specular) * attenuation * 0.1f, 0.0);
		}
	}
}

glm::vec3 Scene::trace(std::vector<Geometry*> geometry, Ray* ray, int depth, bool refracted)
{
	Geometry* nearest = NULL;
	glm::vec3 closestintersect = glm::vec3(0.0f, 0.0f, 0.0f);
	ray->dToObject = 0;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DE INTERSECÇÃO +++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++

	closestintersect = calculateRayObjectIntersection(_geometry, ray, nearest);

	//if there was an intersection calculates shadowfillers
	if (nearest == NULL)
		return _backgroundColor;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DA COR LOCAL +++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++

	std::vector<Ray*> _shadowfillers;
	glm::vec3 normal = nearest->calculateNormal(ray);
	std::unordered_map<Ray*, int> _lightsofSF;
	glm::vec3 lightComp = glm::vec3(0.0);

	//for each light in the scene create a shadowfiller if the light might have a contribuition (l.XYZ - intersect . normal) > 0
	calculateShadowFillers(_shadowfillers, normal, _lightsofSF, _lights, refracted, closestintersect, _geometry);

	calculateLocalColor(lightComp, _shadowfillers, normal, _lightsofSF, _lights, closestintersect, ray, nearest);

	if (depth >= _maxDepth) return lightComp;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DA COR DO RAIO REFLECTIDO +++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	if (nearest->_Ks > 0){
		glm::vec3 rColor;
		Ray* rRay;
		if (refracted)
		{
			rRay = ray->reflect(-normal);
			rColor = trace(geometry, rRay, depth + 1, refracted);
			lightComp = rColor* nearest->_Ks/2.0f + lightComp;
		}
		else
		{
			rRay = ray->reflect(normal);
			rColor = trace(geometry, rRay, depth + 1, refracted);
			lightComp = rColor* nearest->_Ks + lightComp;
		}	
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++ CALCULO DA COR DO RAIO REFRACTADO +++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if (nearest->_T > 0){
		glm::vec3 tColor;
		Ray * tRay;
		if (refracted)
		{
			tRay = ray->refract(-normal, 1.0f);
			tColor = trace(geometry, tRay, depth + 1, false);
		}
		else {
			tRay = ray->refract(normal, nearest->_refract_index);
			tColor = trace(geometry, tRay, depth + 1, true);
		}
		
		lightComp = tColor* nearest->_T/1.1f + lightComp;
	}

	//Clamp
	lightComp.r = fmin(fmax(lightComp.r, 0.0f), 1.0f);
	lightComp.g = fmin(fmax(lightComp.g, 0.0f), 1.0f);
	lightComp.b = fmin(fmax(lightComp.b, 0.0f), 1.0f);
	
	return lightComp;
	
}