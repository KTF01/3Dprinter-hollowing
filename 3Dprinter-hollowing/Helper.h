#include "glm/glm.hpp"
#include <vector>

glm::vec<3, double> calculateSurfaceNormal(glm::vec<3, double> triangle[]) {
	glm::vec<3, double> normal;

	glm::vec<3, double> U = triangle[1] - triangle[0];
	glm::vec<3, double> V = triangle[2] - triangle[0];

	normal.x = (U.y * V.z) - (U.z * V.y);
	normal.y = (U.z * V.x) - (U.x * V.z);
	normal.z = (U.x * V.y) - (U.y * V.x);
	return normal;
}
