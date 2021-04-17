#include "Rhombus.h"
#include <iostream>

Rhombus::Rhombus(double _height, double _width, glm::vec<3, double> _center, double _t, bool isInverted ) :height(_height), width(_width), depth(_width), t(_t) {
	center = _center;

	addVertex(glm::vec<3, double>(0, height / 2.0f, depth/2.0) + center);
	addVertex(glm::vec<3, double>(width / 2.0f, 0, depth / 2.0) + center);
	addVertex(glm::vec<3, double>(0, -(height / 2.0f), depth / 2.0) + center);
	addVertex(glm::vec<3, double>(-(width / 2.0f), 0, depth / 2.0) + center);
	addVertex(glm::vec<3, double>(0, height / 2.0f, -(depth / 2.0)) + center);
	addVertex(glm::vec<3, double>(width / 2.0f, 0, -(depth / 2.0)) + center);
	addVertex(glm::vec<3, double>(0, -(height / 2.0f), -(depth / 2.0)) + center);
	addVertex(glm::vec<3, double>(-(width / 2.0f), 0, -(depth / 2.0)) + center);

	if (isInverted) addInvertedIndexes();
	else addIndexes();

	if (t != 0) {
		double h = t / sin(alpha / 2.0);
		double smallHeigt = height - 2 * h;
		if (smallHeigt > 0.0) {
			double smallWidth = smallHeigt * (width / height);
			Rhombus r(smallHeigt, smallWidth, center, 0, true);
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

void Rhombus::addIndexes() {
		addFace({ 2,1,4 });
		addFace({ 4,3,2 });

		addFace({ 5,1,2 });
		addFace({ 5,2,6 });

		addFace({ 2,3,6 });
		addFace({ 6,3,7 });

		addFace({ 8, 7,3 });
		addFace({ 3, 4, 8 });

		addFace({ 8, 4, 5 });
		addFace({ 1,5, 4 });

		addFace({ 5,6,8 });
		addFace({ 7,8,6 });
}

void Rhombus::addInvertedIndexes() {
	addFace({ 1,2,4 });
	addFace({ 3,4,2 });

	addFace({ 1,5,2 });
	addFace({ 2,5,6 });

	addFace({ 3,2,6 });
	addFace({ 3,6,7 });

	addFace({ 7,8,3 });
	addFace({ 4,3,8 });

	addFace({ 4,8,5 });
	addFace({ 5,1,4 });

	addFace({ 6,5,8 });
	addFace({ 8,7,6 });
}

void Rhombus::removeOutside() {
	vertices.erase(vertices.begin(), vertices.begin()+8);
	faces.erase(faces.begin(), faces.begin()+12);
	for (size_t i = 0; i < faces.size(); i++)
	{
		faces[i] -= 8;
	}

}