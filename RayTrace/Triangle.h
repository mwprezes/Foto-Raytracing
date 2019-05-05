#pragma once
#include "Primitive.h"
#include "Plane.h"
class Triangle :
	public Primitive
{
private:
	Plane plane;
public:
	Point v1, v2, v3;
	Point vt1, vt2, vt3;
	Point vn1, vn2, vn3;

	Triangle();
	Triangle(Point v1, Point v2, Point v3);
	~Triangle();

	int intersect(Ray *ray);
	void makePlane();
};

