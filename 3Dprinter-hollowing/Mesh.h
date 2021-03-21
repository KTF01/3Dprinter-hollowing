#pragma once
#include <vector>
#include "glm/glm.hpp"

typedef glm::vec<3, unsigned int, glm::defaultp> Face;

class Mesh
{
public:

	void addVertex(const glm::vec<3, double>& vertex) {
		vertices.push_back(vertex);
	}
	void addIndex(Face face) {
		faces.push_back(face);
	}
	std::vector<Face> faces;
	std::vector<glm::vec<3, double>> vertices;
	glm::vec<3, double> center = glm::vec<3, double>(0);
	double aabbX = 0, aabbY = 0, aabbZ = 0;
	//float minx = 0, miny = 0, minz = 0, maxx = 0, maxy = 0, maxz = 0;
private:
	bool inited = false;
};