#include <iostream>
#include <fstream>
#include <string>
#include "glm/glm.hpp"
#include "MeshParser.h"
#include "Rhombus.h"
#include "RhombusGrid.h"
#include "BoxTriOverlap.h"

void mergeMeshes(Mesh* mesh1, Mesh* mesh2);
Rhombus* splitRhombus(Rhombus* rhombus, std::vector<Rhombus> rg);

int main()
{
	Mesh* rabbit = MeshParser::parse("objs/bunny.obj");
	std::cout << rabbit->center.x << " " << rabbit->center.y<<" " << rabbit->center.z << std::endl;
	Rhombus* root = new Rhombus(0.5f,0.2f, rabbit->center);

	RhombusGrid rg(*root);
	rg.subDivide();
	rg.subDivide();
	rg.subDivide();
	rg.subDivide();
	for (int i = 0; i < rg.rhombuses.size(); i++)
	{
		Rhombus* r = &rg.rhombuses[i];
		float boxCenter[3] = { r->center.x, r->center.y, r->center.z };
		float boxH[3] = { r->width / 2.0f, r->height / 2.0f, 0 };
		int counter = 0;
		for (int j = 0; j < rabbit->indices.size(); j+=3)
		{
			float triverts[3][3] = { 
				{rabbit->vertices[rabbit->indices[j]-1].x,rabbit->vertices[rabbit->indices[j]-1].y,rabbit->vertices[rabbit->indices[j]-1].z},

				{rabbit->vertices[rabbit->indices[j + 1]-1].x,rabbit->vertices[rabbit->indices[j + 1]-1].y,rabbit->vertices[rabbit->indices[j + 1]-1].z},

				{rabbit->vertices[rabbit->indices[j + 2]-1].x,rabbit->vertices[rabbit->indices[j + 2]-1].y,rabbit->vertices[rabbit->indices[j + 2]-1].z}
			};

			int x = triBoxOverlap(boxCenter, boxH, triverts);
			if (x == 1) {
				counter++;
				//r->center += glm::vec3(0,0,1);
				break;
			}
			else if(j==rabbit->indices.size()-3){
				rg.removeRhombus(*r);
			}
		}
		
	}
	for (size_t i = 0; i < rg.rhombuses.size(); i++)
	{
		mergeMeshes(rabbit, &rg.rhombuses[i]);
	}
	

	MeshParser::exportMesh(*rabbit,"objs/output/test.obj");
	return 0;
}

Rhombus* splitRhombus(Rhombus* rhombus, std::vector<Rhombus> rg) {
	glm::vec3 c = rhombus->center;
	Rhombus* r1 = new Rhombus(rhombus->height / 2.0f, rhombus->width/2.0f, c+glm::vec3(0, rhombus->height / 4.0f, 0));
	Rhombus* r2 = new Rhombus(rhombus->height / 2.0f, rhombus->width/2.0f, c+glm::vec3(rhombus->width / 4.0f,0, 0));
	Rhombus* r3 = new Rhombus(rhombus->height / 2.0f, rhombus->width/2.0f, c-glm::vec3(0, rhombus->height / 4.0f, 0));
	Rhombus* r4 = new Rhombus(rhombus->height / 2.0f, rhombus->width/2.0f, c-glm::vec3(rhombus->width / 4.0f,0, 0));
	mergeMeshes(r1, r2);
	mergeMeshes(r1, r3);
	mergeMeshes(r1, r4);
	rg.push_back(*r1);
	rg.push_back(*r2);
	rg.push_back(*r3);
	rg.push_back(*r4);
	return r1;
}

void mergeMeshes(Mesh* mesh1, Mesh* mesh2) {
	for (unsigned int i = 0; i < mesh2->indices.size(); i++)
	{
		mesh1->addIndex(mesh2->indices[i]+mesh1->vertices.size());
	}
	
	for (unsigned int i = 0; i < mesh2->vertices.size(); i++)
	{
		mesh1->addVertex(mesh2->vertices[i]);
	}
}