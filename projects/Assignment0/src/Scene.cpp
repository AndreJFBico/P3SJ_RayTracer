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

			float closest = 0;
			float prevD2Obj = INT_MAX;
			for (Geometry * g : _geometry)
			{
				if (g->intersect(_r))
				{
					if (_r->dToObject < prevD2Obj)
					{
						closest = g->_id;
						prevD2Obj = _r->dToObject;
					}
				}
			}
			_r->dToObject = 0;
			pixels[_currentPixel].RGB.r = closest;
			pixels[_currentPixel].RGB.g = closest;
			pixels[_currentPixel].RGB.b = closest;
		}
	}
	std::cout << "ended rendering ..." << std::endl;
	OutConverter::getInstance().saveBMP("scene.bmp", width, height, dpi, pixels);
	std::cout << "saved bmp ..." << std::endl;
}