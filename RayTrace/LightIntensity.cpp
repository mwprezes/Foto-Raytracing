#include "LightIntensity.h"



LightIntensity::LightIntensity()
{
	r = g = b = 0.0;
}

LightIntensity::LightIntensity(double R, double G, double B)
{
	r = R;
	g = G;
	b = B;
}

LightIntensity::LightIntensity(double RGB)
{
	r = g = b = RGB;
}


LightIntensity::~LightIntensity()
{
}
