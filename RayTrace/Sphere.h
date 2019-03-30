#pragma once
#include <iostream>
#include "Primitive.h"

class Sphere : public Primitive
{
private:
	Point center;
	LightIntensity color;
	float radious;

public:
	Sphere();
	Sphere(Point p, float r);
	Sphere(float x, float y, float z, float r);
	~Sphere();

	void setColor(double R, double G, double B);
	int intersect(Ray& ray);
};

