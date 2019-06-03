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
	if (plane.intersect(&tempRay) == -1)
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
	ray->addRayInersection(xyz, color, tempRay.getIntersection1());
	return 1;
}

void Triangle::makePlane()
{
	this->plane = Plane(v1, v2, v3);
}

Vector Triangle::getNormal()
{
	//Vector n = Vector::crossProduct(vn2 - vn1, vn3 - vn1);
	return plane.getN();
}

Vector Triangle::getNormal(float x, float y)
{
	Vector n = (1 - x - y) * vn1 + x * vn2 + y * vn3;
	n.normalize();
	return n;
}

Vector Triangle::getNormal(Vector intersection)
{
	/*Vector local_x = plane.getBase() * plane.getV();
	Vector local_y = plane.getBase() * plane.getU();
	float x = Vector::dotProduct(intersection, local_x);
	float y = Vector::dotProduct(intersection, local_y);
	Vector n = (1 - x - y) * vn1 + x * vn2 + y * vn3;
	n.normalize();
	return n;*/

	Vector v00 = v2 - v1, v11 = v3 - v1, v22 = intersection - v1;
	float d00 = Vector::dotProduct(v00, v00);
	float d01 = Vector::dotProduct(v00, v11);
	float d11 = Vector::dotProduct(v11, v11);
	float d20 = Vector::dotProduct(v22, v00);
	float d21 = Vector::dotProduct(v22, v11);
	float invDenom = 1.0 / (d00 * d11 - d01 * d01);

	float v = (d11 * d20 - d01 * d21) * invDenom;
	float w = (d00 * d21 - d01 * d20) * invDenom;
	float u = 1.0f - v - w;

	//float yy = (u * vn1.getX()) + (v * vn2.getX()) + (w * vn3.getX());
	//float xx = (u * vn1.getY()) + (v * vn2.getY()) + (w * vn3.getY());

	Vector n = u * vn1 + v * vn2 + w * vn3;
	n.normalize();

	return n;
}

Vector Triangle::getNormal(Point intersection)
{
	Vector v00 = v2 - v1, v11 = v3 - v1, v22 = intersection - v1;
	float d00 = Vector::dotProduct(v00, v00);
	float d01 = Vector::dotProduct(v00, v11);
	float d11 = Vector::dotProduct(v11, v11);
	float d20 = Vector::dotProduct(v22, v00);
	float d21 = Vector::dotProduct(v22, v11);
	float invDenom = 1.0 / (d00 * d11 - d01 * d01);

	float v = (d11 * d20 - d01 * d21) * invDenom;
	float w = (d00 * d21 - d01 * d20) * invDenom;
	float u = 1.0f - v - w;

	Vector n = u * vn1 + v * vn2 + w * vn3;
	n.normalize();

	return n;

	//return ((vn1 + vn2 + vn3) / 3).normalizeProduct();
	//return plane.getN();
}

void Triangle::setTexture(std::string map)
{
	material.map_Kd = map;
	bitmap_image image(map);

	int size = image.width() * image.height();
	for (int i = 0; i < size; i++)
		texture.push_back(Vector());
	imgWidth = image.width();
	imgHeight = image.height();

	for (int i = 0; i < image.width(); i++) {
		for (int j = 0; j < image.height(); j++) {
			texture[j*image.width() + i].setX(image.get_pixel(i, j).red);
			texture[j*image.width() + i].setY(image.get_pixel(i, j).green);
			texture[j*image.width() + i].setZ(image.get_pixel(i, j).blue);
			texture[j*image.width() + i] /= 255;
		}
	}
}

LightIntensity Triangle::MapTexture(Point intersect)
{
	int x = -1, y = -1;

	Vector v00 = v2 - v1, v11 = v3 - v1, v22 = intersect - v1;
	float d00 = Vector::dotProduct(v00, v00);
	float d01 = Vector::dotProduct(v00, v11);
	float d11 = Vector::dotProduct(v11, v11);
	float d20 = Vector::dotProduct(v22, v00);
	float d21 = Vector::dotProduct(v22, v11);
	float invDenom = 1.0 / (d00 * d11 - d01 * d01);

	float v = (d11 * d20 - d01 * d21) * invDenom;
	float w = (d00 * d21 - d01 * d20) * invDenom;
	float u = 1.0f - v - w;

	float yy = (u * vt1.getX()) + (v * vt2.getX()) + (w * vt3.getX());
	float xx = (u * vt1.getY()) + (v * vt2.getY()) + (w * vt3.getY());

	x = xx * imgWidth;
	y = yy * imgHeight;

	x = imgWidth - 1 - x;
	y = imgHeight - y;

	if (y > 300 || x > 299 || x < 0 || y < 0)
		float dsag = 1;

	LightIntensity fuckOperators = LightIntensity(0);
	color = fuckOperators + texture[x*imgWidth + y];


	return color;
}

/*float d1 = Point::Distance(v1, intersect);
float d2 = Point::Distance(v2, intersect);
float d3 = Point::Distance(v3, intersect);

float r1 = (1 / d1) / ((1 / d1) + (1 / d2) + (1 / d3));
float r2 = (1 / d2) / ((1 / d1) + (1 / d2) + (1 / d3));
float r3 = (1 / d3) / ((1 / d1) + (1 / d2) + (1 / d3));

/*Vector R(r1, r2, r3);

int x = Vector::dotProduct(R, vt1);
int y = Vector::dotProduct(R, vt2);

float xx = (r1 * vt1.getX()) + (r2 * vt2.getX()) + (r3 * vt3.getX());
float yy = (r1 * vt1.getY()) + (r2 * vt2.getY()) + (r3 * vt3.getY());

int x = xx * 400;
int y = yy * 400;*/

/*float u, v, w;

Vector v00 = Point::makeVector(v2 - v1), v11 = Point::makeVector(v3 - v1), v22 = Point::makeVector(intersect - v1);
float d00 = Vector::dotProduct(v00, v00);
float d01 = Vector::dotProduct(v00, v11);
float d11 = Vector::dotProduct(v11, v11);
float d20 = Vector::dotProduct(v22, v00);
float d21 = Vector::dotProduct(v22, v11);
float denom = d00 * d11 - d01 * d01;
v = (d11 * d20 - d01 * d21) / denom;
u = (d00 * d21 - d01 * d20) / denom;
w = 1.0f - v - u;

Vector uvw(u, v, w);
float xx = (u * vt1.getX()) + (u * vt2.getX()) + (u * vt3.getX());
float yy = (v * vt1.getX()) + (v * vt2.getX()) + (v * vt3.getX());

int x = xx * 400;
int y = yy * 400;*/

/*Vector C;

Vector edge1 = v2 - v1;
Vector vp1 = intersect - v1;

Vector edge2 = v3 - v2;
Vector vp2 = intersect - v2;

Vector edge3 = v1 - v3;
Vector vp3 = intersect - v3;

C = Vector::crossProduct(edge2, vp2);
float u = Vector::dotProduct(C, plane.getN());

C = Vector::crossProduct(edge3, vp3);
float v = Vector::dotProduct(C, plane.getN());

float denom = Vector::dotProduct(plane.getN(), plane.getN());

u /= denom;
v /= denom;

int x = u;
int y = v;*/