#include "Scene.h"

Scene::Scene()
{
	_RES = glm::vec2(0.0);
	_backgroundColor = glm::vec3(0.0);
	_lights = *(new std::vector<light>());
	_c = NULL;
	_dpi = 72;
	_width = 512;
	_height = 512;
	_maxDepth = 4;
	_t = new Twister();
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
	genAreaLightPlanes();
}

void Scene::loadObj(std::string fpath)
{
	PieceReader::getInstance().init();
	//PieceReader::getInstance().clearAll();
	PieceReader::getInstance().readObject(fpath);
	std::vector<Vertex> v = PieceReader::getInstance().getVertices();
	
	for (int i = 0; i < v.size(); i += 3)
	{
		Triangle *p = new Triangle();
		p->_id = 0;
		p->_Kd = 1;
		p->_Ks = 0.3f;
		p->_Shine = 10;
		p->_refract_index = 0;
		p->_T = 0;
		p->_RGB = glm::vec3(0.0, 0.0, 1.0f);
		p->_vertexes.push_back(glm::vec3(v.at(i).XYZW));
		p->_vertexes.push_back(glm::vec3(v.at(i + 1).XYZW));
		p->_vertexes.push_back(glm::vec3(v.at(i + 2).XYZW));
		_geometry.push_back(p);
	}
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

glm::vec3 Scene::depthOfField(Ray * ray)
{
	glm::vec3 rayDirection = ray->direction;
	glm::vec3 pointAimed = _c->_from + FOCAL_LENGTH * rayDirection;
	//pointAimed is the position of pixel on focal plane in specified ray
	//direction and 15 is my focal length(can change accordingly)
	glm::normalize(rayDirection);
	float r = 1;
	glm::vec3 pixelColors = glm::vec3(0.0f);
	for (int di = 0; di < DEPTH_RAYS; di++)
	{ // shooting DEPTH_RAYS random rays
		float du = RandomFloat(-LENS_SIZE, LENS_SIZE);
		float dv = RandomFloat(-LENS_SIZE, LENS_SIZE);

		// creating new camera position(or ray start using jittering)
		//glm::vec3 start = _c->_from - (r / 2)*_c->_Xe - (r / 2)*_c->_Ye + r*(du)*_c->_Xe + r*(dv)*_c->_Ye;

		glm::vec3 start = _c->_from - _c->_Xe*du - _c->_Ye*dv;

		//getting the new direction of ray
		glm::vec3 direction = pointAimed - start;

		glm::normalize(direction);
		ray->direction = direction;
		ray->origin = start;
		glm::vec3 pixelColor = trace(_geometry, ray, 0, false);
		pixelColors += pixelColor;
	}
	return (pixelColors / ((float)DEPTH_RAYS));
}

/*void Scene::depthOfField(Ray * ray)
{
	glm::vec3 rayDirection = ray->direction;
	glm::vec3 pointAimed = _c->_from + FOCAL_LENGTH * rayDirection;
	//pointAimed is the position of pixel on focal plane in specified ray
	//direction and 15 is my focal length(can change accordingly)
	glm::normalize(rayDirection);
	float r = 1;
	glm::vec3 pixelColors = glm::vec3(0.0f);



	//float du = rand() / float(RAND_MAX + 1);//generating random number
	//float dv = rand() / float(RAND_MAX + 1);

	// creating new camera position(or ray start using jittering)
	glm::vec3 start = _c->_from - (r / 2)*_c->_Xe - (r / 2)*_c->_Ye + r*(du)*_c->_Xe + r*(dv)*_c->_Ye;

	//getting the new direction of ray
	glm::vec3 direction = pointAimed - start;

	glm::normalize(direction);
	ray->direction = direction;
	ray->origin = start;
}*/


void Scene::partialSceneCalculation(int initX, int initY, float endX, float endY)
{
	Ray* ray = new Ray();

	int e = 1;
	glm::vec3 _colors[4]; //array para guardar as cores de cada canto do pixel

	for (int y = initY; y < initY + endY; y++)
	{
		for (int x = initX; x < initX + endX; x++)
		{
			int currentpixel = y*_width + x;
			
			ray->calculateWCS(glm::vec2(x, y), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto inf esquerdo
			_colors[0] = depthOfField(ray);
			//_colors[0] = trace(_geometry, ray, 0, false);

			ray->calculateWCS(glm::vec2(x + e, y), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto inf direito
			_colors[1] = depthOfField(ray);
			//_colors[1] = trace(_geometry, ray, 0, false);


			ray->calculateWCS(glm::vec2(x + e, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto sup direito
			_colors[2] = depthOfField(ray);
			//_colors[2] = trace(_geometry, ray, 0, false);

			ray->calculateWCS(glm::vec2(x, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye); //canto sup esquerdo
			_colors[3] = depthOfField(ray);
			//_colors[3] = trace(_geometry, ray, 0, false);

			glm::vec3 finalColor = glm::vec3(0.0);
			finalColor = monteCarloSampling(x, y, _colors, 0, e);

			//glm::vec3 finalColor = depthOfField(ray);

			_pixels[currentpixel].RGB.r = finalColor.r;
			_pixels[currentpixel].RGB.g = finalColor.g;
			_pixels[currentpixel].RGB.b = finalColor.b;

		}
	}

}


void Scene::genAreaLightPlanes()
{
	for (unsigned int i = 0; i < _lights.size(); i++)
	{
		_lights[i].corner = _lights[i].XYZ - glm::vec3(A_L_X_SIZE, A_L_Y_SIZE, 0.0f);
		_lights[i].xdir = glm::vec3(1.0f, 0.0f, 0.0f);
		_lights[i].ydir = glm::vec3(0.0f, 1.0f, 0.0f);
	}
}

void Scene::loadScene()
{
	std::cout << std::thread::hardware_concurrency() << std::endl;
	std::cout << "rendering ..." << std::endl;
	
	int n = _width*_height;
	_pixels = new pixel[n];
	_RES.x = (float)_width;
	_RES.y = (float)_height;

	std::thread first([=](){partialSceneCalculation(0, 0, _RES.x / (NUM_THREADS / 2), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread first(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum, _RES.x / (NUM_THREADS / 2), _RES.y / (NUM_THREADS / 2)));

	std::thread second([=](){partialSceneCalculation(_RES.x - _RES.x / 2, 0, _RES.x / (NUM_THREADS / 2), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread second(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum++, _RES.x / NUM_THREADS, _RES.y / NUM_THREADS));

	std::thread third([=](){partialSceneCalculation(0, _RES.y - _RES.y / 2, _RES.x / (NUM_THREADS / 2), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread third(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum++, _RES.x / NUM_THREADS, _RES.y / NUM_THREADS));

	std::thread forth([=](){partialSceneCalculation(_RES.x - _RES.x / 2, _RES.y - _RES.y / 2, _RES.x / (NUM_THREADS / 2), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread forth(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum++, _RES.x / NUM_THREADS, _RES.y / NUM_THREADS));

	/** /
	std::thread fifth([=](){partialSceneCalculation(_RES.x - _RES.x / 2, 0, _RES.x / (NUM_THREADS), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread first(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum, _RES.x / (NUM_THREADS / 2), _RES.y / (NUM_THREADS / 2)));

	std::thread sixth([=](){partialSceneCalculation(_RES.x - _RES.x*3/4, 0, _RES.x / (NUM_THREADS), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread second(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum++, _RES.x / NUM_THREADS, _RES.y / NUM_THREADS));

	std::thread seventh([=](){partialSceneCalculation(_RES.x - _RES.x * 3 / 4, _RES.y - _RES.y / 2, _RES.x / (NUM_THREADS), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread third(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum++, _RES.x / NUM_THREADS, _RES.y / NUM_THREADS));

	std::thread eighth([=](){partialSceneCalculation(_RES.x - _RES.x / 2, _RES.y - _RES.y / 2, _RES.x / (NUM_THREADS), _RES.y / (NUM_THREADS / 2)); return 1; });//std::thread forth(&Scene::partialSceneCalculation, (_RES.x - _RES.x / threadNum, _RES.y - _RES.y / threadNum++, _RES.x / NUM_THREADS, _RES.y / NUM_THREADS));
	/**/

	first.join();
	second.join();
	third.join();
	forth.join();
	/** /
	fifth.join();
	sixth.join();
	seventh.join();
	eighth.join();
	/**/
	std::cout << "ended rendering ..." << std::endl;

}

glm::vec3 Scene::monteCarloSampling(int x, int y, glm::vec3* c, int iter, int epsilon){
	
	Ray* _r = new Ray();

	float threshold = 2.9f;
	int e = epsilon / 2;
	glm::vec3 vecAux[4], vecAux1[4], vecAux2[4], vecAux3[4];

	glm::vec3 traceColor = glm::vec3(0.0);

	glm::vec3 res = glm::vec3(0.0);
	bool similar = true;

	if (iter >= 4){
		res.r = (c[0].r + c[1].r + c[2].r + c[3].r) / 4;
		res.g = (c[0].g + c[1].g + c[2].g + c[3].g) / 4;
		res.b = (c[0].b + c[1].b + c[2].b + c[3].b) / 4;
		delete _r;
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
		traceColor = depthOfField(_r);
		//traceColor = trace(_geometry, _r, 0, false);
		vecAux[1] = traceColor;
		vecAux1[0] = traceColor;

		_r->calculateWCS(glm::vec2(x + e, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = depthOfField(_r);
		//traceColor = trace(_geometry, _r, 0, false);
		vecAux[2] = traceColor;
		vecAux1[3] = traceColor;
		vecAux2[0] = traceColor;
		vecAux3[1] = traceColor;

		_r->calculateWCS(glm::vec2(x , y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = depthOfField(_r);
		//traceColor = trace(_geometry, _r, 0, false);
		vecAux[3] = traceColor;
		vecAux3[0] = traceColor;

		_r->calculateWCS(glm::vec2(x + epsilon, y + e), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = depthOfField(_r);
		//traceColor = trace(_geometry, _r, 0, false);
		vecAux1[2] = traceColor;
		vecAux2[1] = traceColor;

		_r->calculateWCS(glm::vec2(x + e, y + epsilon), _c->_at, _c->_from, _c->_up, _c->_w, _c->_h, _c->_Ze, _c->_Xe, _c->_Ye);
		traceColor = depthOfField(_r);
		//traceColor = trace(_geometry, _r, 0, false);
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
	delete _r;
	return res;
};

glm::vec3 calculateRayObjectIntersection(std::vector<Geometry*> geometry, Ray*& ray, Geometry*& nearest)
{
	glm::vec3 closestintersect = glm::vec3(0.0f, 0.0f, 0.0f);
	float prevD2Obj = (float)INT_MAX;

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

void checkColisionOfShadowRays(std::vector<Geometry*> geometry, std::vector<Ray*>& shadowfillers)
{
	//See if it collides with any object in the scene
	for (std::vector<Geometry*>::iterator it = geometry.begin(); it != geometry.end(); it++)
	{
		for (std::vector<Ray*>::iterator it2 = shadowfillers.begin(); it2 != shadowfillers.end(); it2++)
		{
			if ((*it)->intersect(*it2))
			{
				(*it2)->shadowfillertype = false;
			}
		}
	}
}

void calculateShadowFillers(std::vector<Ray*>& shadowfillers, glm::vec3 normal, 
							std::vector<lightRays>* lightsOfSF, std::vector<light> lights,
							bool refracted, glm::vec3 closestintersect, std::vector<Geometry*> geometry, Twister *t)
{
	int j = 0;
	for (light l : lights)
	{ 
		std::vector<rayPos> rays_pos;
		bool behind_light = false;
		/*Ray * r = new Ray();
			const float ERR = 0.001f;
			if (refracted)
				r->origin = closestintersect - normal * ERR;
			else r->origin = closestintersect + normal * ERR;
			r->direction = glm::normalize(l.XYZ - r->origin);
			shadowfillers.push_back(r);
		rays_pos->push_back(rayPos(r, l.XYZ));*/

		const float ERR = 0.001f;
		for (int i = 0; i < NUM_SHADOW_RAYS; i++)
		{
			Ray * r2 = new Ray();
			if (refracted)
				r2->origin = closestintersect - normal * ERR;
			else r2->origin = closestintersect + normal * ERR;
			//falta mudar a direction para random
			glm::vec3 randomizedPoint;
			float du = t->Rand() * A_L_X_SIZE;
			float dv = t->Rand() * A_L_Y_SIZE;
			if (i < NUM_SHADOW_RAYS / 4.0f)
			{
				
				randomizedPoint = l.corner + du*l.xdir + dv*l.ydir;
			}
			else if (i < 2.0 *(NUM_SHADOW_RAYS / 4.0f))
			{
				
				randomizedPoint = l.XYZ + du*l.xdir + dv*l.ydir;
			}
			else if (i < 3.0 *(NUM_SHADOW_RAYS / 4.0f))
			{
				
				randomizedPoint = l.corner + l.xdir*A_L_X_SIZE + du*l.xdir + dv*l.ydir;
			}
			else if (i < NUM_SHADOW_RAYS)
			{
				
				randomizedPoint = l.corner + l.ydir*A_L_Y_SIZE + du*l.xdir + dv*l.ydir;
			}
			//float du = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f / NUM_SHADOW_RAYS)));
			//float dv = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f / NUM_SHADOW_RAYS)));
			//glm::vec3 randomizedPoint = l.XYZ;
			
			//glm::vec3 randomizedPoint = l.corner + glm::vec3((i % 4) * (2 * A_L_X_SIZE / NUM_SHADOW_RAYS), (i / 4) * (2 * A_L_Y_SIZE / NUM_SHADOW_RAYS), 0.0) + du*l.xdir + dv*l.ydir;
			//glm::vec3 randomizedPoint = l.corner + l.xdir * std::cos(3.0f * du) * 3.0f + l.ydir * sin(3.0f * dv) * 3.0f;
			r2->direction = glm::normalize(randomizedPoint - r2->origin);
			if (glm::dot(normal, randomizedPoint - closestintersect) > 0)
			{
				shadowfillers.push_back(r2);
				rays_pos.push_back(rayPos(r2, randomizedPoint));
			}
			else behind_light = true;
			}
		if (!rays_pos.empty())
		{
			lightsOfSF->push_back(lightRays(j, rays_pos));
			checkColisionOfShadowRays(geometry, shadowfillers);
		}
		j++;
	}
}

void calculateLocalColor(glm::vec3& lightComp, std::vector<Ray*> shadowfillers, glm::vec3 normal,
						std::vector<lightRays>* lightsOfSF, std::vector<light> lights,
						glm::vec3 closestintersect, Ray* ray, Geometry* nearest)
{
	
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		std::vector<rayPos> rays_pos;
		for (std::vector<lightRays>::iterator it = lightsOfSF->begin(); it != lightsOfSF->end(); it++)
		{
			if (i == (*it).l)
			{
				rays_pos = (*it).rays;
				light luz = lights[(*it).l];
				glm::vec3 shadowFillerContrib = glm::vec3(0.0, 0.0, 0.0);

				for (std::vector<rayPos>::iterator it = rays_pos.begin(); it != rays_pos.end(); it++)
				{
					float attenuation = 1.0f / (1.0f + LightAttenuation.x * glm::length(closestintersect - it->pos) + LightAttenuation.y * pow(glm::length(closestintersect - it->pos), 2));
					glm::vec3 L = glm::normalize(it->pos - closestintersect);

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

					if ((it->ray)->shadowfillertype)
					{
						shadowFillerContrib.r = (diffuse.r + specular / 2.0f) * attenuation * luz.RGB.r + shadowFillerContrib.r;
						shadowFillerContrib.g = (diffuse.g + specular / 2.0f) * attenuation * luz.RGB.g + shadowFillerContrib.g;
						shadowFillerContrib.b = (diffuse.b + specular / 2.0f) * attenuation * luz.RGB.b + shadowFillerContrib.b;
					}
					else{
						shadowFillerContrib.r = fmax(shadowFillerContrib.r - (diffuse.r + specular) * attenuation * 0.1f, 0.0f);
						shadowFillerContrib.g = fmax(shadowFillerContrib.g - (diffuse.r + specular) * attenuation * 0.1f, 0.0f);
						shadowFillerContrib.b = fmax(shadowFillerContrib.b - (diffuse.r + specular) * attenuation * 0.1f, 0.0f);
					}
				}
				shadowFillerContrib = (shadowFillerContrib / ((float)rays_pos.size()));
				lightComp.r = shadowFillerContrib.r + lightComp.r;
				lightComp.g = shadowFillerContrib.g + lightComp.g;
				lightComp.b = shadowFillerContrib.b + lightComp.b;
				break;
			}	
		}
	}
	/*glm::vec2 LightAttenuation = glm::vec2(0.0f, 0.00000f);
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
	}*/
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
	std::vector<lightRays>* _lightsofSF = new std::vector<lightRays>;
	glm::vec3 lightComp = glm::vec3(0.0);

	//for each light in the scene create a shadowfiller if the light might have a contribuition (l.XYZ - intersect . normal) > 0
	calculateShadowFillers(_shadowfillers, normal, _lightsofSF, _lights, refracted, closestintersect, _geometry, _t);

	calculateLocalColor(lightComp, _shadowfillers, normal, _lightsofSF, _lights, closestintersect, ray, nearest);

	for (std::vector<Ray*>::iterator it = _shadowfillers.begin(); it != _shadowfillers.end(); it++)
	{
		delete(*it);
	}
	std::vector<Ray*>().swap(_shadowfillers);

	for (std::vector<lightRays>::iterator it = _lightsofSF->begin(); it != _lightsofSF->end(); it++)
	{
		std::vector<rayPos>().swap(it->rays);
	}
	std::vector<lightRays>().swap(*_lightsofSF);

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