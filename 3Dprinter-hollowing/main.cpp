#include <iostream>
#include <fstream>
#include <string>
#include <amp.h>
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

	for (int i = 0; i < rg.rhombuses.size(); i++)
	{
		Rhombus* r = &rg.rhombuses[i];
		float boxCenter0 = r->center.x;
		float boxCenter1 = r->center.y;
		float boxCenter2 = r->center.z;
		float boxH0 = r->width / 2.0f;
		float boxH1 = r->height / 2.0f;
		float boxH2 = 0;
		int counter[1] = { 0 };
		concurrency::array_view<Face, 1> f(rabbit->faces.size(), rabbit->faces);
		concurrency::array_view<glm::vec3, 1> v(rabbit->vertices.size(), rabbit->vertices);
		concurrency::array_view<int, 1> c(1,counter);
		concurrency::parallel_for_each(f.extent, [=](concurrency::index<1> j) restrict(amp)
			{
				float triverts[3][3] = {
					{v[f[j].x - 1].x, v[f[j].x - 1].y, v[f[j].x - 1].z},
					{v[f[j].y - 1].x, v[f[j].y - 1].y, v[f[j].y - 1].z},
					{v[f[j].z - 1].x, v[f[j].z - 1].y, v[f[j].z - 1].z}
				};
				float boxCenter[3] = { boxCenter0,boxCenter1, boxCenter2 };
				float boxH[3] = { boxH0, boxH1, boxH2 };
				int x = triBoxOverlap(boxCenter, boxH, triverts);
				if (x == 1) {
					c[0]++;
				}
			}
		);
		if (c.data()[0] == 0) {
			r->isBound = true;
			rg.removeRhombus(*r);
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
	for (unsigned int i = 0; i < mesh2->faces.size(); i++)
	{
		mesh1->addIndex(mesh2->faces[i]+mesh1->vertices.size());
	}
	
	for (unsigned int i = 0; i < mesh2->vertices.size(); i++)
	{
		mesh1->addVertex(mesh2->vertices[i]);
	}
}
