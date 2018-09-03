#pragma once
#include "Object.h"
#include "Model.h"

class ModelObject : public Object {
private:
	Model model;
public:
	ModelObject(Model model);
	ModelObject(Model model, glm::vec3 position, glm::vec3 angle, float scale);
	virtual void draw_internal(Shader shader);

	bool intersects(glm::vec3 world_camera_origin, glm::vec3 world_ray_direction);
};