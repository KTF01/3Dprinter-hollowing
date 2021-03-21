#include "RhombusGrid.h"

RhombusGrid::RhombusGrid(Rhombus& rootRhombus) {
	root = &rootRhombus;
	rhombuses.push_back(rootRhombus);
}

std::vector<Rhombus> RhombusGrid::splitRhombus(Rhombus* rhombus/*, std::vector<Rhombus> &tmp*/) {
	std::vector<Rhombus> tmp;
	glm::vec3 c = rhombus->center;
	Rhombus r1(rhombus->height / 2.0f, rhombus->width / 2.0f, c + glm::vec3(0, rhombus->height / 4.0f, -rhombus->depth/4.0), rhombus->t);
	Rhombus r2(rhombus->height / 2.0f, rhombus->width / 2.0f, c + glm::vec3(rhombus->width / 4.0f, 0, -rhombus->depth / 4.0), rhombus->t);
	Rhombus r3(rhombus->height / 2.0f, rhombus->width / 2.0f, c - glm::vec3(0, rhombus->height / 4.0f, rhombus->depth / 4.0), rhombus->t);
	Rhombus r4(rhombus->height / 2.0f, rhombus->width / 2.0f, c - glm::vec3(rhombus->width / 4.0f, 0, rhombus->depth / 4.0), rhombus->t);
	
	tmp.push_back(r1);
	tmp.push_back(r2);
	tmp.push_back(r3);
	tmp.push_back(r4);

	Rhombus r5(rhombus->height / 2.0f, rhombus->width / 2.0f, c + glm::vec3(0, rhombus->height / 4.0f, rhombus->depth / 4.0), rhombus->t);
	Rhombus r6(rhombus->height / 2.0f, rhombus->width / 2.0f, c + glm::vec3(rhombus->width / 4.0f, 0, rhombus->depth / 4.0), rhombus->t);
	Rhombus r7(rhombus->height / 2.0f, rhombus->width / 2.0f, c - glm::vec3(0, rhombus->height / 4.0f, -rhombus->depth / 4.0), rhombus->t);
	Rhombus r8(rhombus->height / 2.0f, rhombus->width / 2.0f, c - glm::vec3(rhombus->width / 4.0f, 0, -rhombus->depth / 4.0), rhombus->t);

	tmp.push_back(r5);
	tmp.push_back(r6);
	tmp.push_back(r7);
	tmp.push_back(r8);

	tmp[0].backRhombuses.push_back(&tmp[4]);
	tmp[1].backRhombuses.push_back(&tmp[5]);
	tmp[2].backRhombuses.push_back(&tmp[6]);
	tmp[3].backRhombuses.push_back(&tmp[7]);;
	
	return tmp;
}

void RhombusGrid::subDivide() {
	unsigned int initialSize = rhombuses.size();
	for (unsigned int i = 0; i < initialSize; i++)
	{
		splitRhombus(&rhombuses[i]);
	}
}

void RhombusGrid::removeRhombus(Rhombus &r, float isBack) {
	//rhombuses.erase(std::remove(rhombuses.begin(), rhombuses.end(), r), rhombuses.end());
	for (unsigned int i = 0; i < r.vertices.size(); i++)
	{
		r.vertices[i] += glm::vec3(0,0,isBack/2);
	}
}