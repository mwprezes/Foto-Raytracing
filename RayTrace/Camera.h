#pragma once
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

#pragma endregion

	void renderOrtho(bitmap_image img);
	void renderPersp(bitmap_image img);

};

