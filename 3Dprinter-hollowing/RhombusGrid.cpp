#include "RhombusGrid.h"

RhombusGrid::RhombusGrid(Rhombus& rootRhombus) {
	root = &rootRhombus;
	rhombuses.push_back(rootRhombus);
}

std::vector<Rhombus> RhombusGrid::splitRhombus(Rhombus* rhombus/*, std::vector<Rhombus> &tmp*/) {
	std::vector<Rhombus> tmp;
	glm::vec3 c = rhombus->center;
	Rhombus r1(rhombus->height / 2.0f, rhombus->width / 2.0f, c + glm::vec3(0, rhombus->height / 4.0f, 0));
	Rhombus r2(rhombus->height / 2.0f, rhombus->width / 2.0f, c + glm::vec3(rhombus->width / 4.0f, 0, 0));
	Rhombus r3(rhombus->height / 2.0f, rhombus->width / 2.0f, c - glm::vec3(0, rhombus->height / 4.0f, 0));
	Rhombus r4(rhombus->height / 2.0f, rhombus->width / 2.0f, c - glm::vec3(rhombus->width / 4.0f, 0, 0));
	tmp.push_back(r1);
	tmp.push_back(r2);
	tmp.push_back(r3);
	tmp.push_back(r4);
	//rhombuses.erase(std::remove(rhombuses.begin(), rhombuses.end(), *rhombus), rhombuses.end());
	//delete rhombus;
	/*rhombuses.push_back(r1);
	rhombuses.push_back(r2);
	rhombuses.push_back(r3);
	rhombuses.push_back(r4);*/
	return tmp;
}

void RhombusGrid::subDivide() {
	unsigned int initialSize = rhombuses.size();
	for (unsigned int i = 0; i < initialSize; i++)
	{
		splitRhombus(&rhombuses[i]/*, tmp*/);
	}
}

void RhombusGrid::removeRhombus(Rhombus &r) {
	//rhombuses.erase(std::remove(rhombuses.begin(), rhombuses.end(), r), rhombuses.end());
	for (unsigned int i = 0; i < r.vertices.size(); i++)
	{
		r.vertices[i] += glm::vec3(0,0,1);
	}
}