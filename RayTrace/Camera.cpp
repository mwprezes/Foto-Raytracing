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
	dir = Dir.normalizeProduct();
	up = Vector(0, 1, 0);
	nearPlane = 1;
	farPlane = 1000;
	fov = 90;
	planeDistance = 10;
}

Camera::Camera(Point Pos, Point lookAt)
{
	pos = Pos;
	LookAt(lookAt);
	up = Vector(0, 1, 0);
	nearPlane = 1;
	farPlane = 1000;
	fov = 90;
	planeDistance = 10;
}

Camera::Camera(Point Pos, Vector Dir, Vector Up, float near, float far, float whereveryouare)
{
	pos = Pos;
	dir = Dir.normalizeProduct();
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

Vector Camera::LookAt(Point lookAt)
{
	this->dir = Point::makeVector(pos, lookAt).normalizeProduct();
	return dir;
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
			LightIntensity PhongColor;

			for (int k = 0; k < scene->getAddIndex(); k++) {
				ray.potentialIndex = k;
				scene->getPrimitive(k)->intersect(&ray);
			}
			if (ray.intersects)
			{
				/*if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Triangle))
					PhongColor = PhongTriangle(ray, *(Triangle*)scene->getPrimitive(ray.primIndex), 0);
				else if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Sphere))
					PhongColor = PhongSphere(ray, *(Sphere*)scene->getPrimitive(ray.primIndex), 0);
				else if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Plane))
					PhongColor = PhongPlane(ray, *(Plane*)scene->getPrimitive(ray.primIndex), 0);
				//PhongColor = Phong(ray, scene->getPrimitive(i), height, width);*/

				PhongColor = Phong(ray, *scene->getPrimitive(ray.primIndex), 0);

				//if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Sphere))
				if (scene->getPrimitive(ray.primIndex)->getMat().map_Kd != "")
					PhongColor *= scene->getPrimitive(ray.primIndex)->MapTexture(ray.getIntersection1());
			}

			LightIntensity pixelColor;

			if (antiAliastingOn)
			{
				stop = 0;
				pixelColor = samplingOrtho(ray, Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), pixelHeight, pixelWidth, 4);
			}
			else
				pixelColor = ray.getColor();

			/*int R = int(pixelColor.getR() * 255);
			int G = int(pixelColor.getG() * 255);
			int B = int(pixelColor.getB() * 255);*/

			int R = int(PhongColor.getR() * 255);
			int G = int(PhongColor.getG() * 255);
			int B = int(PhongColor.getB() * 255);

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
	//planeDistance = 5000;
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
			int intersectsAt = -1;

			for (int k = 0; k < scene->getAddIndex(); k++) {
				ray.potentialIndex = k;
				scene->getPrimitive(k)->intersect(&ray);
			}
			if (ray.intersects)
			{
				PhongColor = Phong(ray, *scene->getPrimitive(ray.primIndex), 0);

				/*if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Triangle))
					PhongColor = PhongTriangle(ray, *(Triangle*)scene->getPrimitive(ray.primIndex), 0);
				else if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Sphere))
					PhongColor = PhongSphere(ray, *(Sphere*)scene->getPrimitive(ray.primIndex), 0);
				else if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Plane))
					PhongColor = PhongPlane(ray, *(Plane*)scene->getPrimitive(ray.primIndex), 0);*/

				if (scene->getPrimitive(ray.primIndex)->getMat().map_Kd != "")
					PhongColor += scene->getPrimitive(ray.primIndex)->MapTexture(ray.getIntersection1());

				ray.setColour(PhongColor);
			}
			//LightIntensity pixelColor;

			if (antiAliastingOn)
			{
				stop = 0;				
				PhongColor = samplingPersp(pixelCenter, NULL, NULL, NULL, NULL, Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), Ray(Point(0, 0, 0), -ray.getDirection()), pixelHeight, pixelWidth, 4);
			}
			else
				PhongColor = ray.getColor();

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
		rayMed.potentialIndex = i;
		scene->getPrimitive(i)->intersect(&rayMed);
	}
	LightIntensity color0 = Phong(rayMed, *scene->getPrimitive(rayMed.primIndex), 0);

	//sphere.intersect(&rayTL);
	//sphere2.intersect(&rayTL);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		rayTL.potentialIndex = i;
		scene->getPrimitive(i)->intersect(&rayTL);
	}
	LightIntensity color1 = Phong(rayTL, *scene->getPrimitive(rayTL.primIndex), 0);

	//sphere.intersect(&rayTR);
	//sphere2.intersect(&rayTR);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		rayTR.potentialIndex = i;
		scene->getPrimitive(i)->intersect(&rayTR);
	}
	LightIntensity color2 = Phong(rayTR, *scene->getPrimitive(rayTR.primIndex), 0);

	//sphere.intersect(&rayBL);
	//sphere2.intersect(&rayBL);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		rayBL.potentialIndex = i;
		scene->getPrimitive(i)->intersect(&rayBL);
	}
	LightIntensity color3 = Phong(rayBL, *scene->getPrimitive(rayBL.primIndex), 0);

	//sphere.intersect(&rayBR);
	//sphere2.intersect(&rayBR);
	for (int i = 0; i < scene->getAddIndex(); i++) {
		rayBR.potentialIndex = i;
		scene->getPrimitive(i)->intersect(&rayBR);
	}
	LightIntensity color4 = Phong(rayBR, *scene->getPrimitive(rayBR.primIndex), 0);

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

LightIntensity Camera::Phong(Ray & ray, Primitive& shape, int reflectionNumber)
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

	//kss = 0.08;
	//ns = 10;


	Vector lightIntTest;
	float intensity;
	LightIntensity intens;

	Vector I, N, R;

	LightIntensity diff = 0, spec = 0;
	int rayRefection = reflectionNumber;
	bool shadow = false;

	if ((!shape.isMirror && !shape.isRefracting) || rayRefection >= maxAllowedRayReflections)
	{
		for (LightSource* light : scene->getLights())
		{
			PointLight *li = static_cast<PointLight*>(light);

			float lightDistance = 0;
			li->illuminate(ray.getIntersection1(), I, intens, intensity);

			N = shape.getNormal(ray.getIntersection1());
			//R = I - (2.0f*Vector::dotProduct(N, I)*N);
			R = 2.0f*(Vector::dotProduct(N, I)*N) - I;

			Ray toLight(ray.getIntersection1(), I);
			for (int i = 0; i < scene->getAddIndex(); i++) {
				toLight.potentialIndex = i;
				if (i != ray.primIndex)
					scene->getPrimitive(i)->intersect(&toLight);
			}
			if (toLight.intersects) {
				double rayToIntersection = Point::makeVector(toLight.getIntersection1(), ray.getIntersection1()).lengthSquered(), rayToLight = Point::makeVector(li->getPosition(), ray.getIntersection1()).lengthSquered();
				if (rayToIntersection < rayToLight) {
					//intens = 0;
					shadow = true;
				}
			}
			diff += d * std::max(0.0f, Vector::dotProduct(N, I)) * intens;
			spec += std::pow(std::max(Vector::dotProduct(-R, dir), 0.0f), ns) * intens;
		}
		fin = diff * kds + spec * kss + kas;
	}
	else if (shape.isMirror && rayRefection < maxAllowedRayReflections)
	{
		//Ray mirror = ray;
		I = ray.getDirection();
		N = shape.getNormal(ray.getIntersection1());
		R = I - (2.0f*Vector::dotProduct(N, I)*N);

		Ray reflected(ray.getIntersection1(), R);

		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (reflected.intersects) {
			if (reflected.primIndex == 2)
				float sa = 1;
			fin += Phong(reflected, *scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}
	else if (shape.isRefracting && rayRefection < maxAllowedRayReflections)
	{
		float refFactor = 1.52f;
		I = ray.getDirection();
		N = shape.getNormal(ray.getIntersection1());
		double eta = 1.0f / refFactor;
		double ni = Vector::dotProduct(I, N);
		R = ((I - N * ni)*eta) - (N * std::sqrt(1 - (eta *eta) *(ni*ni)));

		Ray reflected(ray.getIntersection1(), R);

		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (exitSphere)
			exitSphere = false;

		if (reflected.intersects) {
			fin += Phong(reflected, *scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}

	if (shadow)
		fin = LightIntensity(0.07f);
	return fin;
}

LightIntensity Camera::PhongPlane(Ray & ray, Plane & shape, int reflectionNumber)
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

	//kss = 0.08;
	//ns = 10;

	Vector lightIntTest;
	float intensity;
	LightIntensity intens;

	Vector I, N, R;

	LightIntensity diff = 0, spec = 0;
	int rayRefection = reflectionNumber;

	if ((!shape.isMirror && !shape.isRefracting) || rayRefection >= maxAllowedRayReflections)
	{
		for (LightSource* light : scene->getLights())
		{
			PointLight *li = static_cast<PointLight*>(light);

			float lightDistance = 0;
			li->illuminate(ray.getIntersection1(), I, intens, intensity);

			N = shape.getN();
			//R = I - (2.0f*Vector::dotProduct(N, I)*N);
			R = 2.0f*(Vector::dotProduct(N, I)*N) - I;

			Ray toLight(ray.getIntersection1(), -I);
			for (int i = 0; i < scene->getAddIndex(); i++) {
				if (i != ray.primIndex)
					scene->getPrimitive(i)->intersect(&toLight);
			}
			if (toLight.intersects)
				if (Point::makeVector(toLight.getIntersection1(), li->getPosition()).lengthSquered() < Point::makeVector(ray.getIntersection1(), li->getPosition()).lengthSquered())
					intens = 0;
			diff += d * std::max(0.0f, Vector::dotProduct(N, I)) * intens;
			spec += std::pow(std::max(Vector::dotProduct(R, dir), 0.0f), ns) * intens;
		}

		fin = diff * kds + spec * kss + kas;
	}
	else if (shape.isMirror && rayRefection < maxAllowedRayReflections)
	{
		//Ray mirror = ray;
		I = ray.getDirection();
		N = shape.getN();
		R = I - (2.0f*Vector::dotProduct(N, I)*N);

		Ray reflected(ray.getIntersection1(), R);

		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (reflected.intersects) {
			if (reflected.primIndex == 2)
				float sa = 1;
			fin += PhongPlane(reflected, *(Plane*)scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}
	else if (shape.isRefracting && rayRefection < maxAllowedRayReflections)
	{
		float refFactor = 1.52f;
		I = ray.getDirection();
		N = shape.getN();
		//R = (1 * (I - N * Vector::dotProduct(I, N))) / refFactor - N * std::sqrt(1 - ((1 * 1 * (1 - (Vector::dotProduct(I, N) * Vector::dotProduct(I, N)))) / (refFactor * refFactor)));
		float fi = 1.0f / refFactor;
		float ni = Vector::dotProduct(I, N);
		//R = fi * I + N * std::sqrt(1 - fi * fi*(1 - ni * ni)) - fi * N*ni;
		R = std::sqrt(1 - fi * fi*(1 - ni * ni)) *N + fi * (I - ni * N);

		Ray reflected(ray.getIntersection1(), R);

		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (reflected.intersects) {
			fin += PhongPlane(reflected, *(Plane*)scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}

	return fin;
}

LightIntensity Camera::PhongSphere(Ray & ray, Sphere & shape, int reflectionNumber)
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

	//kss = 0.08;
	//ns = 10;

	Vector lightIntTest;
	float intensity;
	LightIntensity intens;

	Vector I, N, R;

	LightIntensity diff = 0, spec = 0;
	int rayRefection = reflectionNumber;

	if ((!shape.isMirror && !shape.isRefracting) || rayRefection >= maxAllowedRayReflections)
	{
		for (LightSource* light : scene->getLights())
		{
			PointLight *li = static_cast<PointLight*>(light);

			float lightDistance = 0;
			li->illuminate(ray.getIntersection1(), I, intens, intensity);

			N = shape.getNormal(ray.getIntersection1());
			//R = I - (2.0f*Vector::dotProduct(N, I)*N);
			R = 2.0f*(Vector::dotProduct(N, I)*N) - I;

			Ray toLight(ray.getIntersection1(), -I);
			for (int i = 0; i < scene->getAddIndex(); i++) {
				toLight.potentialIndex = i;
				if (i != ray.primIndex)
					scene->getPrimitive(i)->intersect(&toLight);
			}
			if (toLight.intersects)
				if (Point::makeVector(toLight.getIntersection1(), li->getPosition()).lengthSquered() < Point::makeVector(ray.getIntersection1(), li->getPosition()).lengthSquered())
					intens = 0;

			diff += d * std::max(0.0f, Vector::dotProduct(N, I)) * intens;
			spec += std::pow(std::max(Vector::dotProduct(R, dir), 0.0f), ns) * intens;
		}
		fin = diff * kds + spec * kss + kas;
	}
	else if (shape.isMirror && rayRefection < maxAllowedRayReflections)
	{
		//Ray mirror = ray;
		I = ray.getDirection();
		N = shape.getNormal(ray.getIntersection1());
		R = I - (2.0f*Vector::dotProduct(N, I)*N);

		Ray reflected(ray.getIntersection1(), R);

		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (reflected.intersects) {
			if (reflected.primIndex == 2)
				float sa = 1;
			fin += PhongSphere(reflected, *(Sphere*)scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}
	else if (shape.isRefracting && rayRefection < maxAllowedRayReflections)
	{
		float refFactor = 1.52f;
		I = ray.getDirection();
		N = shape.getNormal(ray.getIntersection1());
		//R = (1 * (I - N * Vector::dotProduct(I, N))) / refFactor - N * std::sqrt(1 - ((1 * 1 * (1 - (Vector::dotProduct(I, N) * Vector::dotProduct(I, N)))) / (refFactor * refFactor)));
		float fi = 1.0f / refFactor;
		float ni = Vector::dotProduct(I, N);
		//R = fi * I + N * std::sqrt(1 - fi * fi*(1 - ni * ni)) - fi * N*ni;
		R = std::sqrt(1 - fi * fi*(1 - ni * ni)) *N + fi * (I - ni * N);

		Ray reflected(ray.getIntersection1(), R);



		if (typeid(*scene->getPrimitive(ray.primIndex)) == typeid(Sphere) && !exitSphere) {
			scene->getPrimitive(ray.primIndex)->intersect(&reflected);
			exitSphere = true;
			fin += PhongSphere(reflected, *(Sphere*)scene->getPrimitive(reflected.primIndex), rayRefection);
		}
		else
		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (reflected.intersects) {
			fin += PhongSphere(reflected, *(Sphere*)scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}


	return fin;
}

/*LightIntensity Camera::PhongTriangle(Ray & ray, Triangle & shape, float height, float width) // http://www.scratchapixel.com/code.php?id=32&origin=/lessons/3d-basic-rendering/phong-shader-BRDF
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

LightIntensity Camera::PhongTriangle(Ray & ray, Triangle & shape, int reflectionNumber)
{
	LightIntensity fin = LightIntensity();

	Vector kd = shape.getMat().Kd;
	Vector ks = shape.getMat().Ks;
	Vector ka = shape.getMat().Ka;
	float ns = shape.getMat().Ns;
	float d = shape.getMat().d;
	//ns = 128;
	LightIntensity fuckOperators = LightIntensity(0);
	LightIntensity kds = fuckOperators + kd;
	LightIntensity kss = fuckOperators + ks;
	LightIntensity kas = fuckOperators + ka;

	Vector lightIntTest;
	float intensity;
	LightIntensity intens;

	Vector I, N, R;

	LightIntensity diff = 0, spec = 0, ambi = 0;
	int rayRefection = reflectionNumber;

	if ((!shape.isMirror && !shape.isRefracting) || rayRefection >= maxAllowedRayReflections)
	{
		for (LightSource* light : scene->getLights())
		{
			PointLight *li = static_cast<PointLight*>(light);

			float lightDistance = 0;
			li->illuminate(ray.getIntersection1(), I, intens, intensity);

			N = shape.getNormal(ray.getIntersection1());
			I = -I;
			//R = I - (2.0f*Vector::dotProduct(N, I)*N);
			R = 2.0f*(Vector::dotProduct(N, I)*N) - I;

			/*Ray toLight = Ray(ray.getIntersection1(), I);
			for (int i = 0; i < scene->getAddIndex(); i++) {
				if (i != ray.primIndex)
					scene->getPrimitive(i)->intersect(&toLight);
				else
					lightIntTest = 0;
			}
			if (toLight.intersects)
			{
				float dist1 = Point::makeVector(toLight.getIntersection1(), li->getPosition()).length();
				float dist2 = Point::makeVector(ray.getIntersection1(), li->getPosition()).length();
				if (dist1 > dist2);
					return fin;
				//lightIntTest = 0;
			}*/

			diff += d * std::max(0.0f, Vector::dotProduct(N, I)) * intens;
			//spec += std::pow(std::max(0.0f, Vector::dotProduct(R, -ray.getDirection().normalizeProduct())), ns) * intens;
			spec += std::pow(std::max(Vector::dotProduct(R, -dir), 0.0f), ns) * intens;
		}

		fin = diff * kds + spec * kss;
	}
	else if (shape.isMirror && rayRefection < maxAllowedRayReflections)
	{
		//Ray mirror = ray;
		I = ray.getDirection();
		N = shape.getNormal(ray.getIntersection1());
		R = I - (2.0f*Vector::dotProduct(N, I)*N);

		Ray reflected(ray.getIntersection1(), R);

		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (reflected.intersects) {
			if (reflected.primIndex == 2)
				float sa = 1;
			fin += PhongTriangle(reflected, *(Triangle*)scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}
	else if (shape.isRefracting && rayRefection < maxAllowedRayReflections)
	{
		float refFactor = 1.52f;
		I = ray.getDirection();
		N = shape.getNormal(ray.getIntersection1());
		//R = (1 * (I - N * Vector::dotProduct(I, N))) / refFactor - N * std::sqrt(1 - ((1 * 1 * (1 - (Vector::dotProduct(I, N) * Vector::dotProduct(I, N)))) / (refFactor * refFactor)));
		float fi = 1.0f / refFactor;
		float ni = Vector::dotProduct(I, N);
		//R = fi * I + N * std::sqrt(1 - fi * fi*(1 - ni * ni)) - fi * N*ni;
		R = std::sqrt(1 - fi * fi*(1 - ni * ni)) *N + fi * (I - ni * N);

		Ray reflected(ray.getIntersection1(), R);

		for (int i = 0; i < scene->getAddIndex(); i++) {
			reflected.potentialIndex = i;
			if (i != ray.primIndex)
				scene->getPrimitive(i)->intersect(&reflected);
		}
		if (reflected.intersects) {
			fin += PhongTriangle(reflected, *(Triangle*)scene->getPrimitive(reflected.primIndex), ++rayRefection);
		}
		else
			fin += LightIntensity();
	}

	return fin;
}