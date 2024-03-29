#pragma once

#include <vector>
#include "Rhombus.h"

class RhombusGrid
{
public:
	RhombusGrid(Rhombus& rootRhombus);
	void splitRhombus(Rhombus* rhombus, std::vector<Rhombus> &tmp);
	void subDivide();
	std::vector<Rhombus> rhombuses;
	Rhombus* root;
	void removeRhombus(Rhombus &r);
};

