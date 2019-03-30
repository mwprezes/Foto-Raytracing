#include "Vector.h"



Vector::Vector()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(float xyz)
{
	this->x = xyz;
	this->y = xyz;
	this->z = xyz;
}

/*Vector::Vector(const Vector& v)
{
	Vector(v.x, v.y, v.z);
}*/

Vector::~Vector()
{
}

float Vector::length()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

float Vector::lengthSquered()
{
	return abs((x * x) + (y * y) + (z * z));
}

Vector Vector::crossProduct(Vector v, Vector v2)
{
	return Vector((v.y*v2.z) - (v.z*v2.y), (v.z*v2.x) - (v.x*v2.z), (v.x*v2.y) - (v.y*v2.x));
}

float Vector::dotProduct(Vector v, Vector v2)
{
	return ((v.x * v2.x) + (v.y * v2.y) + (v.z * v2.z));
}

Vector Vector::magProduct(Vector v, float mag)
{
	return Vector(v.x*mag, v.y*mag, v.z*mag);
}

void Vector::normalize()
{
	float n = this->length();
	if (n != 0) 
	{
		this->x /= n;
		this->y /= n;
		this->z /= n;
	}
	else 
	{
		//Nie dziel przez 0
	}
}

Vector Vector::normalizeProduct()
{
	Vector vec(*this);
	float n = this->length();

	if (n != 0) 
	{
		vec.x /= n;
		vec.y /= n;
		vec.z /= n;
		return vec;
	}
	else {
		//Nie dziel przez 0
	}

	return Vector(-INFINITY);
}

Vector Vector::reflect()
{
	Vector vec(*this);

	vec.normalize();
	vec = *this - (2 * Vector::dotProduct(*this, vec) * vec);

	return vec;
}

Vector Vector::lerp(Vector v, float t)
{
	Vector vec;

	vec.x = this->x + (t * (v.x - this->x));
	vec.y = this->y + (t * (v.y - this->y));
	vec.z = this->z + (t * (v.z - this->z));

	return vec;
}
