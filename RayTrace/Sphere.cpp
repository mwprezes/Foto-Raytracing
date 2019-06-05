#include "Sphere.h"
#define MINUS_ZERO -0.0001
#define M_PI 3.14159265358979323846

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

	if (det < MINUS_ZERO)
		return -1;

	det = sqrt(det);
	float d1 = (loc - det);
	float d2 = (loc + det);

	if (d1 == d2)
	{
		//ray.addIntersection(Vector(ray.getDirection() * d1));
		if (loc < MINUS_ZERO)
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
		if (loc <= MINUS_ZERO)
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

void Sphere::setTexture(std::string map)
{
	material.map_Kd = map;
	textureName = map;
	bitmap_image image(map);

	int size = image.width() * image.height();
	//texture.reserve(size);
	for (int i = 0; i < size; i++)
		texture.push_back(Vector());

	imgWidth = image.width();
	imgHeight = image.height();

	for (int i = 0; i < image.width(); i++) {
		for (int j = 0; j < image.height(); j++) {
			int ind = j * image.width() + i;
			texture[ind].setX(image.get_pixel(i, j).red);
			texture[ind].setY(image.get_pixel(i, j).green);
			texture[ind].setZ(image.get_pixel(i, j).blue);
			texture[ind] /= 255;
		}
	}
}

/*LightIntensity Sphere::MapTexture(Point intersect)
{
	LightIntensity fin;

	float theta = std::acos(getNormal(intersect).normalizeProduct().getY());
	float phi = std::atan2(getNormal(intersect).normalizeProduct().getX(), getNormal(intersect).normalizeProduct().getZ());
	if (phi < MINUS_ZERO)
		phi += (M_PI * 2.0);

	float u = phi / (2.0 * M_PI);
	float v = 1.0 - theta / M_PI;

	int col = (int)((imgWidth - 1)*u);
	int row = (int)((imgHeight - 1)*v);

	LightIntensity fuckOperators = LightIntensity(0);
	fin = fuckOperators + texture[row*imgWidth + col];

	return fin;
}*/

LightIntensity Sphere::MapTexture(Point intersect)
{
	LightIntensity fin;

	Vector norm = -getNormal(intersect).normalizeProduct();

	float u = 0.5 + std::atan2(norm.getX(), norm.getZ()) / (2.0 * M_PI);
	float v = 0.5 - std::asin(-norm.getY()) / (M_PI);

	int col = u * imgWidth;
	int row = v * imgHeight;

	int i = row * imgWidth + col;

	LightIntensity fuckOperators = LightIntensity(0);
	fin = fuckOperators + texture[i];

	return fin;
}
