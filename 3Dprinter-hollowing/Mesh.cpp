#include "Mesh.h"
#include "Helper.h"

void Mesh::addFace(Face face) {
	faces.push_back(face);
}

void Mesh::addNormal(Face face) {
	glm::vec<3, double> triangle[3] = {
			{vertices[face.x - 1].x, vertices[face.x - 1].y, vertices[face.x - 1].z},
			{vertices[face.y - 1].x, vertices[face.y - 1].y, vertices[face.y - 1].z},
			{vertices[face.y - 1].x, vertices[face.y - 1].y, vertices[face.y - 1].z}
	};
	glm::vec<3, double> normal = calculateSurfaceNormal(triangle);
	normals.push_back(normal);
}

glm::vec<3, glm::vec<3, double>> Mesh::getVertexesByFace(Face face) {
	glm::vec<3, glm::vec<3, double>> triangle = {
					{vertices[face.x - 1].x, vertices[face.x - 1].y, vertices[face.x - 1].z},
					{vertices[face.y - 1].x, vertices[face.y - 1].y, vertices[face.y - 1].z},
					{vertices[face.z - 1].x, vertices[face.z - 1].y, vertices[face.z - 1].z}
	};
	return triangle;
}