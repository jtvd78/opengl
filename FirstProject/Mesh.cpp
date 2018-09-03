#include <limits>

#include "Mesh.h"


Mesh::Mesh(const float vertices_arr[], int count, Texture texture) : vertices(count), indices(count){

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

Mesh::Mesh(Texture texture, int v_count, int i_count) : vertices(v_count), indices(i_count) {
	this->texture = texture;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture texture) {
	this->vertices = vertices;
	this->indices = indices;
	this->texture = texture;

	setup_mesh();
}

void Mesh::draw(Shader shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	shader.setInt("texture_diffuse_1", 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setup_mesh() {

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

	bounds = get_bounds();
}

Bounds Mesh::get_bounds() {

	Bounds bounds;
	float min = std::numeric_limits<float>::min();
	float max = std::numeric_limits<float>::max();
	bounds = {max, max, max, min, min, min};

	for (int i = 0; i < vertices.size(); i++) {

		Vertex* v = vertices.data() + i;
		float x = v->Position.x;
		float y = v->Position.y;
		float z = v->Position.z;

		bounds.x_min = std::min(bounds.x_min, x);
		bounds.x_max = std::max(bounds.x_max, x);

		bounds.y_min = std::min(bounds.y_min, y);
		bounds.y_max = std::max(bounds.y_max, y);

		bounds.z_min = std::min(bounds.z_min, z);
		bounds.z_max = std::max(bounds.z_max, z);
	}

	return bounds;
}