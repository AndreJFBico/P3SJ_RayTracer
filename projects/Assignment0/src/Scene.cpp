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
}

void Scene::drawScene()
{
	std::cout << "rendering ..." << std::endl;
	
	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width*height;
	pixel *pixels = new pixel[n];
	_RES.x = (float)width;
	_RES.y = (float)height;
	for (int y = 0; y < _RES.y; y++)
	{
		for (int x = 0; x < _RES.x; x++)
		{
			_currentPixel = y*width + x;
			_r->calculateWCS(glm::vec2(x, y), _c->_at, _c->_from, _c->_up);

			if ((x > 200 && x < 440) && (y > 200 && y < 280))
			{
				pixels[_currentPixel].RGB.r = 0.2f;
				pixels[_currentPixel].RGB.g = 0.4f;
				pixels[_currentPixel].RGB.b = 0.1f;
			}
			else
			{
				pixels[_currentPixel].RGB.r = 0.0f;
				pixels[_currentPixel].RGB.g = 0.0f;
				pixels[_currentPixel].RGB.b = 0.0f;
			}
		}
	}
	std::cout << "ended rendering ..." << std::endl;
	OutConverter::getInstance().saveBMP("scene.bmp", width, height, dpi, pixels);
	std::cout << "saved bmp ..." << std::endl;
}