#pragma once

#include <vector>
#include "Point.h"
#include "Primitive.h"
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

struct Vertex 
{
	Point position;
	Vector normal;
	Vector texture;
};

struct Material
{
	std::string name;
	Vector Ka;
	Vector Kd;
	Vector Ks;
	float Ns;
	float illum;
	float Tr;
};

struct Mesh
{
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material mat;

	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind) {
		vertices = vert;
		indices = ind;
	}
};

class ObjLoader //https://github.com/Bly7/OBJ-Loader/blob/master/Source/OBJ_Loader.h
{
private:
	std::string filename;
	std::vector<Mesh> meshes;
	std::vector<Vertex> verts;
	std::vector<unsigned int> inds;

	std::string lineBeggin(const std::string &str) 
	{
		if (!str.empty()) 
		{
			size_t start = str.find_first_not_of(" \t");
			size_t end = str.find_first_of(" \t", start);
			if (start != std::string::npos && end != std::string::npos)
				return str.substr(start, end - start);
			else if (start != std::string::npos)
				return str.substr(start);
		}
		return "";
	}

	std::string getRest(const std::string &str) //of the line
	{
		if (!str.empty()) 
		{
			size_t start_b = str.find_first_not_of(" \t");
			size_t end_b = str.find_first_of(" \t", start_b);
			size_t start = str.find_first_not_of(" \t", end_b);
			size_t end = str.find_last_not_of(" \t");
			if (start != std::string::npos && end != std::string::npos)
				return str.substr(start, end - start + 1);
			else if (start != std::string::npos)
				return str.substr(start);
		}
		return "";
	}

	void splitStr(std::string &str, std::vector<std::string> &strOut, char splitCharacter) //split string by a character
	{
		strOut.clear();
		std::string temp = getRest(str);
		std::stringstream test(temp);

		while (std::getline(test, temp, splitCharacter)) {
			strOut.push_back(temp);
		}
	}

public:
	std::vector<Primitive> sceneMesh;

	ObjLoader() {

	}
	~ObjLoader() {

	}

	bool LoadOBJ(std::string filename) 
	{
		this->filename = filename;
		if (filename.substr(filename.size() - 4, 4) != ".obj")
			return false;

		std::ifstream file(filename);

		if (!file.is_open())
			return false;

		meshes.clear();
		verts.clear();
		inds.clear();

		std::string currline;
		while (std::getline(file, currline)) 
		{
			std::string meshname;

			//if (lineBeggin(currline) == "hello")
				//std::cout << "hello" << std::endl;
			if (lineBeggin(currline) == "g") {
				meshname = getRest(currline);
				std::cout << "Meshname: " << meshname << std::endl;
			}
			else if (lineBeggin(currline) == "v") {

				std::vector<std::string> sPos;
				Vector pos;

				splitStr(currline, sPos, '\t'); // '\t' je¿eli tabulatrory; ' ' je¿eli spacje

				pos = Vector(std::stof(sPos[0]), std::stof(sPos[1]), std::stof(sPos[2]));

				std::cout << "Pos: " << pos << std::endl;
			}

		}

		return true;
	}

};