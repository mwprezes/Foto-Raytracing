#pragma once
#include "LightSource.h"
class PointLight :
	public LightSource
{
private:
	Point position;
public:
	PointLight();
	PointLight(LightIntensity intensity, Point position);
	~PointLight();

	void setPosition(Point point) {
		position = point;
	}
	Point getPosition() {
		return position;
	}
};

