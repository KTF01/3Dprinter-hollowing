#include "Rhombus.h"

Rhombus::Rhombus(float _height, float _width, glm::vec3 _center) :height(_height), width(_width) {
	center = _center;
	addVertex(glm::vec3(0, height / 2.0f, 0) + center);
	addVertex(glm::vec3(width / 2.0f, 0, 0) + center);
	addVertex(glm::vec3(0, -(height / 2.0f), 0) + center);
	addVertex(glm::vec3(-(width / 2.0f), 0, 0) + center);
	addIndexes();
}
Rhombus::Rhombus() {
	center = glm::vec3(0, 0, 0);
	addVertex(glm::vec3(0, 0.3, 0) + center);
	addVertex(glm::vec3(0.15, 0, 0) + center);
	addVertex(glm::vec3(0, -0.3, 0) + center);
	addVertex(glm::vec3(-0.15, 0, 0) + center);
	height = vertices[0].y * 2;
	width = vertices[1].x * 2;
	addIndexes();
}