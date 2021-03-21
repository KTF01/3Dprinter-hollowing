
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <string>
#include <amp.h>
#include <math.h>
#include <limits>
#include "glm/glm.hpp"
#include "MeshParser.h"
#include "Rhombus.h"
#include "RhombusGrid.h"
#include "BoxTriOverlap.h"
#include "RayTriIntersect.h"

void mergeMeshes(Mesh* mesh1, Mesh* mesh2);
Rhombus* splitRhombus(Rhombus* rhombus, std::vector<Rhombus> rg);
bool isRhombusIntersect(Rhombus *r, Mesh* rabbit);
std::vector<Rhombus> splitToFourIfIntersect(Rhombus* r, Mesh* rabbit, RhombusGrid& rg);
//std::vector<Rhombus> splitToFourIfIntersect(Mesh* rabbit, RhombusGrid& rg);
bool isRayIntersectTriangle(Rhombus* r, Mesh* rabbit);

bool isSsmaller(Rhombus* a, Rhombus* b) {
	return a->center.z < b->center.z;
}

float boundMinZ = 10000.0f;

double alpha = M_PI / 5.5;

int main()
{
	Mesh* rabbit = MeshParser::parse("objs/bunny.obj");
	std::cout << rabbit->center.x << " " << rabbit->center.y<<" " << rabbit->center.z << std::endl;
	
	double t = 0.001;
	double hperw = tan((M_PI / 2.0) - alpha);
	double w = 4 * rabbit->aabbX;
	if ((4 * rabbit->aabbY) / hperw > w ) {
		w = (4*rabbit->aabbY) / hperw;
	}
	double h = w * hperw;

	Rhombus* root = new Rhombus(h,w, rabbit->center/*+glm::vec<3, double>(0,-0.05,0)*/, t);

	RhombusGrid rg(*root);

	//rg.rhombuses = rg.splitRhombus(root);

	rg.rhombuses = splitToFourIfIntersect(&rg.rhombuses[0], rabbit, rg);


	for (unsigned int i = 0; i < rg.rhombuses.size(); i++) {
		if (!rg.rhombuses[i].isBound && rg.rhombuses[i].center.z < 0) {
			rg.removeRhombus(rg.rhombuses[i], -2);
			if (isRayIntersectTriangle(&rg.rhombuses[i], rabbit)) {
				rg.removeRhombus(rg.rhombuses[i], 1);
			}
		}
		else if (!rg.rhombuses[i].isBound &&rg.rhombuses[i].center.z > 0) {
			//rg.removeRhombus(rg.rhombuses[i]);
			if (isRayIntersectTriangle(&rg.rhombuses[i], rabbit)) {
				rg.removeRhombus(rg.rhombuses[i], -1);
			}
		}
	}


	for (size_t i = 0; i < rg.rhombuses.size(); i++)
	{
		/*if (!rg.rhombuses[i].isBound) {
			rg.removeRhombus(rg.rhombuses[i]);
		}*/
		mergeMeshes(rabbit, &rg.rhombuses[i]);
	}


	Mesh* aabbR = new Mesh();
	/*aabbR->addVertex({rabbit->center.x+rabbit->aabbX,rabbit->center.y+rabbit->aabbY,rabbit->center.z+rabbit->aabbZ});
	aabbR->addVertex({rabbit->center.x+rabbit->aabbX,rabbit->center.y+rabbit->aabbY,rabbit->center.z-rabbit->aabbZ});*/
	/*aabbR->addVertex({rabbit->center.x+rabbit->aabbX,rabbit->center.y-rabbit->aabbY,rabbit->center.z+rabbit->aabbZ});
	aabbR->addVertex({rabbit->center.x-rabbit->aabbX,rabbit->center.y-rabbit->aabbY,rabbit->center.z-rabbit->aabbZ});
	aabbR->addVertex({rabbit->center.x-rabbit->aabbX,rabbit->center.y+rabbit->aabbY,rabbit->center.z-rabbit->aabbZ});*/


	/*aabbR->addVertex({ rabbit->center.x + rabbit->aabbX,rabbit->center.y + rabbit->aabbY,boundMinZ + 9.8 });
	aabbR->addVertex({ rabbit->center.x - rabbit->aabbX,rabbit->center.y - rabbit->aabbY,boundMinZ + 9.8 });
	aabbR->addVertex({ rabbit->center.x + rabbit->aabbX,rabbit->center.y - rabbit->aabbY,boundMinZ + 9.8 });*/
	//aabbR->addIndex({1,2,3});
	//aabbR->addIndex({1,4,3});
	//aabbR->addIndex({2,6,5});
	//aabbR->addIndex({5,2,3});

	mergeMeshes(rabbit, aabbR);
	MeshParser::exportMesh(*rabbit,"objs/output/test.obj");
	return 0;
}

std::vector<Rhombus> splitToFourIfIntersect(Rhombus* r, Mesh *rabbit, RhombusGrid &rg) {
	std::vector<Rhombus> finalGrid;
	float rabbitHeight = 2*abs(rabbit->aabbY - rabbit->center.y);
	if (r->height < rabbitHeight/4.0f) {
		if (!isRhombusIntersect(r, rabbit)) {
			rg.removeRhombus(*r);
			finalGrid.push_back(*r);
		}
		else {
			r->isBound = true;
		}
		if ((r->center.z-r->depth/2.0) < boundMinZ) {
			boundMinZ = (r->center.z - r->depth / 2.0);
			std::cout << boundMinZ << std::endl;
		}
		//finalGrid.push_back(*r); 
		return finalGrid;
	}
	if (isRhombusIntersect(r, rabbit)) {
		std::vector<Rhombus> childs = rg.splitRhombus(r);
		for (unsigned int i = 0; i < childs.size(); i++)
		{
			std::vector<Rhombus> tmpGrid = splitToFourIfIntersect(&childs[i], rabbit, rg);
			finalGrid.insert(finalGrid.end(), tmpGrid.begin(), tmpGrid.end());
		}
	}
	else {
		if ((r->center.z - r->depth / 2.0) < boundMinZ) {
			boundMinZ = (r->center.z - r->depth / 2.0);
			std::cout << boundMinZ << std::endl;
		}
		rg.removeRhombus(*r);
		finalGrid.push_back(*r);
	}
	return finalGrid;
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

bool isRhombusIntersect(Rhombus* r, Mesh* rabbit) {
	double boxCenter0 = r->center.x;
	double boxCenter1 = r->center.y;
	double boxCenter2 = r->center.z;
	double boxH0 = r->width / 2.0f;
	double boxH1 = r->height / 2.0f;
	double boxH2 = r->depth/2.0;
	int counter[1] = { 0 };
	concurrency::array_view<Face, 1> f(rabbit->faces.size(), rabbit->faces);
	concurrency::array_view<glm::vec<3, double>, 1> v(rabbit->vertices.size(), rabbit->vertices);
	concurrency::array_view<int, 1> c(1, counter);
	concurrency::parallel_for_each(f.extent, [=](concurrency::index<1> j) restrict(amp)
		{
			double triverts[3][3] = {
				{v[f[j].x - 1].x, v[f[j].x - 1].y, v[f[j].x - 1].z},
				{v[f[j].y - 1].x, v[f[j].y - 1].y, v[f[j].y - 1].z},
				{v[f[j].z - 1].x, v[f[j].z - 1].y, v[f[j].z - 1].z}
			};
			double boxCenter[3] = { boxCenter0,boxCenter1, boxCenter2 };
			double boxH[3] = { boxH0, boxH1, boxH2 };
			int x = triBoxOverlap(boxCenter, boxH, triverts);
			if (x == 1) {
				c[0]++;
			}
		}
	);
	if (c.data()[0] > 0) {
		return true;
	}
	else return false;
}

bool isRayIntersectTriangle(Rhombus* r, Mesh* rabbit) {
	double boxCenter0 = r->center.x;
	double boxCenter1 = r->center.y;
	double boxCenter2 = r->center.z;


	std::vector<int> counterVec;
	counterVec.resize(rabbit->faces.size());
	concurrency::array_view<Face, 1> f(rabbit->faces.size(), rabbit->faces);
	concurrency::array_view<glm::vec<3, double>, 1> v(rabbit->vertices.size(), rabbit->vertices);
	concurrency::array_view<int, 1> c(counterVec.size(), counterVec);
	concurrency::parallel_for_each(f.extent, [=](concurrency::index<1> j) restrict(amp) {

			double triverts[3][3] = {
				{v[f[j].x - 1].x, v[f[j].x - 1].y, v[f[j].x - 1].z},
				{v[f[j].y - 1].x, v[f[j].y - 1].y, v[f[j].y - 1].z},
				{v[f[j].z - 1].x, v[f[j].z - 1].y, v[f[j].z - 1].z}
			};
			double boxCenter[3] = { boxCenter0,boxCenter1, boxCenter2 };
			double rayDir[] = { 1,0, 0};
			double t, v, u;
			int x = intersect_triangle3(boxCenter, rayDir, triverts[0], triverts[1], triverts[2], &t, &u, &v );
			if (t<0.0) {
				c[j]=x;
			}
		}
	);
	int count = 0;
	for (unsigned int i = 0; i < counterVec.size(); i++)
	{
		count += c.data()[i];
	}
	//std::cout << "Utkozesek szama: " << count << std::endl;
	
	if (count % 2==1) {
		return true;
	}
	else return false;
}