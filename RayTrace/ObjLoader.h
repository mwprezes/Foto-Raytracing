#pragma once

#include <vector>
#include "Point.h"
#include "Primitive.h"
#include <cmath>
#include <fstream>
#include <string>

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

public:
	std::vector<Primitive> Mesh;

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

		}
	}

};