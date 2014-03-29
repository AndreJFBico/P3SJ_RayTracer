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
	_maxDiv = 4;
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
	int e = 1;
	_pixels = new pixel[n];
	_RES.x = (float)_width;
	_RES.y = (float)_height;

	glm::vec3 _colors[4]; //array para guardar as cores de cada canto do pixel

	for (int y = 0; y < _RES.y ; y++)
	{
		for (int x = 0; x < _RES.x ; x++)
		{
			
			_currentPixel = y*_width + x;

			//divide pixel em 4
			_r->calculateWCS(glm::vec2(x, y), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto inf esquerdo
			_colors[0] = trace(_geometry, _r, 0, false);

			_r->calculateWCS(glm::vec2(x + e, y), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto inf direito
			_colors[1] = trace(_geometry, _r, 0, false);

			_r->calculateWCS(glm::vec2(x + e, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto sup direito
			_colors[2] = trace(_geometry, _r, 0, false);

			_r->calculateWCS(glm::vec2(x, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto sup esquerdo
			_colors[3] = trace(_geometry, _r, 0, false);

			glm::vec3 finalColor = glm::vec3(0.0);
			finalColor = monteCarloSampling(x, y, _colors, 0, e);

			_pixels[_currentPixel].RGB.r = finalColor.r;
			_pixels[_currentPixel].RGB.g = finalColor.g;
			_pixels[_currentPixel].RGB.b = finalColor.b;

		}
	}
	std::cout << "ended rendering ..." << std::endl;
}

glm::vec3 Scene::monteCarloSampling(int x, int y, glm::vec3* c, int iter, int epsilon){
	
	float threshold = 2.7f;
	int e = epsilon / 2;
	glm::vec3 vecAux[4], vecAux1[4], vecAux2[4], vecAux3[4];

	glm::vec3 traceColor = glm::vec3(0.0);

	glm::vec3 res = glm::vec3(0.0);
	bool similar = true;

	if (iter >= 4){
		res.r = (c[0].r + c[1].r + c[2].r + c[3].r) / 4;
		res.g = (c[0].g + c[1].g + c[2].g + c[3].g) / 4;
		res.b = (c[0].b + c[1].b + c[2].b + c[3].b) / 4;
		return res;
}

	//verificacao se sao semelhantes
	for (int i = 0, k = 1; i < 4; i++, k++){

		if (i = 3) k = 0;

		float diff = abs(c[i].r - c[k].r) + abs(c[i].g - c[k].g) + abs(c[i].b - c[k].b);

		if (diff > threshold){ // cores diferentes
			similar = false;
			break;
		}

	}

	if (similar){ //semelhantes => avg das cores
		res.r = (c[0].r + c[1].r + c[2].r + c[3].r) / 4;
		res.g = (c[0].g + c[1].g + c[2].g + c[3].g) / 4;
		res.b = (c[0].b + c[1].b + c[2].b + c[3].b) / 4;
	}
	else{ //diferentes => subdivisao em 5 (vecAux = quadrante inf esquerdo do pixel)
		vecAux[0] = c[0];
		vecAux1[1] = c[1];
		vecAux2[2] = c[2];
		vecAux3[3] = c[3];

		_r->calculateWCS(glm::vec2(x + e, y), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = trace(_geometry, _r, 0, false);
		vecAux[1] = traceColor;
		vecAux1[0] = traceColor;

		_r->calculateWCS(glm::vec2(x + e, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = trace(_geometry, _r, 0, false);
		vecAux[2] = traceColor;
		vecAux1[3] = traceColor;
		vecAux2[0] = traceColor;
		vecAux3[1] = traceColor;

		_r->calculateWCS(glm::vec2(x , y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = trace(_geometry, _r, 0, false);
		vecAux[3] = traceColor;
		vecAux3[0] = traceColor;

		_r->calculateWCS(glm::vec2(x + epsilon, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = trace(_geometry, _r, 0, false);
		vecAux1[2] = traceColor;
		vecAux2[1] = traceColor;

		_r->calculateWCS(glm::vec2(x + e, y + epsilon), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = trace(_geometry, _r, 0, false);
		vecAux2[3] = traceColor;
		vecAux3[2] = traceColor;

		glm::vec3 cor1 = glm::vec3(0.0);
		cor1 = monteCarloSampling(x, y, vecAux, iter + 1, e);

		glm::vec3 cor2 = glm::vec3(0.0);
		cor2 = monteCarloSampling(x + e, y, vecAux1, iter + 1, e);

		glm::vec3 cor3 = glm::vec3(0.0);
		cor3 = monteCarloSampling(x + e, y + e, vecAux2, iter + 1, e);

		glm::vec3 cor4 = glm::vec3(0.0);
		cor3 = monteCarloSampling(x, y + e, vecAux3, iter + 1, e);

		res.r = (cor1.r + cor2.r + cor3.r + cor4.r) / 4;
		res.g = (cor1.g + cor2.g + cor3.g + cor4.g) / 4;
		res.b = (cor1.b + cor2.b + cor3.b + cor4.b) / 4;

	}
	
	return res;
};

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