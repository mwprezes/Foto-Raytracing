#pragma once
#include "Point.h"
#include "LightIntensity.h"

class Ray
{
private:
	Point origin;
	Vector direction;
	LightIntensity color;

	//std::list<Vector> intersections;
	Vector intersection1, intersection2;

public:
	Ray();
	Ray(Point o, Vector dir);
	Ray(float x, float y, float z, float xx, float yy, float zz);
	~Ray();

	float distance;
	bool intersects = false;
	int potentialIndex = -1;
	int primIndex = -1;

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
	LightIntensity getColor() {
		return color;
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
	void setColour(LightIntensity li) {
		color = li;
	}
	
#pragma endregion

	void addRayInersection(Vector v, LightIntensity color);
	void addRayInersection(float v, LightIntensity color);
	void addRayInersection(float v, LightIntensity color, Vector point);
};

