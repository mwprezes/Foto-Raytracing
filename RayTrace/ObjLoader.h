#pragma once

#include "Mesh.h"
#include "Point.h"
#include "Primitive.h"
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

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

	void splitStr(std::string &str, std::vector<std::string> &strOut, char splitCharacter) //split string by a character ignoring beggining
	{
		strOut.clear();
		std::string temp = getRest(str);
		std::stringstream test(temp);

		while (std::getline(test, temp, splitCharacter)) {
			strOut.push_back(temp);
		}
	}
	void splitStrWithStart(std::string &str, std::vector<std::string> &strOut, char splitCharacter) //split string by a character with beggining
	{
		strOut.clear();
		std::string temp = str;
		std::stringstream test(temp);

		while (std::getline(test, temp, splitCharacter)) {
			strOut.push_back(temp);
		}
	}

	float AngleBetweenV(Vector a, Vector b)
	{
		float angle = Vector::dotProduct(a, b);
		angle /= (a.length() * b.length());
		return angle = acosf(angle);
	}

	void GenerateVerts(std::vector<Vertex> &verts, std::vector<Vector> positions, std::vector<Vector> tCoords, std::vector<Vector> normals, std::string currline)
	{
		bool noNormal = false;
		std::vector<std::string> sFace, sVert;
		Vertex vert;

		splitStr(currline, sFace, ' ');

		for (std::string face : sFace) 
		{
			int vertType = -1;
			splitStrWithStart(face, sVert, '/');
			
			if (sVert.size() == 1) // v1
			{
				// Position
				vertType = 1;
			}			
			else if (sVert.size() == 2) // v1/vt1
			{
				// Position & Texture
				vertType = 2;
			}			
			else if (sVert.size() == 3) // v1/vt1/vn1 or v1//vn1
			{
				if (sVert[1] != "")
				{
					// Position, Texture, Normal
					vertType = 4;
				}
				else
				{
					// Position & Normal
					vertType = 3;
				}
			}
			bool broken = false;

			switch (vertType)
			{
			case 1:
				vert.position = positions[std::stoi(sVert[0]) - 1];
				vert.texture = Vector(0, 0, 0);
				noNormal = true;
				verts.push_back(vert);
				break;
			case 2:
				vert.position = positions[std::stoi(sVert[0]) - 1];
				vert.texture = tCoords[std::stoi(sVert[1]) - 1];
				noNormal = true;
				verts.push_back(vert);
				break;
			case 3:
				vert.position = positions[std::stoi(sVert[0]) - 1];
				vert.texture = Vector(0, 0, 0);
				vert.normal = normals[std::stoi(sVert[2]) - 1];
				verts.push_back(vert);
				break;
			case 4:
				vert.position = positions[std::stoi(sVert[0]) - 1];
				vert.texture = tCoords[std::stoi(sVert[1]) - 1];
				vert.normal = normals[std::stoi(sVert[2]) - 1];
				verts.push_back(vert);
				break;

			case -1:
				std::cout << "bad vert" << std::endl;
				broken = true;
				break;
			default:
				break;
			}
			if (broken) {
				std::exception ex = std::exception("Opssie woopsi");
				return;
			}
		}

		if (noNormal)
		{
			Vector A = verts[0].position - verts[1].position;
			Vector B = verts[2].position - verts[1].position;

			Vector normal = Vector::crossProduct(A, B);

			for (int i = 0; i < verts.size(); i++)
			{
				verts[i].normal = normal;
			}
		}
	}

	void Triangulate(std::vector<unsigned int> &indices, std::vector<Vertex> &verts, std::vector<Triangle> &outTriangles) 
	{
		if (verts.size() < 3) // No triangle
		{
			return;
		}

		if (verts.size() == 3) // Is triangle
		{
			Triangle tempTri;

			indices.push_back(0);
			tempTri.v1 = verts[0].position;
			tempTri.vn1 = verts[0].normal;
			tempTri.vt1 = verts[0].texture;
			indices.push_back(1);
			tempTri.v2 = verts[1].position;
			tempTri.vn2 = verts[1].normal;
			tempTri.vt2 = verts[1].texture;
			indices.push_back(2);
			tempTri.v3 = verts[2].position;
			tempTri.vn3 = verts[2].normal;
			tempTri.vt3 = verts[2].texture;
			tempTri.makePlane();

			outTriangles.push_back(tempTri);
			return;
		}

		std::vector<Vertex> tempVerts = verts;

		do {

			for (int i = 0; i < tempVerts.size(); i++)
			{
				Vertex prev;
				Vertex curr;
				Vertex next;

				if (i == 0)
					prev = tempVerts[tempVerts.size() - 1];
				else
					prev = tempVerts[i - 1];

				curr = tempVerts[i];

				if (i == tempVerts.size() - 1)
					next = tempVerts[0];
				else
					next = tempVerts[i + 1];

				if (tempVerts.size() == 3) //last 3 verts
				{
					Triangle tempTri;

					for (int j = 0; j < tempVerts.size(); j++)
					{
						if (verts[j].position == prev.position) {
							indices.push_back(j);
							tempTri.v1 = verts[j].position;
							tempTri.vn1 = verts[j].normal;
							tempTri.vt1 = verts[j].texture;
						}
						else if (verts[j].position == curr.position) {
							indices.push_back(j);
							tempTri.v2 = verts[j].position;
							tempTri.vn2 = verts[j].normal;
							tempTri.vt2= verts[j].texture;
						}
						else if (verts[j].position == next.position) {
							indices.push_back(j);
							tempTri.v3 = verts[j].position;
							tempTri.vn3 = verts[j].normal;
							tempTri.vt3 = verts[j].texture;
						}
					}
					tempTri.makePlane();
					outTriangles.push_back(tempTri);
					tempVerts.clear();
					break;
				}
				if (tempVerts.size() == 4)
				{
					Point tempP;
					Triangle tempTri;

					for (int j = 0; j < verts.size(); j++)
					{
						if (verts[j].position == prev.position) {
							indices.push_back(j);
							tempTri.v1 = verts[j].position;
							tempTri.vn1 = verts[j].normal;
							tempTri.vt1 = verts[j].texture;
						}
						else if (verts[j].position == curr.position) {
							indices.push_back(j);
							tempTri.v2 = verts[j].position;
							tempTri.vn2 = verts[j].normal;
							tempTri.vt2 = verts[j].texture;
						}
						else if (verts[j].position == next.position) {
							indices.push_back(j);
							tempTri.v3 = verts[j].position;
							tempTri.vn3 = verts[j].normal;
							tempTri.vt3 = verts[j].texture;
						}
					}
					tempTri.makePlane();
					outTriangles.push_back(tempTri);

					for (int j = 0; j < tempVerts.size(); j++)
					{
						if (tempVerts[j].position != prev.position && tempVerts[j].position != curr.position && tempVerts[j].position != next.position)
						{
							tempP = tempVerts[j].position;
							break;
						}
					}
					for (int j = 0; j < verts.size(); j++)
					{
						if (verts[j].position == prev.position) {
							indices.push_back(j);
							tempTri.v1 = verts[j].position;
							tempTri.vn1 = verts[j].normal;
							tempTri.vt1 = verts[j].texture;
						}
						else if (verts[j].position == tempP) {
							indices.push_back(j);
							tempTri.v2 = verts[j].position;
							tempTri.vn2 = verts[j].normal;
							tempTri.vt2 = verts[j].texture;
						}
						else if (verts[j].position == next.position) {
							indices.push_back(j);
							tempTri.v3 = verts[j].position;
							tempTri.vn3 = verts[j].normal;
							tempTri.vt3 = verts[j].texture;
						}
					}
					tempTri.makePlane();
					outTriangles.push_back(tempTri);
					tempVerts.clear();
					break;
				}

				float angle = AngleBetweenV(prev.position - curr.position, next.position - curr.position) * (180 / 3.14159265359);
				if (angle <= 0 && angle >= 180)
					continue;

				/*bool inTri = false;
				for (int j = 0; j < int(iVerts.size()); j++)
				{
					if (algorithm::inTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position)
						&& iVerts[j].Position != pPrev.Position
						&& iVerts[j].Position != pCur.Position
						&& iVerts[j].Position != pNext.Position)
					{
						inTri = true;
						break;
					}
				}
				if (inTri)
					continue;*/
				Triangle tempTri;
				for (int j = 0; j < verts.size(); j++)
				{
					if (verts[j].position == prev.position) {
						indices.push_back(j);
						tempTri.v1 = verts[j].position;
						tempTri.vn1 = verts[j].normal;
						tempTri.vt1 = verts[j].texture;
					}
					else if (verts[j].position == curr.position) {
						indices.push_back(j);
						tempTri.v2 = verts[j].position;
						tempTri.vn2 = verts[j].normal;
						tempTri.vt2 = verts[j].texture;
					}
					else if (verts[j].position == next.position) {
						indices.push_back(j);
						tempTri.v3 = verts[j].position;
						tempTri.vn3 = verts[j].normal;
						tempTri.vt3 = verts[j].texture;
					}
				}
				tempTri.makePlane();
				outTriangles.push_back(tempTri);
				for (int j = 0; j < tempVerts.size(); j++)
				{
					if (tempVerts[j].position == curr.position) 
					{
						tempVerts.erase(tempVerts.begin() + j);
						break;
					}
				}
				i = -1;
			}

			if (indices.size() == 0)
				break;

			if (tempVerts.size() == 0)
				break;

		} while (true);
	}

public:
	std::vector<Primitive> sceneMesh;
	std::vector<Triangle> triangles;
	Material mat;

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

		std::vector<Vector> positions;
		std::vector<Vector> tCoords;
		std::vector<Vector> normals;

		std::string currline;
		while (std::getline(file, currline)) 
		{
			std::string meshname;

			//if (lineBeggin(currline) == "hello")
				//std::cout << "hello" << std::endl;
			if (lineBeggin(currline) == "g")
			{
				meshname = getRest(currline);
				std::cout << "Meshname: " << meshname << std::endl;
			}
			else if (lineBeggin(currline) == "v") //Position
			{
				std::vector<std::string> sPos;
				Vector pos;

				splitStr(currline, sPos, ' '); // '\t' je¿eli tabulatrory; ' ' je¿eli spacje

				pos = Vector(std::stof(sPos[0]), std::stof(sPos[1]), std::stof(sPos[2]));
				positions.push_back(pos);
				//std::cout << "Pos: " << pos << std::endl;
			}
			else if (lineBeggin(currline) == "vn") //Normals
			{
				std::vector<std::string> sNPos;
				Vector nPos;

				splitStr(currline, sNPos, ' ');

				nPos = Vector(std::stof(sNPos[0]), std::stof(sNPos[1]), std::stof(sNPos[2]));
				normals.push_back(nPos);
			}
			else if (lineBeggin(currline) == "vt") //TextureCoord
			{
				std::vector<std::string> sTPos;
				Vector tPos;

				splitStr(currline, sTPos, ' ');

				tPos = Vector(std::stof(sTPos[0]), std::stof(sTPos[1]), 0);
				tCoords.push_back(tPos);
			}
			else if (lineBeggin(currline) == "f") //Faces
			{
				std::vector<Vertex> tempVerts;
				GenerateVerts(tempVerts, positions, tCoords, normals, currline);

				for (Vertex &ver : tempVerts)
					verts.push_back(ver);

				std::vector<unsigned int> indices;
				std::vector<Triangle> tempTriangles;
				Triangulate(indices, tempVerts, tempTriangles);

				for (unsigned int &i : indices)
				{
					unsigned int indexNum = (unsigned int)(verts.size() - tempVerts.size()) + i;
					inds.push_back(indexNum);
				}
				for (Triangle &tri : tempTriangles)
					triangles.push_back(tri);
			}
			else if (lineBeggin(currline) == "mtllib")
				LoadMaterial(getRest(currline));
		}

		/*for(Vertex &vert : verts)
		{
			std::cout << "Pos: " << vert.position << std::endl;
		}*/
		/*for(unsigned int &i : inds)
		{
			std::cout << "Pos: " << i << std::endl;
		}*/

		file.close();

		return true;
	}

	bool LoadMaterial(std::string filename)
	{
		if (filename.substr(filename.size() - 4, 4) != ".mtl")
			return false;

		std::ifstream file(filename);

		if (!file.is_open())
			return false;

		Material tempMat;
		std::string currline;
		while (std::getline(file, currline))
		{
			//Params
			if (lineBeggin(currline) == "newmtl")
			{
				tempMat.name = getRest(currline);
			}
			else if (lineBeggin(currline) == "Ns")
				tempMat.Ns = std::stof(getRest(currline));
			else if (lineBeggin(currline) == "d")
				tempMat.d = std::stof(getRest(currline));
			else if (lineBeggin(currline) == "Tr")
				tempMat.Tr = std::stof(getRest(currline));
			else if (lineBeggin(currline) == "illum")
				tempMat.illum = std::stof(getRest(currline));
			else if (lineBeggin(currline) == "Ka")
			{
				std::vector<std::string> sKa;
				Vector ka;

				splitStr(currline, sKa, ' '); // '\t' je¿eli tabulatrory; ' ' je¿eli spacje

				ka = Vector(std::stof(sKa[0]), std::stof(sKa[1]), std::stof(sKa[2]));
				tempMat.Ka = ka;
			}
			else if (lineBeggin(currline) == "Kd")
			{
				std::vector<std::string> sKa;
				Vector ka;

				splitStr(currline, sKa, ' '); // '\t' je¿eli tabulatrory; ' ' je¿eli spacje

				ka = Vector(std::stof(sKa[0]), std::stof(sKa[1]), std::stof(sKa[2]));
				tempMat.Kd = ka;
			}
			else if (lineBeggin(currline) == "Ks")
			{
				std::vector<std::string> sKa;
				Vector ka;

				splitStr(currline, sKa, ' '); // '\t' je¿eli tabulatrory; ' ' je¿eli spacje

				ka = Vector(std::stof(sKa[0]), std::stof(sKa[1]), std::stof(sKa[2]));
				tempMat.Ks = ka;
			}
			//Maps
			if (lineBeggin(currline) == "map_Ka")
				tempMat.map_Ka = getRest(currline);
			else if (lineBeggin(currline) == "map_Kd")
				tempMat.map_Kd = getRest(currline);
			else if (lineBeggin(currline) == "map_Ks")
				tempMat.map_Ks = getRest(currline);
			else if (lineBeggin(currline) == "map_d")
				tempMat.map_d = getRest(currline);
			else if (lineBeggin(currline) == "map_bump" || lineBeggin(currline) == "bump")
				tempMat.map_bump = getRest(currline);
		}
		mat = tempMat;
		file.close();

		if (&mat == NULL)
			return false;
		else
			return true;
	}

};