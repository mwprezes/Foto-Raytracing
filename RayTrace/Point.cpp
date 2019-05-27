#include "Point.h"



Point::Point()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point::Point(float xyz)
{
	this->x = xyz;
	this->y = xyz;
	this->z = xyz;
}

Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(Vector v)
{
	this->x = v.getX();
	this->y = v.getY();
	this->z = v.getZ();
}


Point::~Point()
{
}

Vector Point::makeVector(Point p1, Point p2)
{
	return Vector(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
}

Vector Point::makeVector(Point p1)
{
	return Vector(p1.x, p1.y, p1.z);
}

float Point::Distance(Point p1, Point p2)
{
	return sqrt(((p2.getX() - p1.getX()) * (p2.getX() - p1.getX())) + ((p2.getY() - p1.getY()) * (p2.getY() - p1.getY())) + ((p2.getZ() - p1.getZ()) * (p2.getZ() - p1.getZ())));
}
