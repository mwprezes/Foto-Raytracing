#pragma once
#include <ostream>

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

	LightIntensity operator+(LightIntensity& li) {
		return LightIntensity(r + li.r, g + li.g, b + li.b);
	}
	LightIntensity operator-(LightIntensity& li) {
		return LightIntensity(r - li.r, g - li.g, b - li.b);
	}
	LightIntensity operator*(LightIntensity& li) {
		return LightIntensity(r * li.r, g * li.g, b * li.b);
	}
	LightIntensity operator/(LightIntensity& li) {
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
		return *this;
	}
	LightIntensity& operator-=(const LightIntensity& li) {
		this->r -= li.r;
		this->g -= li.g;
		this->b -= li.b;
		return *this;
	}
	LightIntensity& operator*=(const LightIntensity& li) {
		this->r *= li.r;
		this->g *= li.g;
		this->b *= li.b;
		return *this;
	}
	LightIntensity& operator/=(const LightIntensity& li) {
		this->r /= li.r;
		this->g /= li.g;
		this->b /= li.b;
		return *this;
	}

	LightIntensity& operator+=(const double val) {
		this->r += val;
		this->g += val;
		this->b += val;
		return *this;
	}
	LightIntensity& operator-=(const double val) {
		this->r -= val;
		this->g -= val;
		this->b -= val;
		return *this;
	}
	LightIntensity& operator*=(const double val) {
		this->r *= val;
		this->g *= val;
		this->b *= val;
		return *this;
	}
	LightIntensity& operator/=(const double val) {
		this->r /= val;
		this->g /= val;
		this->b /= val;
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

#pragma endregion


};

