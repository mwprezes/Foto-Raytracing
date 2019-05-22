#include "Plane.h"
#include "bitmap_image.hpp"

#define MINUS_ZERO -0.0001
#define PLUS_ZERO 0.00001 


Plane::Plane()
{
	base = Point(0);
	u = Vector(1, 0, 0);
	v = Vector(0, 1, 0);

	n = Vector::crossProduct(u, v);
}

Plane::Plane(Point p, Vector u, Vector v)
{
	base = p;
	this->u = u;
	this->v = v;
	n = Vector::crossProduct(u, v);
	n.normalize();
}

Plane::Plane(float x, float y, float z, float xu, float yu, float zu, float xv, float yv, float zv)
{
	Plane(Point(x, y, z), Vector(xu, yu, zu), Vector(xv, yv, zv));
}

Plane::Plane(Point p, Vector n)
{
	base = p;
	this->n = n;
}

Plane::Plane(float x, float y, float z, float xn, float yn, float zn)
{
	base = Point (x, y, z);
	n = Vector (xn, yn, zn);
}

Plane::Plane(Point p1, Point p2, Point p3)
{
	base = p1;
	u = Point::makeVector(p2, p1);
	v = Point::makeVector(p3, p1);
	n = Vector::crossProduct(u, v);
	n.normalize();
}


Plane::~Plane()
{
}

void Plane::setTexture(std::string map)
{
	material.map_Kd = map;
	textureName = map;
	bitmap_image image(map);

	texture = new Vector[image.width * image.height];
	imgWidth = image.width;

	for (int i = 0; i < image.width; i++) {
		for (int j = 0; j < image.height; j++) {
			texture[j*image.width + i].setX(image.get_pixel(i, j).red) / 255;
			texture[j*image.width + i].setY(image.get_pixel(i, j).green) / 255;
			texture[j*image.width + i].setZ(image.get_pixel(i, j).blue) / 255;
		}
	}
}

int Plane::intersect(Ray & ray)
{
	float d = Vector::dotProduct(n, Point::makeVector(base));
	float nv = (Vector::dotProduct(n, ray.getDirection()));
	/*if (nv == 0)
		return -1;*/
	if (nv >= MINUS_ZERO && nv <= PLUS_ZERO)
		return -1;

	float t = (d - Vector::dotProduct(n, Point::makeVector(ray.getOrigin()))) / nv;
	if (t < MINUS_ZERO)
		return -1;

	float x = ray.getOrigin().getX() + ray.getDirection().getX() * t;
	float y = ray.getOrigin().getY() + ray.getDirection().getY() * t;
	float z = ray.getOrigin().getZ() + ray.getDirection().getZ() * t;

	addIntersection1(Vector(x, y, z));
	//float xyz = (Vector(x, y, z) - ray.getOrigin()).lengthSquered();
	ray.addRayInersection(Vector(x, y, z), color);

	return 1;
}

void Plane::MapTexture(Point intersect)
{
	/*float z, x;
	float ut = (z + 1) / 2;
	float vv = (x - 1) / 2;*/

	int ut = Vector::dotProduct(u, intersect);
	int vt = Vector::dotProduct(v, intersect);

	LightIntensity fuckOperators = LightIntensity(0);
	color = fuckOperators + texture[ut*imgWidth + vt];
}


