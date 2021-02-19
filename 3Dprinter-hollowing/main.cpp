#include <iostream>
#include <fstream>
#include <string>
#include "glm/glm.hpp"
#include "MeshParser.h"
#include "Rhombus.h"
#include "RhombusGrid.h"

void mergeMeshes(Mesh* mesh1, Mesh* mesh2);
Rhombus* splitRhombus(Rhombus* rhombus, std::vector<Rhombus> rg);

int main()
{
	//infile.open("objs/bunny.obj");
	Mesh* rabbit = MeshParser::parse("objs/bunny.obj");
	std::cout << rabbit->center.x << " " << rabbit->center.y<<" " << rabbit->center.z << std::endl;
	
	Rhombus* root = new Rhombus(0.5f,0.2f, rabbit->center);
	RhombusGrid rg(*root);
	//Rhombus* splited = splitRhombus(root, rhombusGrid);
	rg.subDivide();
	rg.subDivide();
	rg.subDivide();
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
