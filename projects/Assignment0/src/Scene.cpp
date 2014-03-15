#include "Scene.h"

Scene::Scene()
{
	_RES.x = 0;
	_RES.y = 0;
}

void Scene::loadNFF(std::string fpath)
{
	std::ifstream in(fpath, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << fpath << std::endl; exit(1); }	
	std::string line;

	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "b ") {
			std::istringstream s(line.substr(2));
			s >> _backgroundColor.r; s >> _backgroundColor.g; s >> _backgroundColor.b;
			//prints the result
			std::cout << "BackgroundColor " << _backgroundColor.r << " " << _backgroundColor.g << " " << _backgroundColor.b << std::endl;
		} 
		else
		if (line.substr(0, 1) == "v") {
			std::istringstream s(line.substr(1));
			_c = new Camera();
			std::cout << "v" << std::endl;
		}
		else
		if (line.substr(0, 5) == "from ") {
			std::istringstream s(line.substr(5));
			s >> _c->_from.x; s >> _c->_from.y; s >> _c->_from.z;
			std::cout <<"from " << _c->_from.x << " " << _c->_from.y << " " << _c->_from.z << std::endl;
		}
		else
		if (line.substr(0, 3) == "at ") {
			std::istringstream s(line.substr(3));
			s >> _c->_at.x; s >> _c->_at.y; s >> _c->_at.z;
			std::cout << "at " << _c->_at.x << " " << _c->_at.y << " " << _c->_at.z << std::endl;
		}
		else
		if (line.substr(0, 3) == "up ") {
			std::istringstream s(line.substr(3));
			s >> _c->_up.x; s >> _c->_up.y; s >> _c->_up.z;
			std::cout << "up " << _c->_up.x << " " << _c->_up.y << " " << _c->_up.z << std::endl;
		}
		else
		if (line.substr(0, 6) == "angle ") {
			std::istringstream s(line.substr(6));
			s >> _c->_angle;
			std::cout << "angle " << _c->_angle << std::endl;
		}
		else
		if (line.substr(0, 7) == "hither ") {
			std::istringstream s(line.substr(7));
			s >> _c->_hither;
			std::cout << "hither " << _c->_hither << std::endl;
		}
		else
		if (line.substr(0, 11) == "resolution ") {
			std::istringstream s(line.substr(11));
			s >> _RES.x; s >> _RES.y;
			std::cout << "resolution " << _RES.x << " " << _RES.y << std::endl;
		}
		else
		if (line.substr(0, 2) == "l ") {
			std::istringstream s(line.substr(2));
			light l;
			s >> l.XYZ.x; s >> l.XYZ.y; s >> l.XYZ.z;

			s >> l.RGB.r; s >> l.RGB.g; s >> l.RGB.b;
			std::cout << "light "	<< l.XYZ.x << " " << l.XYZ.y << " " << l.XYZ.z 
									<< l.RGB.r << " " << l.RGB.g << " " << l.RGB.b << std::endl;
			_lights.push_back(l);
		}
		else
		if (line.substr(0, 2) == "f ") {
			std::istringstream s(line.substr(2));
			//TODO
		}
		else
		if (line.substr(0, 3) == "pl ") {
			std::istringstream s(line.substr(3));
			//TODO
		}
		else
		if (line.substr(0, 2) == "s ") {
			std::istringstream s(line.substr(2));
			//TODO
		}
	}
}

void Scene::drawScene()
{
	std::cout << "rendering ..." << std::endl;

	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width*height;
	pixel *pixels = new pixel[n];
	_RES.x = width;
	_RES.y = height;
	for (int y = 0; y < _RES.y; y++)
	{
		for (int x = 0; x < _RES.x; x++)
		{
			_currentPixel = y*width + x;

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