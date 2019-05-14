#pragma once
#include <iostream>
#include "Primitive.h"

class Sphere : public Primitive
{
private:
	Point center;
	float radious;

public:
	Sphere();
	Sphere(Point p, float r);
	Sphere(float x, float y, float z, float r);
	~Sphere();

	int intersect(Ray *ray);
	Vector getNormal(Point at);
};

