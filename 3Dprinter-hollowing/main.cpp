
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

bool isSsmaller(Rhombus* a, Rhombus* b) {
	return a->center.z < b->center.z;
}

float boundMinZ = 10000.0f;

double alpha = M_PI / 5.5;

int main()
{
	Mesh* rabbit = MeshParser::parse("objs/abstract.obj");
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

	/*for (unsigned int i = 0; i < rg.rhombuses.size(); i++) {
		if (!isRhombusOutsideBound(&rg.rhombuses[i], rabbit)) {
			if (isRayIntersectTriangle(&rg.rhombuses[i], rabbit)) {
					rg.removeRhombus(rg.rhombuses[i], 1);
			}
		}*/
	//	if (!rg.rhombuses[i].isBound && rg.rhombuses[i].center.z < 0) {
	//		rg.removeRhombus(rg.rhombuses[i], -2);
	//		/*if (isRayIntersectTriangle(&rg.rhombuses[i], rabbit)) {
	//			rg.removeRhombus(rg.rhombuses[i], 1);
	//		}*/
	//	}
	//	else if (!rg.rhombuses[i].isBound &&rg.rhombuses[i].center.z > 0) {
	//		//rg.removeRhombus(rg.rhombuses[i]);
	//		/*if (isRayIntersectTriangle(&rg.rhombuses[i], rabbit)) {
	//			rg.removeRhombus(rg.rhombuses[i], -1);
	//		}*/
	//	}
	//}


	for (size_t i = 0; i < rg.rhombuses.size(); i++)
	{

		mergeMeshes(rabbit, &rg.rhombuses[i]);
	}


	//Mesh* aabbR = new Mesh();
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

	//mergeMeshes(rabbit, aabbR);
	//MeshParser::exportMesh(*rabbit,"objs/output/test.obj");
	MeshParser::exportMeshSTL(rabbit,"objs/output/abstract.stl");
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
	if (r->height < rabbitHeight/8.0f) {
		if (!isRhombusIntersect(r, rabbit)) {
			if (!isRhombusOutsideBound(r, rabbit)) {
				if (isRhombusInside(r, rabbit)) {
					r->removeOutside();
					finalGrid.push_back(*r);
				}
			}
		}
		else {
			r->isBound = true;
		}
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
			if (isRhombusInside(r, rabbit)) {
				r->removeOutside();
				finalGrid.push_back(*r);
			}
		}
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

bool isRhombusInside(Rhombus* r, Mesh* rabbit) {
	double boxCenter0 = r->center.x;
	double boxCenter1 = r->center.y;
	double boxCenter2 = r->center.z;


	std::vector<int> counterVec;
	std::vector<int> counterVec2;
	std::vector<int> counterVec3;
	counterVec.resize(rabbit->faces.size());
	counterVec2.resize(rabbit->faces.size());
	counterVec3.resize(rabbit->faces.size());
	concurrency::array_view<Face, 1> f(rabbit->faces.size(), rabbit->faces);
	concurrency::array_view<glm::vec<3, double>, 1> v(rabbit->vertices.size(), rabbit->vertices);
	concurrency::array_view<int, 1> c(counterVec.size(), counterVec);
	concurrency::array_view<int, 1> c2(counterVec2.size(), counterVec2);
	concurrency::array_view<int, 1> c3(counterVec3.size(), counterVec3);

	concurrency::parallel_for_each(f.extent, [=](concurrency::index<1> j) restrict(amp) {

			double triverts[3][3] = {
				{v[f[j].x - 1].x, v[f[j].x - 1].y, v[f[j].x - 1].z},
				{v[f[j].y - 1].x, v[f[j].y - 1].y, v[f[j].y - 1].z},
				{v[f[j].z - 1].x, v[f[j].z - 1].y, v[f[j].z - 1].z}
			};
			double boxCenter[3] = { boxCenter0,boxCenter1, boxCenter2 };
			double rayDir[] = { 1,0, 0 };
			double rayDir2[] = { 0,1, 0 };
			double rayDir3[] = { 0,0, 1 };
			double t, v, u;
			int x = intersect_triangle3(boxCenter, rayDir, triverts[0], triverts[1], triverts[2], &t, &u, &v );
			int y = intersect_triangle3(boxCenter, rayDir2, triverts[0], triverts[1], triverts[2], &t, &u, &v );
			int z = intersect_triangle3(boxCenter, rayDir3, triverts[0], triverts[1], triverts[2], &t, &u, &v );
			if (t<0.0) {
				c[j]=x;
				c2[j]=y;
				c3[j]=z;
			}
		}
	);
	int count = 0;
	int count2 = 0;
	int count3 = 0;
	for (unsigned int i = 0; i < counterVec.size(); i++)
	{
		count += c.data()[i];
		count2 += c2.data()[i];
		count3 += c3.data()[i];
	}
	//std::cout << "Utkozesek szama: " << count << std::endl;
	int oddCount = 0;
	
	if (count % 2==1) {
		oddCount++;
	}
	if (count2 % 2 == 1) {
		oddCount++;
	}
	if (count3%2==1) {
		oddCount++;
	}
	if (oddCount > 1) {
		return true;
	}
	else return false;
}