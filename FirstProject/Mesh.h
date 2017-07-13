#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


#include "Shader.h"

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Texture texture;
	unsigned int VAO;

	Mesh(const float vertices_arr[], int count, Texture texture) : vertices(count), indices(count){

		for (int i = 0; i < count; i++) {
			Vertex* vert = vertices.data() + i;
			vert->Position.x = vertices_arr[i * 8];
			vert->Position.y = vertices_arr[i * 8 + 1];
			vert->Position.z = vertices_arr[i * 8 + 2];

			vert->Normal.x = vertices_arr[i * 8 + 3];
			vert->Normal.y = vertices_arr[i * 8 + 4];
			vert->Normal.z = vertices_arr[i * 8 + 5];

			vert->TexCoords.x = vertices_arr[i * 8 + 6];
			vert->TexCoords.y = vertices_arr[i * 8 + 7];

			indices[i] = i;
		}

		for (int i = 0; i < count; i++) {
			std::cout << indices[i] << " ";
		}

		std::cout << std::endl;

		this->texture = texture;
		setup_mesh();
	}

	Mesh(Texture texture, int v_count, int i_count) : vertices(v_count), indices(i_count) {
		this->texture = texture;
	}

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture texture) {
		this->vertices = vertices;
		this->indices = indices;
		this->texture = texture;

		setup_mesh();
	}

	void draw(Shader shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		shader.setInt("texture_diffuse_1", 0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

protected:
	void setup_mesh() {

		unsigned int VBO, EBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};