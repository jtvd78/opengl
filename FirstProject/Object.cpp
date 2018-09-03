#include "Object.h"

Object::Object(glm::vec3 position, glm::vec3 angle, float scale) : position(position), angle(angle), valid_model(false), scale(scale) {}

void Object::draw(Shader shader) {
	update_model();
	shader.setMat4("model", model_cache);
	draw_internal(shader);
}

void Object::update_model() {
	if (!valid_model) {
		glm::mat4 model;
		model = glm::translate(model, position);
		model = glm::rotate(model, angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale));
		model_cache = model;
		valid_model = true;
	}
}

glm::mat4 Object::get_model_cache() {
	update_model();
	return model_cache;
}

void Object::set_position(glm::vec3 new_position) {
	position = new_position;
	invalidate_model();
}

void Object::translate(glm::vec3 translation) {
	auto next = glm::translate(glm::mat4(1.0f), translation) * glm::vec4(position, 1.0f);
	position.x = next.x;
	position.y = next.y;
	position.z = next.z;
	invalidate_model();
}

void Object::set_angle(glm::vec3 new_angle) {
	angle = new_angle;
	invalidate_model();
}

void Object::rotate(glm::vec3 add_angle) {
	angle = angle + add_angle;
	invalidate_model();
}

void Object::invalidate_model() {
	valid_model = false;
}


float Object::get_scale() {
	return scale;
}

glm::vec3 Object::get_angle() {
	return angle;
}

glm::vec3 Object::get_position() {
	return position;
}