#pragma once
#include <vector>
#include "glm/glm.hpp"

typedef glm::vec<3, unsigned int, glm::defaultp> Face;

class Mesh
{
public:

	void addVertex(const glm::vec3& vertex) {
		vertices.push_back(vertex);
	}
	void addIndex(Face face) {
		faces.push_back(face);
	}
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	glm::vec3 center = glm::vec3(0);
	float aabbX = 0, aabbY = 0, aabbZ = 0;
	//float minx = 0, miny = 0, minz = 0, maxx = 0, maxy = 0, maxz = 0;
private:
	bool inited = false;
};