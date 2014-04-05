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

const float  PI = 3.14159265358979f;

const float A_L_X_SIZE = 0.6f;
const float A_L_Y_SIZE = 0.6f;
const glm::vec2 LightAttenuation = glm::vec2(0.0f, 0.00000f);

#define NUM_SHADOW_RAYS 10
#define CAPTION "P3SJ"
#define NUM_THREADS 4

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
	glm::vec3 corner;
	/*-----*/
	glm::vec3 xdir;
	/*-----*/
	glm::vec3 ydir;
};