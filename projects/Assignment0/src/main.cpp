///////////////////////////////////////////////////////////////////////
//	P3SJ - Assignment 2
// 
//
///////////////////////////////////////////////////////////////////////
#include "Includes.h"
#include "Scene.h"

struct pixel
{
	double r;
	double g;
	double b;
};

int thisone;

int main(int argc, char* argv[])
{
	Scene *s = new Scene();
	std::cout << "rendering ..." << std::endl;

	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width*height;
	pixel *pixels = new pixel[n];

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			thisone = y*width + x;

			if ((x > 200 && x < 440) && (y > 200 && y < 280))
			{
				pixels[thisone].r = 0.2;
				pixels[thisone].g = 0.4;
				pixels[thisone].b = 0.1;
			}
			else
			{
				pixels[thisone].r = 0;
				pixels[thisone].g = 0;
				pixels[thisone].b = 0;
			}
		}
	}

	s->savebmp("scene.bmp",width, height, dpi, pixels);

	return 0;
}