#pragma once
#include "LightSource.h"
#include "Sphere.h"

class SurfaceLight :
	public LightSource
{
private:
	Sphere surface;
public:
	SurfaceLight();
	~SurfaceLight();
};

