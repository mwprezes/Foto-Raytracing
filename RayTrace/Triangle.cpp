#include "Triangle.h"

#define PLUS_ZERO 0.00001 
#define MINUS_ZERO -0.0001

Triangle::Triangle()
{
}

Triangle::Triangle(Point v1, Point v2, Point v3)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;

	plane = Plane(v1, v2, v3);
	color = LightIntensity(0, 1, 0);
}


Triangle::~Triangle()
{
}

int Triangle::intersect(Ray * ray)
{
	Ray tempRay = *ray;
	if (plane.intersect(tempRay) == -1)
		return -1;

	Vector fa, fb, fc;
	Vector x;

	fa = Point::makeVector(tempRay.getIntersection1(), v1);
	fb = Point::makeVector(tempRay.getIntersection1(), v2);
	fc = Point::makeVector(tempRay.getIntersection1(), v3);

	x = Vector::crossProduct(fa, fb);
	if (Vector::dotProduct(x, plane.getN()) < MINUS_ZERO)
		return -1;

	x = Vector::crossProduct(fb, fc);
	if (Vector::dotProduct(x, plane.getN()) < MINUS_ZERO)
		return -1;

	x = Vector::crossProduct(fc, fa);
	if (Vector::dotProduct(x, plane.getN()) < MINUS_ZERO)
		return -1;

	float xyz = (tempRay.getIntersection1() - ray->getOrigin()).lengthSquered();
	ray->addRayInersection(xyz, color);
	return 1;
}

void Triangle::makePlane()
{
	this->plane = Plane(v1, v2, v3);
}
