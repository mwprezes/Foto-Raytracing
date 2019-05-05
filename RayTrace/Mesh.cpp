#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind)
{
	vertices = vert;
	indices = ind;
}


Mesh::~Mesh()
{
}
