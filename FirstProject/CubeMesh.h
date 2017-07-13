#pragma once
#include "Mesh.h"

class CubeMesh : public Mesh {
private:
	static const float vertices[];
public:
	CubeMesh(Texture texture);
};