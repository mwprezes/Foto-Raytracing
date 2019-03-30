#pragma once
#include "Point.h"

class Ray
{
private:
	Point origin;
	Vector direction;

	//std::list<Vector> intersections;
	Vector intersection1, intersection2;

public:
	Ray();
	Ray(Point o, Vector dir);
	Ray(float x, float y, float z, float xx, float yy, float zz);
	~Ray();

	#pragma region Getter/Setter

	Point getOrigin() {
		return origin;
	}
	Vector getDirection() {
		return direction;
	}
	Vector getIntersection1() {
		return intersection1;
	}
	Vector getIntersection2() {
		return intersection2;
	}
	void setOrigin(Point p) {
		origin = p;
	}
	void setOrigin(float x, float y, float z) {
		origin = Point(x, y, z);
	}
	void setDirection(Vector v) {
		direction = v;
	}
	void setDirection(float x, float y, float z) {
		direction = Vector(x, y, z);
	}
	void addIntersection1(Vector v) {
		intersection1 = v;
	}
	void addIntersection2(Vector v) {
		intersection2 = v;
	}

#pragma endregion



};

