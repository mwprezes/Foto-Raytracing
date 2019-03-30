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

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			//Rogi pixela
			float tlX = (-1.0f + i * pixelWidth) * planeWidth;
			float tlY = (1.0f - j * pixelHeight) * planeHeight;

			float trX = (-1.0f + (i + 1) * pixelWidth) * planeWidth;
			float trY = (1.0f - j * pixelHeight) * planeHeight;

			float blX = (-1.0f + i * pixelWidth) * planeWidth;
			float blY = (1.0f - (j + 1) * pixelHeight) * planeHeight;

			float brX = (-1.0f + (i + 1) * pixelWidth) * planeWidth;
			float brY = (1.0f - (j + 1) * pixelHeight) * planeHeight;

			Ray ray(Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU()), projectionPlane.getN());
			Ray rayTL(Point(ray.getOrigin() - pixelWidth / 2 * projectionPlane.getV() + pixelHeight / 2 * projectionPlane.getU()), projectionPlane.getN());
			Ray rayTR(Point(ray.getOrigin() + pixelWidth / 2 * projectionPlane.getV() + pixelHeight / 2 * projectionPlane.getU()), projectionPlane.getN());
			Ray rayBL(Point(ray.getOrigin() - pixelWidth / 2 * projectionPlane.getV() - pixelHeight / 2 * projectionPlane.getU()), projectionPlane.getN());
			Ray rayBR(Point(ray.getOrigin() + pixelWidth / 2 * projectionPlane.getV() - pixelHeight / 2 * projectionPlane.getU()), projectionPlane.getN());

			sphere = Sphere(0, 0, 0, 50);
			sphere.setColor(1, 0, 0);
			sphere2 = Sphere(20, 0, 50, 60);
			sphere2.setColor(0, 0, 1);

			sphere.intersect(ray);
			sphere.intersect(rayTL);
			sphere.intersect(rayTR);
			sphere.intersect(rayBL);
			sphere.intersect(rayBR);

			sphere2.intersect(ray);
			sphere2.intersect(rayTL);
			sphere2.intersect(rayTR);
			sphere2.intersect(rayBL);
			sphere2.intersect(rayBR);

			//LightIntensity color1 = rayTL.getColor();
			//LightIntensity color2 = rayTR.getColor();
			//LightIntensity color3 = rayBL.getColor();
			//LightIntensity color4 = rayBR.getColor();

			////Antyaliasing
			//if (ray.getColor().getR() != color1.getR() || ray.getColor().getG() != color1.getG() || ray.getColor().getB() != color1.getB())
			//{
			//	Ray raySamp(Point(rayTL.getOrigin() + pixelWidth / 4 * projectionPlane.getV() - pixelHeight / 4 * projectionPlane.getU()), projectionPlane.getN());
			//	color1 = sampling();
			//}
			//if (ray.getColor().getR() != color2.getR() || ray.getColor().getG() != color2.getG() || ray.getColor().getB() != color2.getB())
			//{
			//	Ray raySamp(Point(rayTR.getOrigin() - pixelWidth / 4 * projectionPlane.getV() - pixelHeight / 4 * projectionPlane.getU()), projectionPlane.getN());
			//	color2 = sampling();
			//}
			//if (ray.getColor().getR() != color3.getR() || ray.getColor().getG() != color3.getG() || ray.getColor().getB() != color3.getB())
			//{
			//	Ray raySamp(Point(rayBL.getOrigin() + pixelWidth / 4 * projectionPlane.getV() + pixelHeight / 4 * projectionPlane.getU()), projectionPlane.getN());
			//	color3 = sampling();
			//}
			//if (ray.getColor().getR() != color4.getR() || ray.getColor().getG() != color4.getG() || ray.getColor().getB() != color4.getB())
			//{
			//	Ray raySamp(Point(rayBR.getOrigin() - pixelWidth / 4 * projectionPlane.getV() - pixelHeight / 4 * projectionPlane.getU()), projectionPlane.getN());
			//	color4 = sampling();
			//}

			//LightIntensity pixelColor1 = color1 / 4.0;
			//LightIntensity pixelColor2 = color2 / 4.0;
			//LightIntensity pixelColor3 = color3 / 4.0;
			//LightIntensity pixelColor4 = color4 / 4.0;

			//LightIntensity pixelColor = pixelColor1 + pixelColor2 + pixelColor3 + pixelColor4;

			stop = 0;
			LightIntensity pixelColor = samplingOrtho(ray, rayTL, rayTR, rayBL, rayBR, pixelHeight, pixelWidth, 8);

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

	for (int i = 0; i < wig; i++) {
		for (int j = 0; j < hig; j++) {

			float medX = (-1.0f + (i + 0.5f) * pixelWidth) * planeWidth;
			float medY = (1.0f - (j + 0.5f) * pixelHeight) * planeHeight;

			Point pixelCenter = Point(projectionPlane.getBase() + medX * projectionPlane.getV() + medY * projectionPlane.getU());
			Point pixelTL = Point(pixelCenter - pixelWidth / 2 * projectionPlane.getV() + pixelHeight / 2 * projectionPlane.getU());
			Point pixelTR = Point(pixelCenter + pixelWidth / 2 * projectionPlane.getV() + pixelHeight / 2 * projectionPlane.getU());
			Point pixelBL = Point(pixelCenter - pixelWidth / 2 * projectionPlane.getV() - pixelHeight / 2 * projectionPlane.getU());
			Point pixelBR = Point(pixelCenter + pixelWidth / 2 * projectionPlane.getV() - pixelHeight / 2 * projectionPlane.getU());

			Vector toPixelDirection = Point::makeVector(pos, pixelCenter);
			Vector toPixelDirectionTL = Point::makeVector(pos, pixelTL);
			Vector toPixelDirectionTR = Point::makeVector(pos, pixelTR);
			Vector toPixelDirectionBL = Point::makeVector(pos, pixelBL);
			Vector toPixelDirectionBR = Point::makeVector(pos, pixelBR);

			toPixelDirection.normalize();
			toPixelDirectionTL.normalize();
			toPixelDirectionTR.normalize();
			toPixelDirectionBL.normalize();
			toPixelDirectionBR.normalize();

			Ray ray(pos, toPixelDirection);
			Ray rayTL(pos, toPixelDirectionTL);
			Ray rayTR(pos, toPixelDirectionTR);
			Ray rayBL(pos, toPixelDirectionBL);
			Ray rayBR(pos, toPixelDirectionBR);

			sphere = Sphere(0, 0, 0, 30);
			sphere.setColor(1, 0, 0);
			sphere2 = Sphere(20, 0, 50, 40);
			sphere2.setColor(0, 0, 1);

			sphere.intersect(ray);
			sphere.intersect(rayTL);
			sphere.intersect(rayTR);
			sphere.intersect(rayBL);
			sphere.intersect(rayBR);

			sphere2.intersect(ray);
			sphere2.intersect(rayTL);
			sphere2.intersect(rayTR);
			sphere2.intersect(rayBL);
			sphere2.intersect(rayBR);

			stop = 0;
			LightIntensity pixelColor = samplingPersp(pixelCenter, pixelHeight, pixelWidth, 8);

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
	LightIntensity pixelColor;

	if (rayTL.getDirection() == rayMed.getDirection()) {}
	else
		rayTL = Ray(Point(rayMed.getOrigin() - width / 2 * projectionPlane.getV() + height / 2 * projectionPlane.getU()), projectionPlane.getN());
	if (rayTR.getDirection() == rayMed.getDirection()) {}
	else
		rayTR = Ray(Point(rayMed.getOrigin() + width / 2 * projectionPlane.getV() + height / 2 * projectionPlane.getU()), projectionPlane.getN());
	if (rayBL.getDirection() == rayMed.getDirection()) {}
	else
		rayBL = Ray(Point(rayMed.getOrigin() - width / 2 * projectionPlane.getV() - height / 2 * projectionPlane.getU()), projectionPlane.getN());
	if (rayBR.getDirection() == rayMed.getDirection()) {}
	else
		rayBR = Ray(Point(rayMed.getOrigin() + width / 2 * projectionPlane.getV() - height / 2 * projectionPlane.getU()), projectionPlane.getN());

	stop += 1;

	sphere.intersect(rayMed);
	sphere.intersect(rayTL);
	sphere.intersect(rayTR);
	sphere.intersect(rayBL);
	sphere.intersect(rayBR);

	sphere2.intersect(rayMed);
	sphere2.intersect(rayTL);
	sphere2.intersect(rayTR);
	sphere2.intersect(rayBL);
	sphere2.intersect(rayBR);

	LightIntensity color1 = rayTL.getColor();
	LightIntensity color2 = rayTR.getColor();
	LightIntensity color3 = rayBL.getColor();
	LightIntensity color4 = rayBR.getColor();

	//Antyaliasing
	if ((rayMed.getColor().getR() != color1.getR() || rayMed.getColor().getG() != color1.getG() || rayMed.getColor().getB() != color1.getB()) && stop < maxStop)
	{
		Ray raySamp(Point(rayTL.getOrigin() + width / 4 * projectionPlane.getV() - height / 4 * projectionPlane.getU()), projectionPlane.getN());
		color1 = samplingOrtho(raySamp, rayTL, Ray(Point(0, 0, 0), -raySamp.getDirection()), Ray(Point(0, 0, 0), -raySamp.getDirection()), rayMed, height / 2, width / 2, maxStop);
	}
	if ((rayMed.getColor().getR() != color2.getR() || rayMed.getColor().getG() != color2.getG() || rayMed.getColor().getB() != color2.getB()) && stop < maxStop)
	{
		Ray raySamp(Point(rayTR.getOrigin() - width / 4 * projectionPlane.getV() - height / 4 * projectionPlane.getU()), projectionPlane.getN());
		color2 = samplingOrtho(raySamp, Ray(Point(0, 0, 0), -raySamp.getDirection()), rayTR, rayMed, Ray(Point(0, 0, 0), -raySamp.getDirection()), height / 2, width / 2, maxStop);
	}
	if ((rayMed.getColor().getR() != color3.getR() || rayMed.getColor().getG() != color3.getG() || rayMed.getColor().getB() != color3.getB()) && stop < maxStop)
	{
		Ray raySamp(Point(rayBL.getOrigin() + width / 4 * projectionPlane.getV() + height / 4 * projectionPlane.getU()), projectionPlane.getN());
		color3 = samplingOrtho(raySamp, Ray(Point(0, 0, 0), -raySamp.getDirection()), raySamp, rayBL, Ray(Point(0, 0, 0), -raySamp.getDirection()), height / 2, width / 2, maxStop);
	}
	if ((rayMed.getColor().getR() != color4.getR() || rayMed.getColor().getG() != color4.getG() || rayMed.getColor().getB() != color4.getB()) && stop < maxStop)
	{
		Ray raySamp(Point(rayBR.getOrigin() - width / 4 * projectionPlane.getV() - height / 4 * projectionPlane.getU()), projectionPlane.getN());
		color4 = samplingOrtho(raySamp, rayMed, Ray(Point(0, 0, 0), -raySamp.getDirection()), Ray(Point(0, 0, 0), -raySamp.getDirection()), rayBR, height / 2, width / 2, maxStop);
	}

	LightIntensity pixelColor1 = color1 / 4.0;
	LightIntensity pixelColor2 = color2 / 4.0;
	LightIntensity pixelColor3 = color3 / 4.0;
	LightIntensity pixelColor4 = color4 / 4.0;

	pixelColor = pixelColor1 + pixelColor2 + pixelColor3 + pixelColor4;
	return pixelColor;
}

LightIntensity Camera::samplingPersp(Point center, float height, float width, int maxStop)
{
	LightIntensity pixelColor;

	Point pixelTL = Point(center - width / 2 * projectionPlane.getV() + height / 2 * projectionPlane.getU());
	Point pixelTR = Point(center + width / 2 * projectionPlane.getV() + height / 2 * projectionPlane.getU());
	Point pixelBL = Point(center - width / 2 * projectionPlane.getV() - height / 2 * projectionPlane.getU());
	Point pixelBR = Point(center + width / 2 * projectionPlane.getV() - height / 2 * projectionPlane.getU());

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
	sphere.intersect(rayTL);
	sphere.intersect(rayTR);
	sphere.intersect(rayBL);
	sphere.intersect(rayBR);

	sphere2.intersect(rayMed);
	sphere2.intersect(rayTL);
	sphere2.intersect(rayTR);
	sphere2.intersect(rayBL);
	sphere2.intersect(rayBR);

	LightIntensity color1 = rayTL.getColor();
	LightIntensity color2 = rayTR.getColor();
	LightIntensity color3 = rayBL.getColor();
	LightIntensity color4 = rayBR.getColor();

	stop += 1;

	//Antyaliasing
	if ((rayMed.getColor().getR() != color1.getR() || rayMed.getColor().getG() != color1.getG() || rayMed.getColor().getB() != color1.getB()) && stop < maxStop)
	{
		Point centerSamp = Point(pixelTL + width / 4 * projectionPlane.getV() - height / 4 * projectionPlane.getU());
		color1 = samplingPersp(centerSamp, height / 2, width / 2, maxStop);
	}
	if ((rayMed.getColor().getR() != color2.getR() || rayMed.getColor().getG() != color2.getG() || rayMed.getColor().getB() != color2.getB()) && stop < maxStop)
	{
		Point centerSamp = Point(pixelTR - width / 4 * projectionPlane.getV() - height / 4 * projectionPlane.getU());
		color2 = samplingPersp(centerSamp, height / 2, width / 2, maxStop);

	}
	if ((rayMed.getColor().getR() != color3.getR() || rayMed.getColor().getG() != color3.getG() || rayMed.getColor().getB() != color3.getB()) && stop < maxStop)
	{
		Point centerSamp = Point(pixelBL + width / 4 * projectionPlane.getV() + height / 4 * projectionPlane.getU());
		color3 = samplingPersp(centerSamp, height / 2, width / 2, maxStop);

	}
	if ((rayMed.getColor().getR() != color4.getR() || rayMed.getColor().getG() != color4.getG() || rayMed.getColor().getB() != color4.getB()) && stop < maxStop)
	{
		Point centerSamp = Point(pixelBR - width / 4 * projectionPlane.getV() + height / 4 * projectionPlane.getU());
		color4 = samplingPersp(centerSamp, height / 2, width / 2, maxStop);

	}

	LightIntensity pixelColor1 = color1 / 4.0;
	LightIntensity pixelColor2 = color2 / 4.0;
	LightIntensity pixelColor3 = color3 / 4.0;
	LightIntensity pixelColor4 = color4 / 4.0;

	pixelColor = pixelColor1 + pixelColor2 + pixelColor3 + pixelColor4;
	return pixelColor;
}
