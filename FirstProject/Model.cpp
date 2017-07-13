#include "Model.h"

Model::Model(Mesh mesh) : mesh(mesh) {}

void Model::draw(Shader shader) {
	mesh.draw(shader);
}