#pragma once
#include "glm/glm.hpp"
class Voxel {
public:
	glm::vec3 centroid;
	float distanceValue = 0.0f;
	Voxel(glm::vec3 _centroid): centroid(_centroid){}
};