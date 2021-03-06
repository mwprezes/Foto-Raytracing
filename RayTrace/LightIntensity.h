#pragma once
#include <ostream>
#include "Vector.h"

class LightIntensity
{
protected:
	double r, g, b;
public:
	LightIntensity();
	LightIntensity(double R, double G, double B);
	LightIntensity(double RGB);
	~LightIntensity();

#pragma region Getters/Setters

	double getR() {
		return r;
	}
	double getG() {
		return g;
	}
	double getB() {
		return b;
	}
	void setR(double R) {
		if (R < 0.0)
			r = 0.0;
		else if (R > 1.0)
			r = 1.0;
		else
			r = R;
	}
	void setG(double G) {
		if (G < 0.0)
			g = 0.0;
		else if (G > 1.0)
			g = 1.0;
		else
			g = G;
	}
	void setB(double B) {
		if (B < 0.0)
			b = 0.0;
		else if (B > 1.0)
			b = 1.0;
		else
			b = B;
	}
#pragma endregion

#pragma region Operators

	friend std::ostream& operator<<(std::ostream &out, const LightIntensity& li)
	{
		out << "(" << li.r << ", " << li.g << ", " << li.b << ") ";
		return out;
	}

	LightIntensity operator+(const LightIntensity& li) {
		LightIntensity light = LightIntensity(r + li.r, g + li.g, b + li.b);
		return light;
	}
	LightIntensity operator-(const LightIntensity& li) {
		return LightIntensity(r - li.r, g - li.g, b - li.b);
	}
	LightIntensity operator*(const LightIntensity& li) {
		return LightIntensity(r * li.r, g * li.g, b * li.b);
	}
	LightIntensity operator/(const LightIntensity& li) {
		return LightIntensity(r / li.r, g / li.g, b / li.b);
	}

	LightIntensity operator+(double val) {
		return LightIntensity(r + val, g + val, b + val);
	}
	LightIntensity operator-(double val) {
		return LightIntensity(r - val, g - val, b - val);
	}
	LightIntensity operator*(double val) {
		return LightIntensity(r * val, g * val, b * val);
	}
	LightIntensity operator/(double val) {
		return LightIntensity(r / val, g / val, b / val);
	}
	bool operator==(LightIntensity& li) {
		if (this->r == li.r && this->g == li.g && this->b == li.b)
			return true;
		else
			return false;
	}
	bool operator!=(LightIntensity& li) {
		if (this->r != li.r || this->g != li.g || this->b != li.b)
			return true;
		else
			return false;
	}
	LightIntensity& operator+=(const LightIntensity& li) {
		this->r += li.r;
		this->g += li.g;
		this->b += li.b;
		Clamp();
		return *this;
	}
	LightIntensity& operator-=(const LightIntensity& li) {
		this->r -= li.r;
		this->g -= li.g;
		this->b -= li.b;
		Clamp();
		return *this;
	}
	LightIntensity& operator*=(const LightIntensity& li) {
		this->r *= li.r;
		this->g *= li.g;
		this->b *= li.b;
		Clamp();
		return *this;
	}
	LightIntensity& operator/=(const LightIntensity& li) {
		this->r /= li.r;
		this->g /= li.g;
		this->b /= li.b;
		Clamp();
		return *this;
	}

	LightIntensity& operator+=(const double val) {
		this->r += val;
		this->g += val;
		this->b += val;
		Clamp();
		return *this;
	}
	LightIntensity& operator-=(const double val) {
		this->r -= val;
		this->g -= val;
		this->b -= val;
		Clamp();
		return *this;
	}
	LightIntensity& operator*=(const double val) {
		this->r *= val;
		this->g *= val;
		this->b *= val;
		Clamp();
		return *this;
	}
	LightIntensity& operator/=(const double val) {
		this->r /= val;
		this->g /= val;
		this->b /= val;
		Clamp();
		return *this;
	}
	
	LightIntensity& operator+=(Vector& val) {
		this->r += val.getX();
		this->g += val.getY();
		this->b += val.getZ();
		Clamp();
		return *this;
	}
	LightIntensity& operator-=(Vector& val) {
		this->r -= val.getX();
		this->g -= val.getY();
		this->b -= val.getZ();
		Clamp();
		return *this;
	}
	LightIntensity& operator*=(Vector& val) {
		this->r *= val.getX();
		this->g *= val.getY();
		this->b *= val.getZ();
		Clamp();
		return *this;
	}
	LightIntensity& operator/=(Vector& val) {
		this->r /= val.getX();
		this->g /= val.getY();
		this->b /= val.getZ();
		Clamp();
		return *this;
	}

	friend LightIntensity operator+(double val, const LightIntensity& li) {
		return LightIntensity(val + li.r, val + li.g, val + li.b);
	}
	friend LightIntensity operator-(double val, const LightIntensity& li) {
		return LightIntensity(val - li.r, val - li.g, val - li.b);
	}
	friend LightIntensity operator*(double val, const LightIntensity& li) {
		return LightIntensity(val * li.r, val * li.g, val * li.b);
	}
	friend LightIntensity operator/(double val, const LightIntensity& li) {
		return LightIntensity(val / li.r, val / li.g, val / li.b);
	}

	LightIntensity operator+(Vector& li) {
		return LightIntensity(r + li.getX(), g + li.getY(), b + li.getZ());
	}
	LightIntensity operator-(Vector& li) {
		return LightIntensity(r - li.getX(), g - li.getY(), b - li.getZ());
	}
	LightIntensity operator*(Vector& li) {
		return LightIntensity(r * li.getX(), g * li.getY(), b * li.getZ());
	}
	LightIntensity operator/(Vector& li) {
		return LightIntensity(r / li.getX(), g / li.getY(), b / li.getZ());
	}

	friend LightIntensity operator+(Vector val, const LightIntensity& li) {
		return LightIntensity(val.getX() + li.r, val.getY() + li.g, val.getZ() + li.b);
	}
	friend LightIntensity operator-(Vector val, const LightIntensity& li) {
		return LightIntensity(val.getX() - li.r, val.getY() - li.g, val.getZ() - li.b);
	}
	friend LightIntensity operator*(Vector val, const LightIntensity& li) {
		return LightIntensity(val.getX() * li.r, val.getY() * li.g, val.getZ() * li.b);
	}
	friend LightIntensity operator/(Vector val, const LightIntensity& li) {
		return LightIntensity(val.getX() / li.r, val.getY() / li.g, val.getZ() / li.b);
	}

#pragma endregion

	void Clamp();
};

