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
	bool isleaf = true;
private:
	inline void addIndexes() {
		addIndex(1); addIndex(2); addIndex(4);
		addIndex(3); addIndex(4); addIndex(2);
	}
};