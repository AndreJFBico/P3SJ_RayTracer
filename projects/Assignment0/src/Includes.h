#pragma once


//This define is used to allow usage of fopen and fwrite functions
//in the savebmp function, when SOIL is added this can go away.
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
	glm::vec3 RGB;
};

struct light
{
	/*-----*/
	glm::vec3 XYZ;
	/*-----*/
	glm::vec3 RGB;
	/*-----*/
};