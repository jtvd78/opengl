#include <cmath>
#include <iostream>
#include <algorithm> \\ min() max()
#include <glm/gtc/matrix_transform.hpp>
#include "KnobMesh.h"

//const float  PI_F = 3.14159265358979f;

KnobMesh::KnobMesh(int rows, int cols, Texture texture) :  
	Mesh(texture, 
		(1 + (rows - 1) * cols) * 8,				//Verticies count
		12 * (rows - 2) * cols / 2 + 6 * cols / 2),	//Indicies count
	rows(rows), cols(cols) {


	gen_vertices();
	gen_indices();
	setup_mesh();
}

void KnobMesh::gen_vertices() {
	float angle_step = 360.0f / cols;

	int ctr = 0;
	Vertex *v;

	for (int row = 0; row < rows - 1; row++) {

		float y = get_y(row);
		float radius = get_radius(y);
		float slope_angle = atanf(get_radius_slope(y));

		for (int col = 0; col < cols; col++) {
			float angle = angle_step * col;

			v = this->vertices.data() + ctr;
			
			//Position
			v->Position.x = radius*get_x(angle);
			v->Position.y = y;
			v->Position.z = radius*get_z(angle);

			glm::mat4 rot;
			rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
			rot = glm::rotate(rot, slope_angle, glm::vec3(1.0f, 0.0f, 0.0f));

			glm::vec4 norm(0.0f, 0.0f, 1.0f, 1.0f);
			norm = rot * norm;

			glm::vec3 norm3(norm.x, norm.y, norm.z);
			norm3 = glm::normalize(norm3);

			//Normal
			v->Normal.x = norm3.x;
			v->Normal.y = norm3.y;
			v->Normal.z = norm3.z;

			//Texture
			v->TexCoords.x = v->Position.x / 2.0f + 0.5f;
			v->TexCoords.y = v->Position.z / 2.0f + 0.5f;

			ctr++;
		}
	}

	v = this->vertices.data() + ctr;

	//Top
	v->Position.x = 0.0f;
	v->Position.y = 1.0f;
	v->Position.z = 0.0f;

	v->Normal.x = 0.0f;
	v->Normal.y = 1.0f;
	v->Normal.z = 0.0f;

	v->TexCoords.x = 0.5f;
	v->TexCoords.y = 0.5f;
}

void KnobMesh::gen_indices() {
	int ctr = 0;

	unsigned int* index = this->indices.data();

	for (int row = 0; row < rows - 2; row++) {

		for (int col = 0; col < cols; col += 2) {

			//Adding 1 to col is safe, adding 2 to col is unsafe (when at last col chunk)
			//If on the last chunk, we subtract to the beginning instead of adding 2
			//In order to not wrap around to next row
			int plus_two = (col == cols - 2) ? 2 - cols : 2;

			/* /| */
			*(index + ctr) = row * cols + col;
			*(index + ctr + 1) = row * cols + (col + 1);
			*(index + ctr + 2) = (row + 1) * cols + (col + 1);

			/* |\ */
			*(index + ctr + 3) = row * cols + (col + 1);
			*(index + ctr + 4) = (row + 1) * cols + (col + 1);
			*(index + ctr + 5) = row * cols + (col + plus_two);

			/* |/ */
			*(index + ctr + 6) = row * cols + col;
			*(index + ctr + 7) = (row + 1) * cols + col;
			*(index + ctr + 8) = (row + 1) * cols + (col + 1);

			/* \| */
			*(index + ctr + 9) = (row + 1) * cols + (col + 1);
			*(index + ctr + 10) = (row + 1) * cols + (col + plus_two);
			*(index + ctr + 11) = row * cols + (col + plus_two);

			ctr = ctr + 12;
		}
	}

	//Top
	int row = rows - 2;
	int top = (rows - 1) * cols;
	for (int col = 0; col < cols; col++) {

		*(index + ctr) = row * cols + col;
		*(index + ctr + 1) = row * cols + (col + (col != cols - 1 ? 1 : 1 - cols));
		*(index + ctr + 2) = top;

		ctr = ctr + 3;
	}
}

float KnobMesh::get_y(int row) {
	return 2.0f * powf(row / (rows - 1.0f), 1.0f / 2.4f) - 1.0f;
}

float KnobMesh::get_x(float angle) {
	return sinf(angle / 180.0f * PI_F);
}

float KnobMesh::get_radius(float y) {
	return 1.0f / 9.0f * log((-y + 1.0f) / 2.0f) + 1.0f;
}

// Derivitive of get_radius()
float KnobMesh::get_radius_slope(float y) {
	return -1.0f / (6.0f * (1.0f - y));
}

float KnobMesh::get_z(float angle) {
	return cosf(angle / 180.0f * PI_F);
}

Bounds get_bounds(float *vbo, int count) {
	Bounds bounds;

	for (int i = 0; i < count; i++) {
		bounds.x_min = std::min(bounds.x_min, vbo[i * 8]);
		bounds.x_max = std::max(bounds.x_max, vbo[i * 8]);
	
		bounds.y_min = std::min(bounds.y_min, vbo[i * 8 + 1]);
		bounds.y_max = std::max(bounds.y_max, vbo[i * 8 + 1]);

		bounds.z_min = std::min(bounds.z_min, vbo[i * 8 + 2]);
		bounds.z_max = std::max(bounds.z_max, vbo[i * 8 + 2]);
	}

	return bounds;
}