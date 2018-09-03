#include "RectMesh.h"

RectMesh::RectMesh(glm::vec3 minimum, glm::vec3 maximum, Texture texture) : minimum(minimum), maximum(maximum), Mesh(texture, 36, 36) { 
	gen_vertices();
	gen_indices();
	setup_mesh();
}

void RectMesh::gen_vertices() {
	vertices[0] = Vertex{ glm::vec3(minimum.x, minimum.y, minimum.z ), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) };
	vertices[1] = Vertex{ glm::vec3(maximum.x, minimum.y, minimum.z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) };
	vertices[2] = Vertex{ glm::vec3(maximum.x, maximum.y, minimum.z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) };
	vertices[3] = Vertex{ glm::vec3(maximum.x, maximum.y, minimum.z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) };
	vertices[4] = Vertex{ glm::vec3(minimum.x, maximum.y, minimum.z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) };
	vertices[5] = Vertex{ glm::vec3(minimum.x, minimum.y, minimum.z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) };

	vertices[6] = Vertex{ glm::vec3(minimum.x, minimum.y, maximum.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) };
	vertices[7] = Vertex{ glm::vec3(maximum.x, minimum.y, maximum.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) };
	vertices[8] = Vertex{ glm::vec3(maximum.x, maximum.y, maximum.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) };
	vertices[9] = Vertex{ glm::vec3(maximum.x, maximum.y, maximum.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) };
	vertices[10] = Vertex{ glm::vec3(minimum.x, maximum.y, maximum.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) };
	vertices[11] = Vertex{ glm::vec3(minimum.x, minimum.y, maximum.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) };

	vertices[12] = Vertex{ glm::vec3(minimum.x, maximum.y, maximum.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
	vertices[13] = Vertex{ glm::vec3(minimum.x, maximum.y, minimum.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	vertices[14] = Vertex{ glm::vec3(minimum.x, minimum.y, minimum.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	vertices[15] = Vertex{ glm::vec3(minimum.x, minimum.y, minimum.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	vertices[16] = Vertex{ glm::vec3(minimum.x, minimum.y, maximum.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	vertices[17] = Vertex{ glm::vec3(minimum.x, maximum.y, maximum.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) };

	vertices[18] = Vertex{ glm::vec3(maximum.x, maximum.y, maximum.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
	vertices[19] = Vertex{ glm::vec3(maximum.x, maximum.y, minimum.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	vertices[20] = Vertex{ glm::vec3(maximum.x, minimum.y, minimum.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	vertices[21] = Vertex{ glm::vec3(maximum.x, minimum.y, minimum.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	vertices[22] = Vertex{ glm::vec3(maximum.x, minimum.y, maximum.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	vertices[23] = Vertex{ glm::vec3(maximum.x, maximum.y, maximum.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) };

	vertices[24] = Vertex{ glm::vec3(minimum.x, minimum.y, minimum.z), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	vertices[25] = Vertex{ glm::vec3(maximum.x, minimum.y, minimum.z), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	vertices[26] = Vertex{ glm::vec3(maximum.x, minimum.y, maximum.z), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
	vertices[27] = Vertex{ glm::vec3(maximum.x, minimum.y, maximum.z), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
	vertices[28] = Vertex{ glm::vec3(minimum.x, minimum.y, maximum.z), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	vertices[29] = Vertex{ glm::vec3(minimum.x, minimum.y, minimum.z), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) };

	vertices[30] = Vertex{ glm::vec3(minimum.x, maximum.y, minimum.z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	vertices[31] = Vertex{ glm::vec3(maximum.x, maximum.y, minimum.z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	vertices[32] = Vertex{ glm::vec3(maximum.x, maximum.y, maximum.z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
	vertices[33] = Vertex{ glm::vec3(maximum.x, maximum.y, maximum.z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
	vertices[34] = Vertex{ glm::vec3(minimum.x, maximum.y, maximum.z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	vertices[35] = Vertex{ glm::vec3(minimum.x, maximum.y, minimum.z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
}

void RectMesh::gen_indices() {
	for (int i = 0; i < 36; i++) {
		indices[i] = i;
	}
}
