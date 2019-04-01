#pragma once

#include <cmath>
#include <ostream>

class Vector
{
protected:
	float x, y, z;

public:
	Vector();
	Vector(float x, float y, float z);
	Vector(float xyz);
	//Vector(const Vector&);
	~Vector();

	#pragma region Getters/Setters
	float getX() {
		return x;
	}
	float getY() {
		return y;
	}
	float getZ() {
		return z;
	}
	void setX(float x) {
		this->x = x;
	}
	void setY(float y) {
		this->y = y;
	}
	void setZ(float z) {
		this->z = z;
	}
#pragma endregion

	#pragma region Methods

	float length();
	float lengthSquered();

	static Vector crossProduct(Vector v, Vector v2);
	static float dotProduct(Vector v, Vector v2);
	static Vector magProduct(Vector v, float mag);

	void normalize();
	Vector normalizeProduct();

	Vector reflect();
	Vector lerp(Vector v, float t);

#pragma endregion

	#pragma region Operators

	friend std::ostream& operator<<(std::ostream &out, const Vector& v) 
	{
		out << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
		return out;
	}

	Vector operator+(Vector v) {
		Vector vec(x + v.x, y + v.y, z + v.z);
		return vec;
	}
	Vector operator-(Vector v) {
		Vector vec(x - v.x, y - v.y, z - v.z);
		return vec;
	}
	Vector operator*(Vector v) {
		Vector vec(x * v.x, y * v.y, z * v.z);
		return vec;
	}
	Vector operator/(Vector v) {
		Vector vec(x / v.x, y / v.y, z / v.z);
		return vec;
	}
	/*Vector& operator+(const Vector& v) {
		Vector vec(x + v.x, y + v.y, z + v.z);
		return vec;
	}
	Vector& operator-(const Vector& v) {
		Vector vec(x - v.x, y - v.y, z - v.z);
		return vec;
	}
	Vector& operator*(const Vector& v) {
		Vector vec(x * v.x, y * v.y, z * v.z);
		return vec;
	}
	Vector& operator/(const Vector& v) {
		Vector vec(x / v.x, y / v.y, z / v.z);
		return vec;
	}*/
	Vector& operator+=(const Vector& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}
	Vector& operator-=(const Vector& v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}
	Vector& operator*=(const Vector& v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		return *this;
	}
	Vector& operator/=(const Vector& v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		return *this;
	}
	bool operator==(Vector v) {		
		if(this->x == v.x && this->y == v.y && this->z == v.z)
			return true;
		else return false;
	}
	bool operator!=(Vector v) {
		if (this->x != v.x || this->y != v.y || this->z != v.z)
			return true;
		else return false;
	}
	/*friend Vector& operator+(const Vector &v, const Vector &v2) {
	Vector vec;
	vec.x = v.x + v2.x;
	vec.y = v.y + v2.y;
	vec.z = v.z + v2.z;
	return vec;
	}*/
	friend Vector& operator-(const Vector& v) {
		Vector vec(v.x * -1, v.y * -1, v.z *-1);
		return vec;
	}
	/*friend Vector& operator*(const Vector& v, float val) {
		Vector vec;
		vec.x = v.x * val;
		vec.y = v.y * val;
		vec.z = v.z * val;
		return vec;
	}*/
	friend Vector operator+(float val, const Vector& v) {
		return Vector(val + v.x, val + v.y, val + v.z);
	}
	friend Vector operator-(float val, const Vector& v) {
		return Vector(val - v.x, val - v.y, val - v.z);
	}
	friend Vector operator*(float val, const Vector& v) {
		return Vector(val * v.x, val * v.y, val * v.z);
	}
	friend Vector operator/(float val, const Vector& v) {
		return Vector(val / v.x, val / v.y, val / v.z);
	}

#pragma endregion

};


/*Vector operator+(Vector left, Vector const & v) {
	Vector vec(left += v);
	return vec;
}
Vector operator-(Vector left, Vector const & v) {
	Vector vec(left -= v);
	return vec;
}
Vector operator*(Vector left, Vector const & v) {
	Vector vec(left *= v);
	return vec;
}
Vector operator/(Vector left, Vector const & v) {
	Vector vec(left /= v);
	return vec;
}*/