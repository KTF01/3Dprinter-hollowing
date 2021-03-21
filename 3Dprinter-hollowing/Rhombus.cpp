#include "Rhombus.h"
#include <iostream>

Rhombus::Rhombus(double _height, double _width, glm::vec<3, double> _center, double _t ) :height(_height), width(_width), depth(_width), t(_t) {
	center = _center;

	addVertex(glm::vec<3, double>(0, height / 2.0f, depth/2.0) + center);
	addVertex(glm::vec<3, double>(width / 2.0f, 0, depth / 2.0) + center);
	addVertex(glm::vec<3, double>(0, -(height / 2.0f), depth / 2.0) + center);
	addVertex(glm::vec<3, double>(-(width / 2.0f), 0, depth / 2.0) + center);
	addVertex(glm::vec<3, double>(0, height / 2.0f, -(depth / 2.0)) + center);
	addVertex(glm::vec<3, double>(width / 2.0f, 0, -(depth / 2.0)) + center);
	addVertex(glm::vec<3, double>(0, -(height / 2.0f), -(depth / 2.0)) + center);
	addVertex(glm::vec<3, double>(-(width / 2.0f), 0, -(depth / 2.0)) + center);

	addIndexes();

	if (t != 0) {
		double h = t / sin(alpha / 2.0);
		double smallHeigt = height - 2 * h;
		if (smallHeigt > 0.0) {
			double smallWidth = smallHeigt * (width / height);
			Rhombus r(smallHeigt, smallWidth, center);
			vertices.insert(vertices.end(), r.vertices.begin(), r.vertices.end());
			for (unsigned int i = 0; i < r.faces.size(); i++)
			{
				r.faces[i] += 8;
			}
			faces.insert(faces.end(), r.faces.begin(), r.faces.end());
		}
		
	}
	
}
Rhombus::Rhombus() {
	center = glm::vec<3, double>(0, 0, 0);
	addVertex(glm::vec<3, double>(0, 0.3, 0.15) + center);
	addVertex(glm::vec<3, double>(0.15, 0, 0.15) + center);
	addVertex(glm::vec<3, double>(0, -0.3, 0.15) + center);
	addVertex(glm::vec<3, double>(-0.15, 0, 0.15) + center);

	addVertex(glm::vec<3, double>(0, 0.3, -0.15) + center);
	addVertex(glm::vec<3, double>(0.15, 0, -0.15) + center);
	addVertex(glm::vec<3, double>(0, -0.3, -0.15) + center);
	addVertex(glm::vec<3, double>(-0.15, 0, -0.15) + center);
	height = vertices[0].y * 2;
	width = vertices[1].x * 2;
	depth = width;
	addIndexes();
}