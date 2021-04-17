#pragma once
#include <vector>
#include "glm/glm.hpp"

typedef glm::vec<3, unsigned int, glm::defaultp> Face;

//glm::vec<3, double> calculateSurfaceNormal(glm::vec<3, double> triangle[]) {
//	glm::vec<3, double> normal;
//
//	glm::vec<3, double> U = triangle[1] - triangle[0];
//	glm::vec<3, double> V = triangle[2] - triangle[0];
//
//	normal.x = (U.y * V.z) - (U.z * V.y);
//	normal.y = (U.z * V.x) - (U.x * V.z);
//	normal.z = (U.x * V.y) - (U.y * V.x);
//	return normal;
//}

class Mesh
{
public:

	void addVertex(const glm::vec<3, double>& vertex) {
		vertices.push_back(vertex);
	}
	void addFace(Face face) {
		faces.push_back(face);
	}

	//glm::vec<3, glm::vec<3, double>> getVertexesByFace(Face face);
	std::vector<Face> faces;
	std::vector<glm::vec<3, double>> normals;
	std::vector<glm::vec<3, double>> vertices;
	glm::vec<3, double> center = glm::vec<3, double>(0);
	double aabbX = 0, aabbY = 0, aabbZ = 0;
private:
	bool inited = false;
};