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
#include <unordered_map>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

const float  PI = 3.14159265358979f;

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