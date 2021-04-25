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
	void addFace(Face face);
	void addNormal(Face face);
	void addNormal(glm::vec<3, double> normal) {
		normals.push_back(normal);
	}
	glm::vec<3, glm::vec<3, double>> getVertexesByFace(Face face);
	std::vector<Face> faces;
	std::vector<glm::vec<3, double>> normals;
	std::vector<glm::vec<3, double>> vertices;
	glm::vec<3, double> center = glm::vec<3, double>(0);
	double aabbX = 0, aabbY = 0, aabbZ = 0;
private:
	bool inited = false;
};