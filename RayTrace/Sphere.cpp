#include "Sphere.h"

Sphere::Sphere()
{
	center = Point(0);
	radious = 1;
	color = LightIntensity(1.0, 0.0, 0.0);
}

Sphere::Sphere(Point p, float r)
{
	center = p;
	radious = r;
	color = LightIntensity(1.0, 0.0, 0.0);
}

Sphere::Sphere(float x, float y, float z, float r)
{
	Point p(x, y, z);
	center = p;
	radious = r;
	color = LightIntensity(1.0, 0.0, 0.0);
}

Sphere::~Sphere()
{
}

int Sphere::intersect(Ray *ray)
{
	Vector oc = Point::makeVector(center, ray->getOrigin());
	float loc = -Vector::dotProduct(ray->getDirection(), oc);

	float det = loc * loc - Vector::dotProduct(oc, oc) + radious * radious;

	if (det < 0)
		return -1;

	det = sqrt(det);
	float d1 = (loc - det);
	float d2 = (loc + det);

	if (d1 == d2)
	{
		//ray.addIntersection(Vector(ray.getDirection() * d1));
		if (loc < 0)
			return -1;
		float x = ray->getOrigin().getX() + ray->getDirection().getX() * d1;
		float y = ray->getOrigin().getY() + ray->getDirection().getY() * d1;
		float z = ray->getOrigin().getZ() + ray->getDirection().getZ() * d1;
		//ray.addIntersection1(Vector(x, y, z));
		//ray.addRayInersection(Vector(x, y, z), color);
		float xyz = (Vector(x, y, z) - ray->getOrigin()).lengthSquered();
		ray->addRayInersection(xyz, color, Vector(x, y, z));
		addIntersection1(Vector(x, y, z));
		return 0;
	}
	else
	{
		if (loc <= 0)
			return -1;
		float x = ray->getOrigin().getX() + ray->getDirection().getX() * d1;
		float y = ray->getOrigin().getY() + ray->getDirection().getY() * d1;
		float z = ray->getOrigin().getZ() + ray->getDirection().getZ() * d1;
		//ray.addIntersection1(Vector(x, y, z));
		Vector v1(x, y, z);
		addIntersection1(Vector(x, y, z));
		float xyz = (Vector(x, y, z) - ray->getOrigin()).lengthSquered();
		ray->addRayInersection(xyz, color, Vector(x, y, z));

		x = ray->getOrigin().getX() + ray->getDirection().getX() * d2;
		y = ray->getOrigin().getY() + ray->getDirection().getY() * d2;
		z = ray->getOrigin().getZ() + ray->getDirection().getZ() * d2;
		//ray.addRayInersection(Vector(x, y, z),color);
		Vector v2(x, y, z);
		addIntersection2(Vector(x, y, z));

		/*if ((v1 - ray.getOrigin()).length() <= (v2 - ray.getOrigin()).length())
			ray.addRayInersection(v1, color);
		else
			ray.addRayInersection(v2, color);*/

		return 1;
	}
}

Vector Sphere::getNormal(Point at)
{
	Vector normal = -Point::makeVector(at, center);
	return normal.normalizeProduct();
}
