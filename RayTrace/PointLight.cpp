#include "PointLight.h"



PointLight::PointLight()
{
	intensity = LightIntensity();
	position = Point();
}

PointLight::PointLight(LightIntensity intensity, Point position)
{
	this->intensity = intensity;
	this->position = position;
}


PointLight::~PointLight()
{
}
