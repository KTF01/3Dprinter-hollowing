#pragma once
#include <vector>
#include "glm/glm.hpp"
class Mesh
{
public:

	void addVertex(const glm::vec3& vertex) {
		vertices.push_back(vertex);
	}
	void addIndex(unsigned int index) {
		indices.push_back(index);
	}
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	glm::vec3 center = glm::vec3(0);
	//float minx = 0, miny = 0, minz = 0, maxx = 0, maxy = 0, maxz = 0;
private:
	bool inited = false;
};