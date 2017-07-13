#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

class Model {
private:
	Mesh mesh;

public:
	Model(Mesh mesh_);
	void draw(Shader shader);
};