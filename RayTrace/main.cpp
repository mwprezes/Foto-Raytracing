#include <iostream>
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "LightIntensity.h"
#include "Camera.h"
#include "ObjLoader.h"

#include "PointLight.h"

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

	int i = sphere.intersect(&ray);
	int j = sphere.intersect(&ray2);
	int k = sphere.intersect(&ray3);
	int l = plane.intersect(&ray2);

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
	
	int height = 200;
	int width = 200;

	Camera *cam = new Camera(Point(0, 0, -100), Vector(0, 0, 1));
	cam->setAntiAliasingOn(0);
	bitmap_image img(1024, 1024);
	cam->setFilename("renderOTriangle.jpg");

	cam->setScene(3);
	cam->getScene()->addPrimitive(new Sphere(-30, 0, 0, 30));
	cam->getScene()->getPrimitive(0)->setColor(0, 0, 1);

	cam->getScene()->addPrimitive(new Sphere(30, 0, 0, 30));
	cam->getScene()->getPrimitive(1)->setColor(1, 0, 0);

	cam->getScene()->addPrimitive(new Triangle(Point(-30, 0, -50), Point(30, 0, -50), Point(0, 50, -50)));
	cam->getScene()->getPrimitive(2)->setColor(0, 1, 0);

	//cam->renderOrtho(img, height, width);

	cam = new Camera(Point(0, 0, -30), Vector(0, 0, 1));
	cam->setAntiAliasingOn(0);
	cam->setFov(45);
	img = bitmap_image(1024, 1024);
	cam->setFilename("renderPTriangle.jpg");

	cam->setScene(3);
	cam->getScene()->addPrimitive(new Sphere(-30, 0, 0, 30));
	cam->getScene()->getPrimitive(0)->setColor(0, 0, 1);

	cam->getScene()->addPrimitive(new Sphere(30, 0, 0, 30));
	cam->getScene()->getPrimitive(1)->setColor(1, 0, 0);

	cam->getScene()->addPrimitive(new Triangle(Point(-30, 0, -50), Point(30, 0, -50), Point(0, 50, -50)));
	cam->getScene()->getPrimitive(2)->setColor(0, 1, 0);

	//cam->renderPersp(img, height, width);

	getchar();

	//Zadanie 3\\

	ObjLoader loader;
	cout << "Loader: " << loader.LoadOBJ("TexPlane.obj") << endl;
	
	Mesh mesh;
	mesh.setTriangles(loader.triangles);
	mesh.setMaterial(loader.mat);

	cam = new Camera(Point(30, 30, -30), Vector(-1, -1, 1));
	cam->setAntiAliasingOn(0);
	cam->setFov(45);
	img = bitmap_image(1024, 1024);
	cam->setFilename("renderMesh.jpg");

	cam->setScene(mesh.getTriangles().size());
	for (int i = 0; i < mesh.getTriangles().size(); i++)
	{
		cam->getScene()->addPrimitive(new Triangle(mesh.getTriangle(i)));
		cam->getScene()->getPrimitive(i)->setColor(mesh.getMaterial().Kd.getX(), mesh.getMaterial().Kd.getY(), mesh.getMaterial().Kd.getZ());
	}

	//cam->renderPersp(img, height, width);

	getchar();

	//Zadanie 4\\

	cam = new Camera(Point(50, 50, -50), Vector(0, 0, 1));
	//cam->LookAt(Point(0, 0, 0));
	//cam = new Camera(Point(50, 0, -50), Point(0, 0, 0));
	cam->setAntiAliasingOn(0);
	cam->setFov(45);
	img = bitmap_image(1024, 1024);

	cam->setScene(2);
	cam->getScene()->addPrimitive(new Sphere(0, 0, 0, 40));
	cam->getScene()->getPrimitive(0)->setColor(0, 0, 1);

	cam->getScene()->addPrimitive(new Sphere(10, -70, 0, 20));
	cam->getScene()->getPrimitive(1)->setColor(0, 0, 1);
	
	/*cam->setScene(mesh.getTriangles().size());
	for (int i = 0; i < mesh.getTriangles().size(); i++)
	{
		cam->getScene()->addPrimitive(new Triangle(mesh.getTriangle(i)));
		cam->getScene()->getPrimitive(i)->setColor(mesh.getMaterial().Kd.getX(), mesh.getMaterial().Kd.getY(), mesh.getMaterial().Kd.getZ());
		cam->getScene()->getPrimitive(i)->setMat(mesh.getMaterial().name, mesh.getMaterial().Ka, mesh.getMaterial().Kd, mesh.getMaterial().Ks, mesh.getMaterial().Ns, mesh.getMaterial().illum, mesh.getMaterial().Tr, mesh.getMaterial().d);
	}*/

	cam->getScene()->addLight(new PointLight(LightIntensity(1,1,1), Point(0, 40, -60), 50000));

	//cam->setFilename("renderMeshPhongO.jpg");
	//cam->renderOrtho(img, height, width);
	cam->setFilename("renderMeshPhongS2.jpg");
	//cam->renderPersp(img, height, width);

	getchar();

	//Zadanie 5\\

	cam = new Camera(Point(0, 0, -50), Vector(0, 0, 1));
	//cam->LookAt(Point(0, 0, 0));
	//cam = new Camera(Point(50, 0, -50), Point(0, 0, 0));
	cam->setAntiAliasingOn(0);
	cam->setFov(45);
	img = bitmap_image(1024, 1024);

	/*cam->setScene(1);
	cam->getScene()->addPrimitive(new Plane(Point(20, 10, 0), Point(0, 0.5, 0), Point(0.5, 0, 0)));
	//cam->getScene()->addPrimitive(new Plane(Point(20, -10, 0), Vector(-10, 0, 0).normalizeProduct(), Vector(0, -10, 0).normalizeProduct()));
	//cam->getScene()->addPrimitive(new Sphere(0, 0, 0, 40));
	cam->getScene()->getPrimitive(0)->setColor(0, 0, 1);
	cam->getScene()->getPrimitive(0)->setTexture("maps/tex.bmp");*/

	cam->setScene(mesh.getTriangles().size());
	for (int i = 0; i < mesh.getTriangles().size(); i++)
	{
		cam->getScene()->addPrimitive(new Triangle(mesh.getTriangle(i)));
		cam->getScene()->getPrimitive(i)->setColor(mesh.getMaterial().Kd.getX(), mesh.getMaterial().Kd.getY(), mesh.getMaterial().Kd.getZ());
		cam->getScene()->getPrimitive(i)->setMat(mesh.getMaterial().name, mesh.getMaterial().Ka, mesh.getMaterial().Kd, mesh.getMaterial().Ks, mesh.getMaterial().Ns, mesh.getMaterial().illum, mesh.getMaterial().Tr, mesh.getMaterial().d);
		//cam->getScene()->getPrimitive(i)->setTexture(mesh.getMaterial().map_Kd);
		//cam->getScene()->getPrimitive(i)->setTexture("maps/tex.bmp");
	}

	cam->getScene()->addLight(new PointLight(LightIntensity(1, 1, 1), Point(0, 0, -20), 5000));

	cam->setFilename("PlaneTest.jpg");
	//cam->renderPersp(img, height, width);
	getchar();

	//Zadanie 6\\

	cam = new Camera(Point(0, 0, -100), Vector(0, 0, 1));
	//cam->LookAt(Point(0, 0, 0));
	//cam = new Camera(Point(50, 0, -50), Point(0, 0, 0));
	cam->setAntiAliasingOn(0);
	cam->setFov(45);
	img = bitmap_image(1024, 1024);

	cam->setScene(4);
	cam->getScene()->addPrimitive(new Sphere(42, 0, 0, 40));
	cam->getScene()->getPrimitive(0)->setColor(0, 0, 1);
	cam->getScene()->getPrimitive(0)->isMirror = true;

	cam->getScene()->addPrimitive(new Sphere(-32, 0, 0, 30));
	cam->getScene()->getPrimitive(1)->setColor(0, 0, 1);

	/*cam->getScene()->addPrimitive(new Sphere(0, 0, 200, 100));
	cam->getScene()->getPrimitive(2)->setColor(0, 0, 1);
	cam->getScene()->getPrimitive(2)->setMatColour(Vector(0.6f, 0.0f, 0.1f), Vector(0.9f, 0.0f, 0.2f), Vector(0.4f, 0.0f, 0.0f));*/
	//cam->getScene()->addPrimitive(new Plane(Point(0, 0, 100), Vector(0, 1, 0), Vector(1, 0, 0)));
	cam->getScene()->addPrimitive(new Plane(Point(0, -80, 100), Vector(0, 1, -0.3f)));
	cam->getScene()->getPrimitive(2)->setColor(0, 0, 1);
	cam->getScene()->getPrimitive(2)->setMatColour(Vector(0.6f, 0.0f, 0.1f), Vector(0.9f, 0.0f, 0.2f), Vector(0.4f, 0.0f, 0.0f));

	cam->getScene()->addPrimitive(new Sphere(100, 0, -50, 40));
	cam->getScene()->getPrimitive(3)->setColor(0, 0, 1);

	cam->getScene()->addLight(new PointLight(LightIntensity(1, 1, 1), Point(0, 0, -50), 50000));

	cam->setMaxReyReflect(2);
	cam->setFilename("Last.jpg");
	cam->renderPersp(img, height, width);

	getchar();

	return 0;
}