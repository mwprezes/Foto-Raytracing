#pragma once
#include <iostream>
#include "Primitive.h"

class Sphere : public Primitive
{
private:
	Point center;
	float radious;
	std::vector<Vector> texture;
	int imgWidth;
	int imgHeight;

public:
	Sphere();
	Sphere(Point p, float r);
	Sphere(float x, float y, float z, float r);
	~Sphere();
	std::string textureName;

	int intersect(Ray *ray);
	Vector getNormal(Point at);

	void setTexture(std::string map);
	LightIntensity MapTexture(Point intersect);
};

