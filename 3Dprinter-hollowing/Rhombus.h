#pragma once
#include "glm/glm.hpp"
#include "Mesh.h"

extern double alpha;

class Rhombus: public Mesh
{
public:
	Rhombus(double _height, double _width, glm::vec<3,double> _center, double t = 0, bool isInverted= false);
	Rhombus();
	bool operator==(const Rhombus& r) {
		return height == r.height && r.width == width && center==r.center;
	}
	void removeOutside();
	double height;
	double width;
	double depth;
	double t;
	bool isBound = false;
	bool isInside = false;
	std::vector<Rhombus*> backRhombuses;
	//int backRhombusIndex = -1;
private:
	void addIndexes();
	void addInvertedIndexes();
};