#pragma once
#include "Mesh.h"


class FloorMesh : public Mesh {
private:
	static const float vertices[];
public:
	FloorMesh(Texture texture);
};