#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "RectMesh.h"
#include <glm/gtc/matrix_transform.hpp>

class Model {
private:
	Mesh mesh;

	RectMesh* bounds_ptr;
	bool draw_bounds;

public:
	Model(Mesh mesh_);
	~Model();
	void draw(Shader shader);
	bool intersects(Ray r, glm::mat4 model);
};

typedef struct RaycastResult {
	glm::vec3 point;
	glm::vec3 normal;
	float t;
	bool hit;
} RaycastResult;

typedef struct OBB {
	glm::vec3 position;
	glm::vec3 size; // HALF SIZE!
	glm::mat3 orientation;

	inline OBB() : size(1, 1, 1) {}
	inline OBB(const glm::vec3& p, const glm::vec3& s) :
		position(p), size(s) {
	}
	inline OBB(const glm::vec3& p, const glm::vec3& s, const glm::mat3& o) :
		position(p), size(s), orientation(o) {
	}
} OBB;

bool Raycast(const OBB& obb, const Ray& ray, RaycastResult* outResult);