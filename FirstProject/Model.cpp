#include "Model.h"

Model::Model(Mesh mesh) : mesh(mesh), draw_bounds(true), bounds_ptr(nullptr) {}

void Model::draw(Shader shader) {
	mesh.draw(shader);

	//Draw bounds
	if (draw_bounds) {
		if (!bounds_ptr) {
			Bounds bounds = mesh.bounds;
			bounds_ptr = new RectMesh(
				glm::vec3(bounds.x_min, bounds.y_min, bounds.z_min),
				glm::vec3(bounds.x_max, bounds.y_max, bounds.z_max), mesh.texture);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		bounds_ptr->draw(shader);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Model::~Model() {
	if (bounds_ptr) {
		delete bounds_ptr;
	}
}

//model has position, rotation, translation
bool Model::intersects(Ray r, glm::mat4 model) {


	//Below is not my code. it just works so...


	glm::vec3 bounds[] = {
		glm::vec3(mesh.bounds.x_min, mesh.bounds.y_min, mesh.bounds.z_min),
		glm::vec3(mesh.bounds.x_max, mesh.bounds.y_max, mesh.bounds.z_max)
	};

	bounds[0] = model * glm::vec4(bounds[0], 1.0f);
	bounds[1] = model * glm::vec4(bounds[1], 1.0f);

	float tmin = (bounds[r.sign[0]].x - r.origin.x) * r.inverse_dir.x;
	float tmax = (bounds[1 - r.sign[0]].x - r.origin.x) * r.inverse_dir.x;
	float tymin = (bounds[r.sign[1]].y - r.origin.y) * r.inverse_dir.y;
	float tymax = (bounds[1 - r.sign[1]].y - r.origin.y) * r.inverse_dir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (bounds[r.sign[2]].z - r.origin.z) * r.inverse_dir.z;
	float tzmax = (bounds[1 - r.sign[2]].z - r.origin.z) * r.inverse_dir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	//	return ((tmin < t1) && (tmax > t0));
	return true;
}

#define CMP(x, y) \
	(fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

float Dot(const glm::vec3& l, const glm::vec3& r) {
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

float Magnitude(const glm::vec3& v) {
	return sqrtf(Dot(v, v));
}

glm::vec3 Normalized(const glm::vec3& v) {
	return v * (1.0f / Magnitude(v));
}



void ResetRaycastResult(RaycastResult* outResult) {
	if (outResult != 0) {
		outResult->t = -1;
		outResult->hit = false;
		outResult->normal = glm::vec3(0, 0, 1);
		outResult->point = glm::vec3(0, 0, 0);
	}
}




bool Raycast(const OBB& obb, const Ray& ray, RaycastResult* outResult) {
	ResetRaycastResult(outResult);

	const float* size = new float[3]{ obb.size.x, obb.size.y, obb.size.z };

	glm::vec3 p = obb.position - ray.origin;

	glm::vec3 X(obb.orientation[0].x, obb.orientation[1].x, obb.orientation[2].x);
	glm::vec3 Y(obb.orientation[0].y, obb.orientation[1].y, obb.orientation[2].y);
	glm::vec3 Z(obb.orientation[0].z, obb.orientation[1].z, obb.orientation[2].z);

	glm::vec3 f(
		Dot(X, ray.direction),
		Dot(Y, ray.direction),
		Dot(Z, ray.direction)
	);

	glm::vec3 e(
		Dot(X, p),
		Dot(Y, p),
		Dot(Z, p)
	);

	float t[6] = { 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 3; ++i) {
		if (CMP(f[i], 0)) {
			if (-e[i] - size[i] > 0 || -e[i] + size[i] < 0) {
				return false;
			}
			f[i] = 0.00001f; // Avoid div by 0!
		}

		t[i * 2 + 0] = (e[i] + size[i]) / f[i]; // tmin[x, y, z]
		t[i * 2 + 1] = (e[i] - size[i]) / f[i]; // tmax[x, y, z]
	}

	delete size;

	float tmin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));
	float tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));


	// if tmax < 0, ray is intersecting AABB
	// but entire AABB is behing it's origin
	if (tmax < 0) {
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) {
		return false;
	}

	// If tmin is < 0, tmax is closer
	float t_result = tmin;

	if (tmin < 0.0f) {
		t_result = tmax;
	}

	if (outResult != 0) {
		outResult->hit = true;
		outResult->t = t_result;
		outResult->point = ray.origin + ray.direction * t_result;

		glm::vec3 normals[] = {
			X,			// +x
			X * -1.0f,	// -x
			Y,			// +y
			Y * -1.0f,	// -y
			Z,			// +z
			Z * -1.0f	// -z
		};

		for (int i = 0; i < 6; ++i) {
			if (CMP(t_result, t[i])) {
				outResult->normal = Normalized(normals[i]);
			}
		}
	}
	return true;
}

