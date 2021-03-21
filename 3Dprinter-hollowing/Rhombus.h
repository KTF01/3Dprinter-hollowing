#pragma once
#include "glm/glm.hpp"
#include "Mesh.h"

extern double alpha;

class Rhombus: public Mesh
{
public:
	Rhombus(double _height, double _width, glm::vec<3,double> _center, double t = 0);
	Rhombus();
	bool operator==(const Rhombus& r) {
		return height == r.height && r.width == width && center==r.center;
	}
	double height;
	double width;
	double depth;
	double t;
	bool isBound = false;
	bool isInside = false;
	std::vector<Rhombus*> backRhombuses;
	//int backRhombusIndex = -1;
private:
	inline void addIndexes() {
		addIndex({1,2,4});
		addIndex({3,4,2});

		addIndex({1, 5, 2});
		addIndex({2, 5, 6});

		addIndex({2, 3, 6});
		addIndex({3, 6, 7});

		addIndex({7, 8, 3});
		addIndex({3, 4, 8});

		addIndex({4, 8, 5});
		addIndex({5, 1, 4});

		addIndex({ 5,6,8 });
		addIndex({ 7,8,6 });
	}
};