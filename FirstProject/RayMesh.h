#pragma once

#include "Mesh.h"
#include "Ray.h"

class RayMesh : public Mesh {
public:
	RayMesh(Texture texture);
	~RayMesh();

private:
	static const float verticies[];
};

