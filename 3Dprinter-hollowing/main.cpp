
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
bool isRhombusIntersect(Rhombus *r, Mesh* rabbit);
std::vector<Rhombus> splitToFourIfIntersect(Rhombus* r, Mesh* rabbit, RhombusGrid& rg);
//std::vector<Rhombus> splitToFourIfIntersect(Mesh* rabbit, RhombusGrid& rg);
bool isRhombusInside(Rhombus* r, Mesh* rabbit);
bool isRhombusOutsideBound(Rhombus* r, Mesh* rabbit);
void removeRhombusesOutside(std::vector<Rhombus> &rhombuses, Mesh* mesh, RhombusGrid* rg);

bool isSsmaller(Rhombus* a, Rhombus* b) {
	return a->center.z < b->center.z;
}

float boundMinZ = 10000.0f;

double alpha = M_PI / 5.5;

int rombusCounter = 0;
int main()
{
	Mesh* rabbit = MeshParser::parse("objs/happy.obj");
	std::cout << rabbit->center.x << " " << rabbit->center.y<<" " << rabbit->center.z << std::endl;
	
	double t = 0.0001;
	double hperw = tan((M_PI / 2.0) - alpha);
	double w = 4 * rabbit->aabbX;
	if ((4 * rabbit->aabbY) / hperw > w ) {
		w = (4*rabbit->aabbY) / hperw;
	}
	double h = w * hperw;

	//Rhombus* root = new Rhombus(h,w, rabbit->center+glm::vec<3, double>(0.2,0.1,-0.2), t);
	Rhombus* root = new Rhombus(h,w, rabbit->center, t);

	RhombusGrid rg(*root);

	//rg.rhombuses = rg.splitRhombus(root);
	//std::cout << isRhombusIntersect(&rg.rhombuses[0], rabbit) << std::endl;


	rg.rhombuses = splitToFourIfIntersect(&rg.rhombuses[0], rabbit, rg);
	std::cout << rg.rhombuses.size() << std::endl;

	removeRhombusesOutside(rg.rhombuses, rabbit, &rg);

	for (size_t i = 0; i < rg.rhombuses.size(); i++)
	{
		mergeMeshes(rabbit, &rg.rhombuses[i]);
	}
	//MeshParser::exportMesh(*rabbit,"objs/output/test.obj");
	MeshParser::exportMeshSTL(rabbit,"objs/output/happy.stl");
	return 0;
}

bool isRhombusOutsideBound(Rhombus* r, Mesh* rabbit) {
	if (r->center.x<rabbit->center.x - rabbit->aabbX || r->center.x > rabbit->center.x + rabbit->aabbX) return true;
	if (r->center.y<rabbit->center.y - rabbit->aabbY || r->center.y > rabbit->center.y + rabbit->aabbY) return true;
	if (r->center.z<rabbit->center.z - rabbit->aabbZ || r->center.z > rabbit->center.z + rabbit->aabbZ) return true;
	return false;
}


std::vector<Rhombus> splitToFourIfIntersect(Rhombus* r, Mesh *rabbit, RhombusGrid &rg) {
	std::vector<Rhombus> finalGrid;
	float rabbitHeight = 2*abs(rabbit->aabbY - rabbit->center.y);
	if (r->height < rabbitHeight/16.0f) {
		if (!isRhombusIntersect(r, rabbit)) {
			if (!isRhombusOutsideBound(r, rabbit)) {
				/*if (isRhombusInside(r, rabbit)) {
					r->removeOutside();
					finalGrid.push_back(*r);
					rombusCounter++;
				}*/
				finalGrid.push_back(*r);
			}
			//rg.removeRhombus(*r);
		}
		else {
			r->isBound = true;
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
		if (!isRhombusOutsideBound(r, rabbit)) {
			/*if (isRhombusInside(r, rabbit)) {
				r->removeOutside();
				finalGrid.push_back(*r);
				rombusCounter++;
			}*/
			finalGrid.push_back(*r);
		}
		//rg.removeRhombus(*r);
		//finalGrid.push_back(*r);
	}
	return finalGrid;
}

void mergeMeshes(Mesh* mesh1, Mesh* mesh2) {
	for (unsigned int i = 0; i < mesh2->faces.size(); i++)
	{
		mesh1->addFace(mesh2->faces[i]+mesh1->vertices.size());
		mesh1->addNormal(mesh2->normals[i]);
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
			//int x = NoDivTriTriIsect(triverts[0], triverts[1], triverts[2], rhombusTris[0][0], rhombusTris[0][1], rhombusTris[0][2]);
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

void removeRhombusesOutside(std::vector<Rhombus> &rhombuses, Mesh* mesh, RhombusGrid* rg) {
	int fsize = mesh->faces.size();
	std::vector<glm::vec<3, double>> rhombusCenters;
	for (size_t i = 0; i < rhombuses.size(); i++)
	{
		rhombusCenters.push_back(rhombuses[i].center);
	}
	concurrency::array_view<glm::vec<3, double>, 1> r(rhombuses.size(), rhombusCenters);
	concurrency::array_view<Face, 1> f(fsize, mesh->faces);
	concurrency::array_view<glm::vec<3, double>, 1> v(mesh->vertices.size(), mesh->vertices);
	concurrency::array_view<int, 1> c(rhombuses.size());
	//concurrency::array_view<Face,Rhombus, 2> rf(mesh->faces.size(), mesh->faces);
	concurrency::parallel_for_each(r.extent, [=](concurrency::index<1> i) restrict(amp) {
		int countX = 0;
		int countY = 0;
		int countZ = 0;
		for (unsigned int  j = 0; j < fsize ; j++)
		{
			double triverts[3][3] = {
				{v[f[j].x - 1].x, v[f[j].x - 1].y, v[f[j].x - 1].z},
				{v[f[j].y - 1].x, v[f[j].y - 1].y, v[f[j].y - 1].z},
				{v[f[j].z - 1].x, v[f[j].z - 1].y, v[f[j].z - 1].z}
			};

			double boxCenter[3] = { r[i].x,r[i].y, r[i].z };
			double rayDir[] = { 1,0, 0 };
			double rayDir2[] = { 0,1, 0 };
			double rayDir3[] = { 0,0, 1 };
			double t, v, u;
			int x = intersect_triangle3(boxCenter, rayDir, triverts[0], triverts[1], triverts[2], &t, &u, &v);
			int y = intersect_triangle3(boxCenter, rayDir2, triverts[0], triverts[1], triverts[2], &t, &u, &v);
			int z = intersect_triangle3(boxCenter, rayDir3, triverts[0], triverts[1], triverts[2], &t, &u, &v);
			if (t > 0.0) {
				countX += x;
				countY += y;
				countZ += z;
			}
		}
		int oddCount = 0;

		if (countX % 2 == 1) oddCount++;
		if (countY % 2 == 1) oddCount++;
		if (countZ % 2 == 1) oddCount++;
		if (oddCount > 1) {
			c[i] = 1;
		}
		else c[i] = 0;

	});

	for (unsigned int i = 0; i < rhombuses.size(); i++)
	{
		if (c[i]==1) {
			rhombuses[i].removeOutside();
			rhombuses[i].isInside = true;
		}
		else {
			rhombuses[i].isInside = false;
		}
	}
	rhombuses.erase(std::remove_if(rhombuses.begin(),rhombuses.end(),
		[](Rhombus r) {return !r.isInside; }),
		rhombuses.end());
}