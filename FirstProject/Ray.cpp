#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {
	inverse_dir = glm::vec3(1 / direction.x, 1 / direction.y, 1 / direction.z);
	sign[0] = (inverse_dir.x < 0);
	sign[1] = (inverse_dir.y < 0);
	sign[2] = (inverse_dir.z < 0);
}