#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <vector>

#include <memory>

#include "Shader.h"
#include "Ray.h"

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};



struct Bounds {
	float x_min, y_min, z_min, x_max, y_max, z_max;
};


struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh {
	
	
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Bounds bounds;

	Texture texture;
	unsigned int VAO;

	Mesh(const float vertices_arr[], int count, Texture texture);

	Mesh(Texture texture, int v_count, int i_count);

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture texture);

	void draw(Shader shader);

	Bounds get_bounds();

protected:
	void setup_mesh();
};