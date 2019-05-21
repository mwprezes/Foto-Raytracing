#include "Ray.h"



Ray::Ray()
{
}

Ray::Ray(Point o, Vector dir)
{
	origin = o;
	direction = dir.normalizeProduct();
	color = LightIntensity(1.0, 1.0, 1.0);
	distance = 0;
}

Ray::Ray(float x, float y, float z, float xx, float yy, float zz)
{
	origin = Point(x, y, z);
	direction = Vector(xx, yy, zz);
	direction.normalize();
	color = LightIntensity(1.5, 1.5, 1.5);
	distance = 0;
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
		primIndex = potentialIndex;
		intersects = true;
	}
	else
	{
		Vector s1 = v - origin;
		Vector s2 = intersection1 - origin;
		if (s1.length() < s2.length())
		{
			intersection1 = v;
			this->color = color;
			primIndex = potentialIndex;
		}
	}
}

void Ray::addRayInersection(float v, LightIntensity color)
{
	if (distance == 0 || distance == NULL)
	{
		distance = v;
		this->color = color;
		primIndex = potentialIndex;
		intersects = true;
	}
	else
	{
		if (v < distance)
		{
			distance = v;
			this->color = color;
			primIndex = potentialIndex;
		}
	}
}

void Ray::addRayInersection(float v, LightIntensity color, Vector point)
{
	if (distance == 0 || distance == NULL)
	{
		distance = v;
		intersection1 = point;
		this->color = color;
		primIndex = potentialIndex;
		intersects = true;
	}
	else
	{
		if (v < distance)
		{
			distance = v;
			intersection1 = point;
			this->color = color;
			primIndex = potentialIndex;
		}
	}
}
