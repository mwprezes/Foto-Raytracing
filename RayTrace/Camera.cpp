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

void Camera::renderOrtho(bitmap_image img, int height, int width)
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

	projectionPlane = Plane(pos, up, v1);

	sphere = Sphere(0, 0, 0, 60);
	sphere.setColor(0, 0, 1);
	sphere2 = Sphere(-70, 0, 40, 40);
	sphere2.setColor(1, 0, 0);

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Ray ray(Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU()), projectionPlane.getN());
			sphere.intersect(ray);
			sphere2.intersect(ray);

			LightIntensity pixelColor;
			
			if (antiAliastingOn)
			{
				Ray rayTL(Point(ray.getOrigin() - pixelWidth * 0.5f * projectionPlane.getV() + pixelHeight * 0.5f * projectionPlane.getU()), projectionPlane.getN());
				Ray rayTR(Point(ray.getOrigin() + pixelWidth * 0.5f * projectionPlane.getV() + pixelHeight * 0.5f * projectionPlane.getU()), projectionPlane.getN());
				Ray rayBL(Point(ray.getOrigin() - pixelWidth * 0.5f * projectionPlane.getV() - pixelHeight * 0.5f * projectionPlane.getU()), projectionPlane.getN());
				Ray rayBR(Point(ray.getOrigin() + pixelWidth * 0.5f * projectionPlane.getV() - pixelHeight * 0.5f * projectionPlane.getU()), projectionPlane.getN());

				sphere.intersect(rayTL);
				sphere2.intersect(rayTL);

				sphere.intersect(rayTR);
				sphere2.intersect(rayTR);

				sphere.intersect(rayBL);
				sphere2.intersect(rayBL);

				sphere.intersect(rayBR);	
				sphere2.intersect(rayBR);

				stop = 0;
				pixelColor = samplingOrtho(ray, rayTL, rayTR, rayBL, rayBR, pixelHeight, pixelWidth, 10);
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

	sphere = Sphere(0, 0, 0, 60);
	sphere.setColor(0, 0, 1);
	sphere2 = Sphere(-70, 0, 40, 40);
	sphere2.setColor(1, 0, 0);

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Point pixelCenter = Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU());
			Vector toPixelDirection = Point::makeVector(pos, pixelCenter);
			toPixelDirection.normalize();
			Ray ray(pos, toPixelDirection);
			sphere.intersect(ray);
			sphere2.intersect(ray);

			LightIntensity pixelColor;

			if (antiAliastingOn)
			{
				Point pixelTL = Point(pixelCenter - pixelWidth * 0.5f * projectionPlane.getV() + pixelHeight * 0.5f * projectionPlane.getU());
				Point pixelTR = Point(pixelCenter + pixelWidth * 0.5f * projectionPlane.getV() + pixelHeight * 0.5f * projectionPlane.getU());
				Point pixelBL = Point(pixelCenter - pixelWidth * 0.5f * projectionPlane.getV() - pixelHeight * 0.5f * projectionPlane.getU());
				Point pixelBR = Point(pixelCenter + pixelWidth * 0.5f * projectionPlane.getV() - pixelHeight * 0.5f * projectionPlane.getU());

				Vector toPixelDirectionTL = Point::makeVector(pos, pixelTL);
				Vector toPixelDirectionTR = Point::makeVector(pos, pixelTR);
				Vector toPixelDirectionBL = Point::makeVector(pos, pixelBL);
				Vector toPixelDirectionBR = Point::makeVector(pos, pixelBR);

				toPixelDirectionTL.normalize();
				toPixelDirectionTR.normalize();
				toPixelDirectionBL.normalize();
				toPixelDirectionBR.normalize();

				Ray rayTL(pos, toPixelDirectionTL);
				Ray rayTR(pos, toPixelDirectionTR);
				Ray rayBL(pos, toPixelDirectionBL);
				Ray rayBR(pos, toPixelDirectionBR);

				sphere.intersect(rayTL);
				sphere2.intersect(rayTL);

				sphere.intersect(rayTR);
				sphere2.intersect(rayTR);

				sphere.intersect(rayBL);
				sphere2.intersect(rayBL);

				sphere.intersect(rayBR);											
				sphere2.intersect(rayBR);

				stop = 0;
				pixelColor = samplingPersp(pixelCenter, pixelHeight, pixelWidth, 10);
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

	std::cout << "Render Persp completed" << std::endl;
	img.save_image("renderP.jpg");
}

LightIntensity Camera::samplingOrtho(Ray rayMed, Ray rayTL, Ray rayTR, Ray rayBL, Ray rayBR, float height, float width, int maxStop)
{
	width = width * 0.5;
	height = height * 0.5;

	LightIntensity pixelColor;	

	if (rayTL.getDirection() == rayMed.getDirection()) {}
	else
		rayTL = Ray(Point(rayMed.getOrigin() - width * projectionPlane.getV() + height * projectionPlane.getU()), projectionPlane.getN());
	if (rayTR.getDirection() == rayMed.getDirection()) {}
	else
		rayTR = Ray(Point(rayMed.getOrigin() + width * projectionPlane.getV() + height * projectionPlane.getU()), projectionPlane.getN());
	if (rayBL.getDirection() == rayMed.getDirection()) {}
	else
		rayBL = Ray(Point(rayMed.getOrigin() - width * projectionPlane.getV() - height * projectionPlane.getU()), projectionPlane.getN());
	if (rayBR.getDirection() == rayMed.getDirection()) {}
	else
		rayBR = Ray(Point(rayMed.getOrigin() + width * projectionPlane.getV() - height * projectionPlane.getU()), projectionPlane.getN());

	stop += 1;

	sphere.intersect(rayMed);
	sphere2.intersect(rayMed);
	LightIntensity color0 = rayMed.getColor();

	sphere.intersect(rayTL);
	sphere2.intersect(rayTL);
	LightIntensity color1 = rayTL.getColor();

	sphere.intersect(rayTR);
	sphere2.intersect(rayTR);
	LightIntensity color2 = rayTR.getColor();

	sphere.intersect(rayBL);
	sphere2.intersect(rayBL);
	LightIntensity color3 = rayBL.getColor();

	sphere.intersect(rayBR);
	sphere2.intersect(rayBR);	
	LightIntensity color4 = rayBR.getColor();

	//Antyaliasing
	if (stop < maxStop)
	{		
		if (rayMed.getColor().getR() != color1.getR() || rayMed.getColor().getG() != color1.getG() || rayMed.getColor().getB() != color1.getB())
		{
			Ray raySamp(Point(rayTL.getOrigin() + width * 0.5 * projectionPlane.getV() - height * 0.5 * projectionPlane.getU()), projectionPlane.getN());
			color1 = samplingOrtho(raySamp, rayTL, Ray(Point(0, 0, 0), -raySamp.getDirection()), Ray(Point(0, 0, 0), -raySamp.getDirection()), rayMed, height, width, maxStop);
		}
		if (rayMed.getColor().getR() != color2.getR() || rayMed.getColor().getG() != color2.getG() || rayMed.getColor().getB() != color2.getB())
		{
			Ray raySamp(Point(rayTR.getOrigin() - width * 0.5 * projectionPlane.getV() - height * 0.5 * projectionPlane.getU()), projectionPlane.getN());
			color2 = samplingOrtho(raySamp, Ray(Point(0, 0, 0), -raySamp.getDirection()), rayTR, rayMed, Ray(Point(0, 0, 0), -raySamp.getDirection()), height, width, maxStop);
		}
		if (rayMed.getColor().getR() != color3.getR() || rayMed.getColor().getG() != color3.getG() || rayMed.getColor().getB() != color3.getB())
		{
			Ray raySamp(Point(rayBL.getOrigin() + width * 0.5 * projectionPlane.getV() + height  * 0.5 * projectionPlane.getU()), projectionPlane.getN());
			color3 = samplingOrtho(raySamp, Ray(Point(0, 0, 0), -raySamp.getDirection()), rayMed, rayBL, Ray(Point(0, 0, 0), -raySamp.getDirection()), height, width, maxStop);
		}
		if (rayMed.getColor().getR() != color4.getR() || rayMed.getColor().getG() != color4.getG() || rayMed.getColor().getB() != color4.getB())
		{
			Ray raySamp(Point(rayBR.getOrigin() - width * 0.5 * projectionPlane.getV() + height  * 0.5 * projectionPlane.getU()), projectionPlane.getN());
			color4 = samplingOrtho(raySamp, rayMed, Ray(Point(0, 0, 0), -raySamp.getDirection()), Ray(Point(0, 0, 0), -raySamp.getDirection()), rayBR, height, width, maxStop);
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

LightIntensity Camera::samplingPersp(Point center, float height, float width, int maxStop)
{
	width = width * 0.5;
	height = height * 0.5;

	LightIntensity pixelColor;

	Point pixelTL = Point(center - width * projectionPlane.getV() + height * projectionPlane.getU());
	Point pixelTR = Point(center + width * projectionPlane.getV() + height * projectionPlane.getU());
	Point pixelBL = Point(center - width * projectionPlane.getV() - height * projectionPlane.getU());
	Point pixelBR = Point(center + width * projectionPlane.getV() - height * projectionPlane.getU());

	Vector toPixelDirection = Point::makeVector(pos, center);
	Vector toPixelDirectionTL = Point::makeVector(pos, pixelTL);
	Vector toPixelDirectionTR = Point::makeVector(pos, pixelTR);
	Vector toPixelDirectionBL = Point::makeVector(pos, pixelBL);
	Vector toPixelDirectionBR = Point::makeVector(pos, pixelBR);

	Ray rayMed(pos, toPixelDirection);
	Ray rayTL(pos, toPixelDirectionTL);
	Ray rayTR(pos, toPixelDirectionTR);
	Ray rayBL(pos, toPixelDirectionBL);
	Ray rayBR(pos, toPixelDirectionBR);

	sphere.intersect(rayMed);
	sphere2.intersect(rayMed);
	LightIntensity color0 = rayMed.getColor();
	
	sphere.intersect(rayTL);
	sphere2.intersect(rayTL);
	LightIntensity color1 = rayTL.getColor();
	
	sphere.intersect(rayTR);
	sphere2.intersect(rayTR);
	LightIntensity color2 = rayTR.getColor();
	
	sphere.intersect(rayBL);
	sphere2.intersect(rayBL);
	LightIntensity color3 = rayBL.getColor();

	sphere.intersect(rayBR);
	sphere2.intersect(rayBR);
	LightIntensity color4 = rayBR.getColor();

	stop += 1;

	//Antyaliasing
	if (stop < maxStop)
	{	
		if (rayMed.getColor().getR() != color1.getR() || rayMed.getColor().getG() != color1.getG() || rayMed.getColor().getB() != color1.getB())
		{
			Point centerSamp = Point(pixelTL + width * 0.5 * projectionPlane.getV() - height * 0.5 * projectionPlane.getU());
			color1 = samplingPersp(centerSamp, height, width, maxStop);
		}
		if (rayMed.getColor().getR() != color2.getR() || rayMed.getColor().getG() != color2.getG() || rayMed.getColor().getB() != color2.getB())
		{
			Point centerSamp = Point(pixelTR - width * 0.5 * projectionPlane.getV() - height * 0.5 * projectionPlane.getU());
			color2 = samplingPersp(centerSamp, height, width, maxStop);

		}
		if (rayMed.getColor().getR() != color3.getR() || rayMed.getColor().getG() != color3.getG() || rayMed.getColor().getB() != color3.getB())
		{
			Point centerSamp = Point(pixelBL + width * 0.5 * projectionPlane.getV() + height * 0.5 * projectionPlane.getU());
			color3 = samplingPersp(centerSamp, height, width, maxStop);

		}
		if (rayMed.getColor().getR() != color4.getR() || rayMed.getColor().getG() != color4.getG() || rayMed.getColor().getB() != color4.getB())
		{
			Point centerSamp = Point(pixelBR - width * 0.5 * projectionPlane.getV() + height * 0.5 * projectionPlane.getU());
			color4 = samplingPersp(centerSamp, height, width, maxStop);

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
