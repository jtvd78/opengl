#pragma once


#include <glm/glm.hpp>

#include "Mesh.h"

class RectMesh : public Mesh {
public:
	RectMesh(glm::vec3 minumum, glm::vec3 maximum, Texture texture);

private:
	glm::vec3 minimum, maximum;
	void gen_vertices();
	void gen_indices();

};