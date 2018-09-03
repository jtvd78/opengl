#include "ModelObject.h"

ModelObject::ModelObject(Model model) : Object(), model(model) {}

ModelObject::ModelObject(Model model, glm::vec3 position, glm::vec3 angle, float scale) : Object(position, angle, scale), model(model) {}

void ModelObject::draw_internal(Shader shader) {
	model.draw(shader);
}

bool ModelObject::intersects(glm::vec3 world_camera_origin, glm::vec3 world_ray_direction) {

	glm::mat4 obj_model = get_model_cache();
	glm::mat4 inv_model = glm::inverse(glm::scale(obj_model, glm::vec3(1.0f / get_scale())));

	//Camera position in model coordinates
	glm::mat4 rot;
	rot = glm::translate(rot, get_position());
	rot = glm::rotate(rot, get_angle().x, glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, get_angle().y, glm::vec3(0.0f, 1.0f, 0.0f));
	rot = glm::rotate(rot, get_angle().z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 model_camera_origin = rot * glm::vec4(world_camera_origin - get_position(), 1.0f);


	//	glm::mat4 rot2;
	//	rot2 = glm::translate(rot2, get_position());
	//	rot2 = glm::rotate(rot2, get_angle().x, glm::vec3(1.0f, 0.0f, 0.0f));
	//	rot2 = glm::rotate(rot2, get_angle().y, glm::vec3(0.0f, 1.0f, 0.0f));
	//	rot2 = glm::rotate(rot2, get_angle().z, glm::vec3(0.0f, 0.0f, 1.0f));
	//	glm::vec3 model_camera_direction = rot2 * glm::vec4(world_ray_direction, 0.0f);

		//Origin and direction relative to model. 
//	glm::vec3 model_direction = glm::vec4(world_ray_direction, 1.0f) * inv_model;
	//	glm::vec3 model_origin = glm::vec4(world_camera_origin, 1.0f) * inv_model;




	Ray r = Ray(model_camera_origin, world_ray_direction);



	return model.intersects(r, obj_model);
}