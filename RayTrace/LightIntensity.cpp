#include "LightIntensity.h"



LightIntensity::LightIntensity()
{
	r = g = b = 1.0;
}

LightIntensity::LightIntensity(double R, double G, double B)
{
	/*r = R;
	g = G;
	b = B;*/
	setR(R);
	setG(G);
	setB(B);
}

LightIntensity::LightIntensity(double RGB)
{
	/*r = g = b = RGB;*/
	setR(RGB);
	setG(RGB);
	setB(RGB);
}


LightIntensity::~LightIntensity()
{
}
