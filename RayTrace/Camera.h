#pragma once
#include <cmath>
#include <iostream>
#include <conio.h>
#include "Point.h"
#include "bitmap_image.hpp"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"


class Camera
{
protected:
	Point pos;
	Vector dir, up;
	float nearPlane, farPlane;
	float fov;
	Plane projectionPlane;
	float planeDistance;
	int planeHeight;
	int planeWidth;
	Sphere sphere;
	Sphere sphere2;
	int stop;
	bool antiAliastingOn;

public:
	Camera();
	Camera(Point Pos, Vector Dir);
	Camera(Point Pos, Vector Dir, Vector Up, float near, float far, float whereveryouare);
	Camera(float x, float y, float z, float vx, float vy, float vz, float ux, float uy, float uz, float near, float far, float fov);
	~Camera();

	#pragma region Getters/Setters

	Point getPos() {
		return pos;
	}
	Vector getDir() {
		return dir;
	}
	Vector getUp() {
		return up;
	}
	float getNear() {
		return nearPlane;
	}
	float getFar() {
		return farPlane;
	}
	float getFov() {
		return fov;
	}
	bool getAntiAliasingOn() {
		return antiAliastingOn;
	}
	void setPos(float x, float y, float z) {
		pos = Point(x, y, z);
	}
	void setDir(float x, float y, float z) {
		dir = Vector(x, y, z);
	}
	void setUp(float x, float y, float z) {
		up = Vector(x, y, z);
	}
	void setNear(float val) {
		nearPlane = val;
	}
	void setFar(float val) {
		farPlane = val;
	}
	void setFov(float val) {
		fov = val;
	}
	void setPDist(float val) {
		planeDistance = val;
	}
	void setAntiAliasingOn(bool val) {
		antiAliastingOn = val;
	}

#pragma endregion

	void renderOrtho(bitmap_image img, int height, int width);
	void renderPersp(bitmap_image img, int height, int width);
	LightIntensity samplingOrtho(Ray rayMed, Ray rayTL, Ray rayTR, Ray rayBL, Ray rayBR, float height, float width, int maxStop);
	LightIntensity samplingPersp(Point center, Point TL, Point TR, Point BL, Point BR, Ray rayTL, Ray rayTR, Ray rayBL, Ray rayBR, float height, float width, int maxStop);
};

