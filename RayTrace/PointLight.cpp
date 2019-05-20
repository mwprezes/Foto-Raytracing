#include "PointLight.h"
#define M_PI 3.14159265358979323846


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

void PointLight::illuminate(const Point & hitPoint, Vector & lightDir, LightIntensity & lightIntensity, float& intens)
{
	lightDir = -Point::makeVector(hitPoint, position);
	//I = lightDir.normalizeProduct();
	float dist2 = lightDir.lengthSquered();
	float intensDist = lightDir.length();
	lightDir /= intensDist;
	Vector inte = Vector(intensity.getR(), intensity.getG(), intensity.getB());
	intens = radiousIntesity;
	//lightIntensity = intensity * radiousIntesity;// / (4.0 * 3.14 * dist2);
	inte *= radiousIntesity / (4 * M_PI * dist2);
	//LightIntensity test = intensity * radiousIntesity;
	lightIntensity = intensity * (radiousIntesity / (4 * M_PI * dist2));
}
