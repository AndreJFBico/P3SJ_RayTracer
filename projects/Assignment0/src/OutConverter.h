#pragma once

#include "Includes.h"

class OutConverter
{
	OutConverter(){}
	OutConverter(OutConverter const&);

public:
	static OutConverter& getInstance()
	{
		static OutConverter instance;
		return instance;
	}

	void saveBMP(const char *filename, int w, int h, int dpi, pixel *data);

	virtual ~OutConverter();
};

