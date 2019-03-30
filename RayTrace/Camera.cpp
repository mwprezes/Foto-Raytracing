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
			sphere.setColor(1, 0, 0);
			Sphere sphere2(20, 0, 50, 40);
			sphere2.setColor(0, 0, 1);

			int intersection = sphere.intersect(ray);
			int intersection2 = sphere2.intersect(ray);

			if (ray.getIntersection1() == NULL)
			{
				img.set_pixel(i, j, 255, 255, 255);
			}
			else
			{
				int R = int(ray.getColor().getR() * 255);
				int G = int(ray.getColor().getG() * 255);
				int B = int(ray.getColor().getB() * 255);
				img.set_pixel(i, j, R, G, B);
			}
		}
	}

	std::cout << "Render Ortho completed" << std::endl;
	img.save_image("renderO.jpg");
}

void Camera::renderPersp(bitmap_image img, int height, int width)
{
	int planeHeight = height;
	int planeWidth = width;
	int hig = img.height();
	int wig = img.width();

	float pixelWidth = 2.0f / wig;
	float pixelHeight = 2.0f / hig;

	dir.normalize();
	up.normalize();

	Vector v1 = Vector::crossProduct(dir, up);
	v1.normalize();
 
	planeDistance = (planeWidth / 2.0) / std::tan((fov * 3.14159 / 180) / 2.0);

	projectionPlane = Plane(pos, up, v1);
	pos = pos - (dir * planeDistance);

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Point pixelCenter = Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU());
			Vector toPixelDirection = Point::makeVector(pos, pixelCenter);
			toPixelDirection.normalize();

			Ray ray(pos, toPixelDirection);

			Sphere sphere(0, 0, 0, 30);
			sphere.setColor(1, 0, 0);
			Sphere sphere2(20, 0, 50, 40);
			sphere2.setColor(0, 0, 1);

			int intersection = sphere.intersect(ray);
			int intersection2 = sphere2.intersect(ray);

			if (ray.getIntersection1() == NULL)
			{
				img.set_pixel(i, j, 255, 255, 255);
			}
			else
			{
				int R = int(ray.getColor().getR() * 255);
				int G = int(ray.getColor().getG() * 255);
				int B = int(ray.getColor().getB() * 255);
				img.set_pixel(i, j, R, G, B);
			}
		}
	}

	std::cout << "Render Persp completed" << std::endl;
	img.save_image("renderP.jpg");
}
