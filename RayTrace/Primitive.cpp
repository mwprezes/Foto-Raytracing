#include "Primitive.h"



Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

int Primitive::intersect(Ray *ray)
{
	return 0;
}

void Primitive::setColor(double R, double G, double B)
{
	color = LightIntensity(R, G, B);
}
