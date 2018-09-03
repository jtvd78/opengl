#include "RayMesh.h"


#define LENGTH 50.0f
#define WIDTH 0.03f

const float RayMesh::verticies[] = {
	//top
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	LENGTH, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	LENGTH, 0.0f, WIDTH, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, WIDTH, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	LENGTH, 0.0f, WIDTH, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

	//bottom
	0.0f, -WIDTH, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	LENGTH, -WIDTH, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	LENGTH, -WIDTH, WIDTH, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

	0.0f, -WIDTH, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, -WIDTH, WIDTH, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	LENGTH, -WIDTH, WIDTH, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

	/*
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, -0.2f, 0.0f,

	1.0f, 0.0f, 0.0f,
	0.0f, -0.2f, 0.0f,
	1.0f, -0.2f, 0.0f,
	*/

	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	LENGTH, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, -WIDTH, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

	LENGTH, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, -WIDTH, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	LENGTH, -WIDTH, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,


	0.0f, 0.0f, WIDTH, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	LENGTH, 0.0f, WIDTH, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, -WIDTH, WIDTH, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

	LENGTH, 0.0f, WIDTH, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, -WIDTH, WIDTH, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	LENGTH, -WIDTH, WIDTH, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,






};

RayMesh::RayMesh(Texture texture) : Mesh(verticies, 24, texture) {}


RayMesh::~RayMesh() {}
