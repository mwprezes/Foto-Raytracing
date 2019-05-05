#pragma once
#include "Triangle.h"
#include <vector>

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

class Mesh
{
private:
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<Triangle> triangles;
	Material mat;
public:
	Mesh();
	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind);
	~Mesh();

	#pragma region Getters/Setters

	std::string getName() {
		return name;
	}
	std::vector<Vertex> getVertices() {
		return vertices;
	}
	std::vector<unsigned int> getIndices(){
		return indices;
	}
	std::vector<Triangle> getTriangles() {
		return triangles;
	}
	Material getMat() {
		return mat;
	}

	Vertex getVert(int at) {
		return vertices[at];
	}
	unsigned int getIndex(int at) {
		return indices[at];
	}
	Triangle getTriangle(int at) {
		return triangles[at];
	}

	void setName(std::string name) {
		this->name = name;
	}
	void setTriangles(std::vector<Triangle> tri) {
		triangles = tri;
	}
	void addVert(Vertex vert) {
		vertices.push_back(vert);
	}
	void addIndex(unsigned int i) {
		indices.push_back(i);
	}

	#pragma endregion

};

