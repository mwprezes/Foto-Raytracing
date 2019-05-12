#include "LightSource.h"



LightSource::LightSource()
{
	intensity = LightIntensity();
}

LightSource::LightSource(LightIntensity intensity)
{
	this->intensity = intensity;
}

LightSource::LightSource(float R, float G, float B)
{
	intensity = LightIntensity(R, G, B);
}


LightSource::~LightSource()
{
}
