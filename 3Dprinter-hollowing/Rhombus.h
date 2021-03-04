#pragma once
#include "glm/glm.hpp"
#include "Mesh.h"

class Rhombus: public Mesh
{
public:
	Rhombus(float _height, float _width, glm::vec3 _center);
	Rhombus();
	bool operator==(const Rhombus& r) {
		return height == r.height && r.width == width && center==r.center;
	}
	float height;
	float width;
	bool isBound = true;
private:
	inline void addIndexes() {
		addIndex({1,2,4});
		addIndex({3,4,2});
	}
};