#pragma once
#include"LightIntensity.h"
#include "Point.h"

class LightSource
{
protected:
	LightIntensity intensity;

public:
	LightSource();
	LightSource(LightIntensity intensity);
	LightSource(float R, float G, float B);
	~LightSource();

	void setIntensity(LightIntensity light) {
		intensity = light;
	}

	LightIntensity getIntensity() {
		return intensity;
	}
};

