#pragma once
#include <glm/glm.hpp>

class Ray {
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	glm::vec3 origin, direction, inverse_dir;
	int sign[3];
};