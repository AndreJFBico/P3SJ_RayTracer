#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>

#include <glm/glm.hpp>

#define CAPTION "P3SJ"

struct pixel
{
	double r;
	double g;
	double b;
};

struct light
{
	/*-----*/
	double x;
	double y;
	double z;
	/*-----*/
	double r;
	double g;
	double b;
	/*-----*/
};