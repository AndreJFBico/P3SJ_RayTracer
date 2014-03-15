#include "Scene.h"

Scene::Scene()
{
	RES.x = 0;
	RES.y = 0;
}

void Scene::loadNFF(const char *filename)
{

}

void Scene::drawScene()
{
	std::cout << "rendering ..." << std::endl;

	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width*height;
	pixel *pixels = new pixel[n];
	RES.x = width;
	RES.y = height;
	for (int y = 0; y < RES.y; y++)
	{
		for (int x = 0; x < RES.x; x++)
		{
			currentPixel = y*width + x;

			if ((x > 200 && x < 440) && (y > 200 && y < 280))
			{
				pixels[currentPixel].RGB.r = 0.2f;
				pixels[currentPixel].RGB.g = 0.4f;
				pixels[currentPixel].RGB.b = 0.1f;
			}
			else
			{
				pixels[currentPixel].RGB.r = 0.0f;
				pixels[currentPixel].RGB.g = 0.0f;
				pixels[currentPixel].RGB.b = 0.0f;
			}
		}
	}
	std::cout << "ended rendering ..." << std::endl;
	savebmp("scene.bmp", width, height, dpi, pixels);
	std::cout << "saved bmp ..." << std::endl;
}

void Scene::savebmp(const char *filename, int w, int h, int dpi, pixel *data)
{
	FILE *f;
	int k = w*h;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };

	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);

	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(filename,"wb");

	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for (int i = 0; i < k; i++)
	{
		pixel rgb = data[i];

		double red = (data[i].RGB.r) * 255;
		double green = (data[i].RGB.g) * 255;
		double blue = (data[i].RGB.b) * 255;

		unsigned char color[3] = { (int)floor(blue), (int)floor(green), (int)floor(red)};

		fwrite(color,1,3,f);
	}

	fclose(f);
}