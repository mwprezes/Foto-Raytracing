#include "Ray.h"



Ray::Ray()
{
}

Ray::Ray(Point o, Vector dir)
{
	origin = o;
	direction = dir.normalizeProduct();
	color = LightIntensity(1.0, 1.0, 1.0);
}

Ray::Ray(float x, float y, float z, float xx, float yy, float zz)
{
	origin = Point(x, y, z);
	direction = Vector(xx, yy, zz);
	direction.normalize();
	color = LightIntensity(1.5, 1.5, 1.5);
}

Ray::~Ray()
{
}

void Ray::addRayInersection(Vector v, LightIntensity color)
{
	if (intersection1 == NULL)
	{
		intersection1 = v;
		this->color = color;
	}
	else
	{
		Vector s1 = v - origin;
		Vector s2 = intersection1 - origin;
		if (s1.length() < s2.length())
		{
			intersection1 = v;
			this->color = color;
		}
	}

}
