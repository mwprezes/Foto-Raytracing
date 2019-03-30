#include <iostream>
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "LightIntensity.h"
#include "Camera.h"


using namespace std;

int main(int argc, char *argv[])
{
	/*Vector a(1, 2, 1);
	Vector b(2,2,3);
	Vector c(3);
	Vector e(0);

	Vector d;

	//d = a + b - c;
	d = c.reflect();

	cout << d << endl;

	//cout << "Hello" << endl;*/

	/*Point a(10);
	Point b(6);
	Point c = a + b;

	cout << c << endl;*/

	//Zadanie 1\\

	Point p(0, 0, 0);
	Vector v(0, 1, 1);

	Ray ray(0, 0, -20, 0, 0, 1);
	Ray ray2(0, 0, -20, 0, 1, 0);
	Ray ray3(10, 0, -10, -1, 0, 0);
	Sphere sphere(0, 0, 0, 10);
	Plane plane(0, 0, 0, 0, 1, 1);
	Plane plane2(p, v);

	int i = sphere.intersect(ray);
	int j = sphere.intersect(ray2);
	int k = sphere.intersect(ray3);
	int l = plane.intersect(ray2);

	//R1 & Sphere
	cout << "Sphere & R1: ";
	if (i < 0)
		cout << "No intersection" << endl;
	else if (i == 0)
		cout << "1 intersection at: " << sphere.getIntersection1() << endl;
	else
		cout << "2 intersections at: " << "first: " <<sphere.getIntersection1() << "and second: " << sphere.getIntersection2() << endl;

	//R2 & Sphere
	cout << "Sphere & R2: ";
	if (j < 0)
		cout << "No intersection" << endl;
	else if (j == 0)
		cout << "1 intersection at: " << sphere.getIntersection1() << endl;
	else
		cout << "2 intersections at: " << sphere.getIntersection1() << "and " << sphere.getIntersection2() << endl;

	//R3 & Sphere
	cout << "Sphere & R3: ";
	if (k < 0)
		cout << "No intersection" << endl;
	else if (k == 0)
		cout << "1 intersection at: " << sphere.getIntersection1() << endl;
	else
		cout << "2 intersections at: " << sphere.getIntersection1() << "and " << sphere.getIntersection2() << endl;

	//R2 & Plane
	cout << "Plane & R2: ";
	if (l < 0)
		cout << "No intersection" << endl;
	else
		cout << "Intersections at: " << plane.getIntersection1() << endl;
	getchar();

	//Zadanie 2\\

	//LightIntensity li(1);
	//LightIntensity li2(1, 2, 3);
	//cout << "LightTest " << li - li2 << endl;

	Camera cam(Point(50, 0, -50), Vector(-1, 0, 1));
	bitmap_image img(512, 512);

	cam.renderOrtho(img);

	cam = Camera(Point(50, 0, -50), Vector(-1, 0, 1));
	cam.setFov(10);
	cam.setPDist(10);
	img = bitmap_image(512, 512);

	cam.renderPersp(img);

	getchar();
	return 0;
}