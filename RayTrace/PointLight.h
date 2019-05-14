#pragma once
#include "LightSource.h"
class PointLight :
	public LightSource
{
private:
	float radiousIntesity = 1;
	Point position;
public:
	PointLight();
	PointLight(LightIntensity intensity, Point position);
	PointLight(LightIntensity intensity, Point position, float intens);
	~PointLight();

	void setPosition(Point point) {
		position = point;
	}
	Point getPosition() {
		return position;
	}

	void illuminate(const Point& hitPoint, Vector& lightDir, LightIntensity& lightIntensity);
};

