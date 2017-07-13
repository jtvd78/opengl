#pragma once
#include <cmath>
#include <iostream>
#include <vector>

#include "Mesh.h"

const float  PI_F = 3.14159265358979f;

typedef struct {
	float x_min, y_min, z_min, x_max, y_max, z_max;
} Bounds;

void fill_ebo(int *arr, int rows, int cols);
void fill_vbo(float *arr, int rows, int cols);
Bounds get_bounds(float *vbo, int count);

class KnobMesh : public Mesh {
public:
	KnobMesh(int rows, int cols, Texture texture);

private:
	int rows, cols;
	void gen_vertices();
	void gen_indices();

	static float get_x(float angle);
	static float get_radius(float y);
	static float get_radius_slope(float y);
	static float get_z(float angle);
	float get_y(int row);
	
};