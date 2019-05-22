#pragma once
#include "Primitive.h"

class Plane : public Primitive
{
private:
	Point base;
	Vector u, v;
	Vector n;
	Vector *texture;

	int imgWidth;
public:
	Plane();
	Plane(Point p, Vector u, Vector v);
	Plane(float x, float y, float z, float xu, float yu, float zu, float xv, float yv, float zv);
	Plane(Point p, Vector n);
	Plane(float x, float y, float z, float xn, float yn, float zn);
	Plane(Point p1, Point p2, Point p3);
	~Plane();

	std::string textureName;

	Point getBase() {
		return base;
	}
	Vector getU() {
		return u;
	}
	Vector getV() {
		return v;
	}
	Vector getN() {
		return n;
	}
	void setTexture(std::string map);

	int intersect(Ray& ray);
	void MapTexture(Point intersect);
	//r(s,t)=p+su+tv
};

