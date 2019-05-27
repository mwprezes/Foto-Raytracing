#pragma once
#include "Vector.h"
class Point :
	public Vector
{
public:
	Point();
	Point(float xyz);
	Point(float x, float y, float z);
	Point(Vector v);
	~Point();

	static Vector makeVector(Point p1, Point p2);
	static Vector makeVector(Point p1);
	static float Distance(Point p1, Point p2);

	#pragma region Operators

	friend std::ostream& operator<<(std::ostream &out, const Point& v)
	{
		out << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
		return out;
	}

	Point operator+(Point v) {
		Point vec(x + v.x, y + v.y, z + v.z);
		return vec;
	}
	Point operator-(Point v) {
		Point vec(x - v.x, y - v.y, z - v.z);
		return vec;
	}
	Point operator*(Point v) {
		Point vec(x * v.x, y * v.y, z * v.z);
		return vec;
	}
	Point operator/(Point v) {
		Point vec(x / v.x, y / v.y, z / v.z);
		return vec;
	}
	Point& operator+=(const Point& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}
	Point& operator-=(const Point& v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}
	Point& operator*=(const Point& v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		return *this;
	}
	Point& operator/=(const Point& v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		return *this;
	}
	bool operator==(Point v) {
		if (this->x == v.x && this->y == v.y && this->z == v.z)
			return true;
		else return false;
	}
	friend Point& operator-(const Point& v) {
		Point vec(v.x * -1, v.y * -1, v.z *-1);
		return vec;
	}
	friend Point& operator+(float val, const Point& v) {
		Point vec;
		vec.x = val + v.x;
		vec.y = val + v.y;
		vec.z = val + v.z;
		return vec;
	}
	friend Point& operator-(float val, const Point& v) {
		Point vec;
		vec.x = val - v.x;
		vec.y = val - v.y;
		vec.z = val - v.z;
		return vec;
	}
	friend Point& operator*(float val, const Point& v) {
		Point vec;
		vec.x = v.x * val;
		vec.y = v.y * val;
		vec.z = v.z * val;
		return vec;
	}
	friend Point& operator/(float val, const Point& v) {
		Point vec;
		vec.x = val / v.x;
		vec.y = val / v.y;
		vec.z = val / v.z;
		return vec;
	}

#pragma endregion

};

