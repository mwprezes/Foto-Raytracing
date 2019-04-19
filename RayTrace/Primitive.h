#pragma once
#include "Ray.h"

class Primitive
{
protected:
	//Materia³y
	Vector intersection1, intersection2;
	LightIntensity color;

public:
	Primitive();
	~Primitive();

	virtual int intersect(Ray *ray);
	void setColor(double R, double G, double B);

	Vector getIntersection1() {
		return intersection1;
	}
	Vector getIntersection2() {
		return intersection2;
	}
	void addIntersection1(Vector v) {
		intersection1 = v;
	}
	void addIntersection2(Vector v) {
		intersection2 = v;
	}
};

