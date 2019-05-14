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

PointLight::PointLight(LightIntensity intensity, Point position, float intens)
{
	this->intensity = intensity;
	this->position = position;
	radiousIntesity = intens;
}


PointLight::~PointLight()
{
}

void PointLight::illuminate(const Point & hitPoint, Vector & lightDir, LightIntensity & lightIntensity)
{
	lightDir = Point::makeVector(position, hitPoint);
	//I = lightDir.normalizeProduct();
	float dist2 = lightDir.lengthSquered();
	float intensDist = lightDir.length();
	lightDir /= intensDist;
	lightIntensity = intensity * radiousIntesity; //* intensity / (4.0 * 3.14 * dist2);
}
