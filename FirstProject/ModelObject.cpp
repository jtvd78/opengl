#include "ModelObject.h"

ModelObject::ModelObject(Model model) : Object(),  model(model) {}

ModelObject::ModelObject(Model model, glm::vec3 position, glm::vec3 angle, float scale) : Object(position, angle, scale), model(model) {}

void ModelObject::draw_internal(Shader shader) {
	model.draw(shader);
}