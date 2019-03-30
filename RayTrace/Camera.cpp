#include "Camera.h"



Camera::Camera()
{
	pos = Point(0, 0, 0);
	dir = Vector(0, 0, 1);
	up = Vector(0, 1, 0);
	nearPlane = 1;
	farPlane = 1000;
	fov = 90;
	planeDistance = 10;
}

Camera::Camera(Point Pos, Vector Dir)
{
	pos = Pos;
	dir = Dir;
	up = Vector(0, 1, 0);
	nearPlane = 1;
	farPlane = 1000;
	fov = 90;
	planeDistance = 10;
}

Camera::Camera(Point Pos, Vector Dir, Vector Up, float near, float far, float whereveryouare)
{
	pos = Pos;
	dir = Dir;
	up = Up;
	nearPlane = near;
	farPlane = far;
	fov = whereveryouare;
	planeDistance = 10;
}

Camera::Camera(float x, float y, float z, float vx, float vy, float vz, float ux, float uy, float uz, float near, float far, float fov)
{
	Camera(Point(x, y, z), Vector(vx, vy, vz), Vector(ux, uy, uz), near, far, fov);
}


Camera::~Camera()
{
}

void Camera::renderOrtho(bitmap_image img)
{
	int planeHeight = 100;
	int planeWidth = 100;
	int hig = img.height();
	int wig = img.width();

	float pixelWidth = 2.0f / wig;
	float pixelHeight = 2.0f / hig;

	dir.normalize();
	up.normalize();

	Vector v1 = Vector::crossProduct(dir, up);
	v1.normalize();

	projectionPlane = Plane(pos, up, v1);

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Ray ray(Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU()), projectionPlane.getN());

			Sphere sphere(0, 0, 0, 30);

			int intersection = sphere.intersect(ray);

			if (intersection == -1) {
				img.set_pixel(i, j, 255, 255, 255);
			}
			else
				img.set_pixel(i, j, 200, 0, 0);

		}
	}

	img.save_image("renderO.jpg");
}

void Camera::renderPersp(bitmap_image img)
{
	int planeHeight = 20;
	int planeWidth = 20;
	int hig = img.height();
	int wig = img.width();

	float pixelWidth = 2.0f / wig;
	float pixelHeight = 2.0f / hig;

	//Vector v1 = Vector::crossProduct(up, dir);
	//v1 *= fov;
	//Vector v2 = up;
	//v2 *= fov;
	//Vector v3 = v1 - v2;
	//v3 += pos;
	//Point pbl(v3); //Point bottom left, not project base lerning :P

	//projectionPlane = Plane(pos + (dir * planeDistance), v1, v2);

	dir.normalize();
	up.normalize();

	Vector v1 = Vector::crossProduct(dir, up);
	v1.normalize();

	planeDistance = 20; //Tu musi byæ FOV, ¿eby zmieniaæ mo¿na ogniskow¹

	projectionPlane = Plane(pos + (dir * planeDistance), up, v1);

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Point pixelCenter = Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU());
			Vector toPixelDirection = Point::makeVector(pos, pixelCenter);
			toPixelDirection.normalize();

			Ray ray(pos, toPixelDirection);

			Sphere sphere(0, 0, 0, 30);

			int intersection = sphere.intersect(ray);

			if (intersection == -1) {
				img.set_pixel(i, j, 0, 0, 0);
			}
			else
				img.set_pixel(i, j, 200, 200, 200);
		}
	}

	img.save_image("renderP.jpg");
}
