#pragma once
#include "Ray.h"
#include "bitmap_image.hpp"

struct PrimitiveMaterial
{
	std::string name;
	Vector Ka;
	Vector Kd;
	Vector Ks;
	// Specular Exponent
	float Ns;
	// Illumination
	float illum;
	// Transparent
	float Tr;
	// Dissolve
	float d;

	std::string map_Kd;

	PrimitiveMaterial() {
		name = "Default";
		Ka = Vector(0.3f);
		Kd = Vector(0.5f);
		Ks = Vector(0.8f);
		Ns = 32;
		illum = 2;
		Tr = 0;
		d = 1;
	}
	PrimitiveMaterial(std::string name, Vector Ka, Vector Kd, Vector Ks, float Ns, float illum, float Tr, float d) {
		this->name = name;
		this->Ka = Ka;
		this->Kd = Kd;
		this->Ks = Ks;
		this->Ns = Ns;
		this->illum = illum;
		this->Tr = Tr;
		this->d = d;
	}
};

class Primitive
{
protected:
	//Materia³y
	Vector intersection1, intersection2;
	LightIntensity color;
	Vector normal;

	PrimitiveMaterial material;

public:
	Primitive();
	~Primitive();

	virtual int intersect(Ray *ray);
	void setColor(double R, double G, double B);
	void setMat(std::string name, Vector Ka, Vector Kd, Vector Ks, float Ns, float illum, float Tr, float d);

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
	void setMat(PrimitiveMaterial mat) {
		material = mat;
	}

	Vector getNormal() {
		return normal;
	}
	Vector getNormal(Point intersection) {
		return normal;
	}
	PrimitiveMaterial getMat() {
		return material;
	}
	/*void setTexture(std::string map) {
		material.map_Kd = map;
	}

	LightIntensity MapTexture(Point intersect) {
		return LightIntensity();
	}*/

	virtual void setTexture(std::string map);
	virtual LightIntensity MapTexture(Point intersect);
};

