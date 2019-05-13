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

void Primitive::setMat(std::string name, Vector Ka, Vector Kd, Vector Ks, float Ns, float illum, float Tr, float d)
{
	material = PrimitiveMaterial(name, Ka, Kd, Ks, Ns, illum, Tr, d);
}
