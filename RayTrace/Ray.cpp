#include "Ray.h"



Ray::Ray()
{
}

Ray::Ray(Point o, Vector dir)
{
	origin = o;
	direction = dir.normalizeProduct();
}

Ray::Ray(float x, float y, float z, float xx, float yy, float zz)
{
	origin = Point(x, y, z);
	direction = Vector(xx, yy, zz);
	direction.normalize();
}


Ray::~Ray()
{
}
