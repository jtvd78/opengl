#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object {
private:
	glm::vec3 position;
	glm::vec3 angle;
	float scale;

	bool valid_model;
	void invalidate_model();

	glm::mat4 model_cache;

	virtual void draw_internal(Shader shader) = 0;

public:
	Object(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 angle = glm::vec3(0.0f, 0.0f, 0.0f), float scale = 1.0f);
	void draw(Shader shader);

	void set_position(glm::vec3 new_position);
	void translate(glm::vec3 translation);
	void set_angle(glm::vec3 new_angle);
	void rotate(glm::vec3 add_angle);
};
