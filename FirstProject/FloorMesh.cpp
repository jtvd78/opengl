#include "FloorMesh.h"

const float FloorMesh::vertices[] = {
	// positions            // normals         // texcoords
	100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
	-100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,

	100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
	-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,
	100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,  100.0f, 100.0f
};

FloorMesh::FloorMesh(Texture texture) : Mesh(vertices, 6, texture) {}