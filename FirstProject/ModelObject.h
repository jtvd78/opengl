#pragma once
#include "Object.h"
#include "Model.h"

class ModelObject : public Object {
private:
	Model model;
public:
	ModelObject(Model model);
	ModelObject(Model model, glm::vec3 position, glm::vec3 angle, float scale);
	virtual void draw_internal(Shader shader) ;

};