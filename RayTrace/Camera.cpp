#include "Camera.h"
#include <cmath>

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
	delete scene;
}

void Camera::renderOrtho(bitmap_image img, int height, int width)
{
	planeHeight = height;
	planeWidth = width;
	int hig = img.height();
	int wig = img.width();

	float pixelWidth = 2.0f / wig;
	float pixelHeight = 2.0f / hig;
	dir.normalize();
	up.normalize();

	Vector v1 = Vector::crossProduct(dir, up);
	v1.normalize();

	projectionPlane = Plane(pos, up, v1);

	/*sphere = Sphere(0, 0, 0, 60);
	sphere.setColor(0, 0, 1);
	sphere2 = Sphere(30, 0, 0, 60);
	sphere2.setColor(1, 0, 0);*/

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Ray ray = Ray(Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU()), projectionPlane.getN());
			/*sphere.intersect(&ray);
			sphere2.intersect(&ray);*/

			for (int i = 0; i < scene->getAddIndex(); i++) {
				scene->getPrimitive(i)->intersect(&ray);
			}

			LightIntensity pixelColor;

			if (antiAliastingOn)
			{
				stop = 0;
				pixelColor = samplingOrtho(ray, Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), pixelHeight, pixelWidth, 4);
			}
			else
				pixelColor = ray.getColor();

			int R = int(pixelColor.getR() * 255);
			int G = int(pixelColor.getG() * 255);
			int B = int(pixelColor.getB() * 255);

			img.set_pixel(i, j, R, G, B);
		}
		if (i % 5 == 0)
			std::cout << ".";
	}

	std::cout << "Render Ortho completed" << std::endl;
	img.save_image(filename);
}

void Camera::renderPersp(bitmap_image img, int height, int width)
{
	planeHeight = height;
	planeWidth = width;
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

	/*sphere = Sphere(0, 0, 0, 60);
	sphere.setColor(0, 0, 1);
	sphere2 = Sphere(30, 0, 0, 60);
	sphere2.setColor(1, 0, 0);*/

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Point pixelCenter = Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU());
			Vector toPixelDirection = Point::makeVector(pos, pixelCenter);
			toPixelDirection.normalize();
			Ray ray(pos, toPixelDirection);
			/*sphere.intersect(&ray);
			sphere2.intersect(&ray);*/

			LightIntensity PhongColor;

			for (int i = 0; i < scene->getAddIndex(); i++) {
				scene->getPrimitive(i)->intersect(&ray);
				if (ray.intersects) 
				{
					/*//Triangle *testT = static_cast<Triangle*>(scene->getPrimitive(i));
					Triangle testT = *(Triangle*)scene->getPrimitive(i);
					std::string testS = typeid(testT).name();*/
					/*std::string testS = typeid(static_cast<Triangle*>(scene->getPrimitive(i))).name();
					if (testS == "Triangle")
						PhongColor = PhongTriangle(ray, *(Triangle*)scene->getPrimitive(i), height, width);*/
					if (typeid(static_cast<Triangle*>(scene->getPrimitive(i))) == typeid(Triangle*))
						PhongColor = PhongTriangle(ray, *(Triangle*)scene->getPrimitive(i), height, width);
				}
			}

			LightIntensity pixelColor;

			if (antiAliastingOn)
			{
				stop = 0;				
				pixelColor = samplingPersp(pixelCenter, NULL, NULL, NULL, NULL, Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), pixelHeight, pixelWidth, 4);
			}
			else
				pixelColor = ray.getColor();

			/*int R = int(pixelColor.getR() * 255);
			int G = int(pixelColor.getG() * 255);
			int B = int(pixelColor.getB() * 255);
			img.set_pixel(i, j, R, G, B);*/

			int R = int(PhongColor.getR() * 255);
			int G = int(PhongColor.getG() * 255);
			int B = int(PhongColor.getB() * 255);
			img.set_pixel(i, j, R, G, B);

		}
		if (i % 5 == 0)
			std::cout << ".";
	}

	std::cout << "Render Persp completed" << std::endl;
	img.save_image(filename);
}

LightIntensity Camera::samplingOrtho(Ray rayMed, Ray rayTL, Ray rayTR, Ray rayBL, Ray rayBR, float height, float width, int maxStop)
{
	width = width * 0.5;
	height = height * 0.5;

	LightIntensity pixelColor;

	if (rayTL.getDirection() != rayMed.getDirection())
		rayTL = Ray(Point(rayMed.getOrigin() - width * planeWidth * projectionPlane.getV() + height * planeHeight * projectionPlane.getU()), projectionPlane.getN());
	if (rayTR.getDirection() != rayMed.getDirection())
		rayTR = Ray(Point(rayMed.getOrigin() + width * planeWidth * projectionPlane.getV() + height * planeHeight * projectionPlane.getU()), projectionPlane.getN());
	if (rayBL.getDirection() != rayMed.getDirection())
		rayBL = Ray(Point(rayMed.getOrigin() - width * planeWidth * projectionPlane.getV() - height * planeHeight * projectionPlane.getU()), projectionPlane.getN());
	if (rayBR.getDirection() != rayMed.getDirection())
		rayBR = Ray(Point(rayMed.getOrigin() + width * planeWidth * projectionPlane.getV() - height * planeHeight * projectionPlane.getU()), projectionPlane.getN());

	stop += 1;

	//sphere.intersect(&rayMed);
	//sphere2.intersect(&rayMed);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayMed);
	}
	LightIntensity color0 = rayMed.getColor();

	//sphere.intersect(&rayTL);
	//sphere2.intersect(&rayTL);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayTL);
	}
	LightIntensity color1 = rayTL.getColor();

	//sphere.intersect(&rayTR);
	//sphere2.intersect(&rayTR);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayTR);
	}
	LightIntensity color2 = rayTR.getColor();

	//sphere.intersect(&rayBL);
	//sphere2.intersect(&rayBL);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayBL);
	}
	LightIntensity color3 = rayBL.getColor();

	//sphere.intersect(&rayBR);
	//sphere2.intersect(&rayBR);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayBL);
	}
	LightIntensity color4 = rayBR.getColor();

	//Antyaliasing
	if (stop < maxStop)
	{
		if (color1 != color0)
		{
			Ray raySamp(Point(rayTL.getOrigin() + width * 0.5 * planeWidth * projectionPlane.getV() - height * 0.5 * planeHeight * projectionPlane.getU()), projectionPlane.getN());
			color1 = samplingOrtho(raySamp, rayTL, Ray(Point(0, 0, 0), Vector(0, 0, 0)), Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayMed, height, width, maxStop);
		}
		if (color2 != color0)
		{
			Ray raySamp(Point(rayTR.getOrigin() - width * 0.5 * planeWidth * projectionPlane.getV() - height * 0.5 * planeHeight * projectionPlane.getU()), projectionPlane.getN());
			color2 = samplingOrtho(raySamp, Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayTR, rayMed, Ray(Point(0, 0, 0), Vector(0, 0, 0)), height, width, maxStop);
		}
		if (color3 != color0)
		{
			Ray raySamp(Point(rayBL.getOrigin() + width * 0.5 * planeWidth * projectionPlane.getV() + height * 0.5 * planeHeight * projectionPlane.getU()), projectionPlane.getN());
			color3 = samplingOrtho(raySamp, Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayMed, rayBL, Ray(Point(0, 0, 0), Vector(0, 0, 0)), height, width, maxStop);
		}
		if (color4 != color0)
		{
			Ray raySamp(Point(rayBR.getOrigin() - width * 0.5 * planeWidth * projectionPlane.getV() + height * 0.5 * planeHeight * projectionPlane.getU()), projectionPlane.getN());
			color4 = samplingOrtho(raySamp, rayMed, Ray(Point(0, 0, 0), Vector(0, 0, 0)), Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayBR, height, width, maxStop);
		}
	}

	color0 = color0 * 0.5;
	color1 = color1 * 0.5;
	color2 = color2 * 0.5;
	color3 = color3 * 0.5;
	color4 = color4 * 0.5;

	color1 = color1 + color0;
	color2 = color2 + color0;
	color3 = color3 + color0;
	color4 = color4 + color0;

	color1 = color1 * 0.25;
	color2 = color2 * 0.25;
	color3 = color3 * 0.25;
	color4 = color4 * 0.25;

	pixelColor = color1 + color2 + color3 + color4;
	return pixelColor;
}

LightIntensity Camera::samplingPersp(Point center, Point TL, Point TR, Point BL, Point BR, Ray rayTL, Ray rayTR, Ray rayBL, Ray rayBR, float height, float width, int maxStop)
{
	width = width * 0.5;
	height = height * 0.5;

	LightIntensity pixelColor;

	Vector toPixelDirection = Point::makeVector(pos, center);
	toPixelDirection.normalize();
	Ray rayMed(pos, toPixelDirection);

	Point pixelTL = Point(center - width * planeWidth * projectionPlane.getV() + height * planeHeight * projectionPlane.getU());
	Point pixelTR = Point(center + width * planeWidth * projectionPlane.getV() + height * planeHeight * projectionPlane.getU());
	Point pixelBL = Point(center - width * planeWidth * projectionPlane.getV() - height * planeHeight * projectionPlane.getU());
	Point pixelBR = Point(center + width * planeWidth * projectionPlane.getV() - height * planeHeight * projectionPlane.getU());

	if (pixelTL != TL)
	{		
		TL = pixelTL;
		Vector toPixelDirectionTL = Point::makeVector(pos, pixelTL);
		toPixelDirectionTL.normalize();
		rayTL = Ray(pos, toPixelDirectionTL);
	}	
	if (pixelTR != TR)
	{	
		TR = pixelTR;
		Vector toPixelDirectionTR = Point::makeVector(pos, pixelTR);
		toPixelDirectionTR.normalize();
		rayTR = Ray(pos, toPixelDirectionTR);
	}	
	if (pixelBL != BL)
	{		
		BL = pixelBL;
		Vector toPixelDirectionBL = Point::makeVector(pos, pixelBL);
		toPixelDirectionBL.normalize();
		rayBL = Ray(pos, toPixelDirectionBL);
	}	
	if (pixelBR != BR)
	{		
		BR = pixelBR;
		Vector toPixelDirectionBR = Point::makeVector(pos, pixelBR);
		rayBR = Ray(pos, toPixelDirectionBR);
		toPixelDirectionBR.normalize();
	}	

	//sphere.intersect(&rayMed);
	//sphere2.intersect(&rayMed);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayMed);
	}
	LightIntensity color0 = rayMed.getColor();

	//sphere.intersect(&rayTL);
	//sphere2.intersect(&rayTL);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayTL);
	}
	LightIntensity color1 = rayTL.getColor();

	//sphere.intersect(&rayTR);
	//sphere2.intersect(&rayTR);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayTR);
	}
	LightIntensity color2 = rayTR.getColor();

	//sphere.intersect(&rayBL);
	//sphere2.intersect(&rayBL);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayBL);
	}
	LightIntensity color3 = rayBL.getColor();

	//sphere.intersect(&rayBR);
	//sphere2.intersect(&rayBR);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		scene->getPrimitive(i)->intersect(&rayBL);
	}
	LightIntensity color4 = rayBR.getColor();

	stop += 1;

	//Antyaliasing
	if (stop < maxStop)
	{
		if (color1 != color0)
		{
			Point centerSamp = Point(pixelTL + width * 0.5 * planeWidth * projectionPlane.getV() - height * 0.5 * planeHeight * projectionPlane.getU());
			color1 = samplingPersp(centerSamp, TL, NULL, NULL, center, rayTL, Ray(Point(0, 0, 0), Vector(0, 0, 0)), Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayMed, height, width, maxStop);
		}
		if (color2 != color0)
		{
			Point centerSamp = Point(pixelTR - width * 0.5 * planeWidth * projectionPlane.getV() - height * 0.5 * planeHeight * projectionPlane.getU());
			color2 = samplingPersp(centerSamp, NULL, TR, center, NULL, Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayTR, rayMed, Ray(Point(0, 0, 0), Vector(0, 0, 0)), height, width, maxStop);

		}
		if (color3 != color0)
		{
			Point centerSamp = Point(pixelBL + width * 0.5 * planeWidth * projectionPlane.getV() + height * 0.5 * planeHeight * projectionPlane.getU());
			color3 = samplingPersp(centerSamp, NULL, center, BL, NULL, Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayMed, rayBL, Ray(Point(0, 0, 0), Vector(0, 0, 0)), height, width, maxStop);

		}
		if (color4 != color0)
		{
			Point centerSamp = Point(pixelBR - width * 0.5 * planeWidth * projectionPlane.getV() + height * 0.5 * planeHeight * projectionPlane.getU());
			color4 = samplingPersp(centerSamp, center, NULL, NULL, BR, rayMed, Ray(Point(0, 0, 0), Vector(0, 0, 0)), Ray(Point(0, 0, 0), Vector(0, 0, 0)), rayBR, height, width, maxStop);
		}
	}

	color0 = color0 * 0.5;
	color1 = color1 * 0.5;
	color2 = color2 * 0.5;
	color3 = color3 * 0.5;
	color4 = color4 * 0.5;

	color1 = color1 + color0;
	color2 = color2 + color0;
	color3 = color3 + color0;
	color4 = color4 + color0;

	color1 = color1 * 0.25;
	color2 = color2 * 0.25;
	color3 = color3 * 0.25;
	color4 = color4 * 0.25;

	pixelColor = color1 + color2 + color3 + color4;
	return pixelColor;
}

LightIntensity Camera::Phong(Ray & ray, Primitive& shape, float height, float width)
{
	Vector specular;
	double r, g, b, cos;
	Vector I, N, R;

	I = ray.getDirection().normalizeProduct();
	N = shape.getNormal();
	R = I - (N * Vector::crossProduct(N, I) * 2.0f);

	Vector ss = Vector::crossProduct(ray.getDirection().normalizeProduct(), R);
	if (-ss.getX() > 0 && -ss.getY() > 0 && -ss.getZ() > 0) {
		specular.setX(std::pow(ss.getX(), shape.getMat().Ka.getX()));
		specular.setY(std::pow(ss.getY(), shape.getMat().Ka.getY()));
		specular.setZ(std::pow(ss.getZ(), shape.getMat().Ka.getZ()));
	}
	else
		specular = 0;
	for (LightSource *light : scene->getLights())
	{
		LightIntensity intense = light->getIntensity() * specular;
		//cos = Vector.dot
	}

	return LightIntensity();
}

/*LightIntensity Camera::PhongTriangle(Ray & ray, Triangle & shape, float height, float width)
{
	LightIntensity fin = LightIntensity();

	Vector kd = shape.getMat().Kd;
	Vector ks = shape.getMat().Ks;
	Vector ka = shape.getMat().Ka;
	float ns = shape.getMat().Ns;

	Vector specular;
	double r, g, b, cos, k = 1;
	Vector I, N, R;
	for (LightSource light : scene->getLights())
	{
		PointLight *li = (PointLight*)&light;
		Vector l = ray.getIntersection1() - li->getPosition();
		N = shape.getNormal(ray.getIntersection1());
		I = -1.0*(ray.getIntersection1() - ray.getOrigin());

		l.normalize();
		N.normalize();
		I.normalize();

		Vector nl = Vector::dotProduct(N, l);
		R = (2 * nl*N) - l;
		R.normalize();
		Vector temp = (kd*nl + ks * std::powf(Vector::dotProduct(I, R), ns));
		fin += light.getIntensity() * temp;
	}

	return fin;
}*/

/*LightIntensity Camera::PhongTriangle(Ray & ray, Triangle & shape, float height, float width)
{
	LightIntensity fin = LightIntensity();

	Vector kd = shape.getMat().Kd;
	Vector ks = shape.getMat().Ks;
	Vector ka = shape.getMat().Ka;
	float ns = shape.getMat().Ns;
	float d = shape.getMat().d;

	LightIntensity fuckOperators = LightIntensity(0);
	LightIntensity kds = fuckOperators + kd;
	LightIntensity kss = fuckOperators + ks;
	LightIntensity kas = fuckOperators + ka;

	Vector specular;
	double r, g, b, cos;
	Vector I, N, R;
	Vector l;

	for (LightSource light : scene->getLights())
	{
		PointLight *li = (PointLight*)&light;

		//I = ray.getDirection().normalizeProduct();
	I = (li->getPosition() - ray.getIntersection1()).normalizeProduct();
	N = shape.getNormal();
	//R = I - ((N * Vector::dotProduct(N, I)) * 2.0f);
	R = (2.0f*Vector::dotProduct(N, I)*N) - I;

	float ss = Vector::dotProduct(ray.getDirection(), R);
	if (-ss > 0) {
		specular = Vector(std::pow(ss, ns));
	}
	else
		specular = Vector(0);

	specular *= ks;

		//l = ray.getIntersection1() - li->getPosition();
		LightIntensity sIntense = light.getIntensity() * specular;
		cos = Vector::dotProduct(ray.getDirection().normalizeProduct(), N);
		r = -light.getIntensity().getR() * d * cos;
		g = -light.getIntensity().getG() * d * cos;
		b = -light.getIntensity().getB() * d * cos;

		LightIntensity dIntense = LightIntensity(r, g, b) + ka;
		fin += sIntense + dIntense;
		//Vector temp = (kd*ni + ks * std::powf(ss, ns));
		//fin = light.getIntensity() * temp;
		LightIntensity temp1 = (kds * std::max(0.0f, Vector::dotProduct(N, li->getPosition())));
		//LightIntensity temp2 = (kss*std::pow(std::max(0.0f, Vector::dotProduct(R, li->getPosition())), ns));
		LightIntensity temp2 = fuckOperators + specular;
		fin = kas + temp1 + temp2;
	}

	return fin;
}*/

LightIntensity Camera::PhongTriangle(Ray & ray, Triangle & shape, float height, float width)
{
	LightIntensity fin = LightIntensity();

	Vector kd = shape.getMat().Kd;
	Vector ks = shape.getMat().Ks;
	Vector ka = shape.getMat().Ka;
	float ns = shape.getMat().Ns;
	float d = shape.getMat().d;

	LightIntensity fuckOperators = LightIntensity(0);
	LightIntensity kds = fuckOperators + kd;
	LightIntensity kss = fuckOperators + ks;
	LightIntensity kas = fuckOperators + ka;
	Vector lightDir;
	LightIntensity intens;

	Vector specular;
	double r, g, b, cos;
	Vector I, N, R;
	Vector l;

	LightIntensity diff = 0, spec = 0;

	for (LightSource* light : scene->getLights())
	{
		PointLight *li = static_cast<PointLight*>(light);

		float lightDistance = 0;
		li->illuminate(ray.getIntersection1(), I, intens);

		N = shape.getNormal();
		R = I - (2.0f*Vector::dotProduct(N, I)*N);

		diff += d * std::max(0.0f, Vector::dotProduct(N, -I)) * intens;
		spec += std::pow(std::max(0.0f, Vector::dotProduct(R, -ray.getDirection().normalizeProduct())), ns) * intens;
	}

	fin = diff * kds + spec * kss;
	LightIntensity test = LightIntensity(0);
	if (fin == test)
		l = 1;

	return fin;
}