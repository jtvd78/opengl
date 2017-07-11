#include <cmath>
#include <iostream>
const float  PI_F = 3.14159265358979f;

float get_x(float angle) {
	return sinf(angle / 180.0f * PI_F);
}

float get_radius(float y) {
	return 1.0f / 6.0f * log((-y + 1.0f) / 2.0f) + 1.0f;
}

float get_z(float angle) {
	return cosf(angle / 180.0f * PI_F);
}

float get_y(int row, int rows) {
	return 2.0f * powf(row / (rows - 1.0f), 1.0f/2.4f) - 1.0f;
	//return 2.0f * row / (rows - 1.0f) - 1.0f;
}


void fill_array(float *arr, int rows, int cols) {
	float angle_step = 360.0f / cols;

	int ctr = 0;

	for (int row = 0; row < rows - 1; row++) {
		float y = get_y(row, rows);
		float y_next = get_y(row + 1, rows);

		float radius = get_radius(y);

		float radius_next;
		if (row != rows - 2) {
			radius_next = get_radius(y_next);
		} else {
			radius_next = 0.0f;
		}		

		std::cout << "y: " << y << " y next: " << y_next << std::endl;
		std::cout << "radius" << radius <<  " radius next: " << radius_next << std::endl;

		//Left handed triangles /|
		for (int col = 0; col < cols; col+=2) {
			float angle_0 = angle_step * col;
			float angle_1 = angle_step * (col + 1);

			float x_0 = radius*get_x(angle_0);
			float z_0 = radius*get_z(angle_0);

			float x_1 = radius_next*get_x(angle_1);
			float z_1 = radius_next*get_z(angle_1);

			float x_2 = radius*get_x(angle_1);
			float z_2 = radius*get_z(angle_1);

			arr[ctr] = x_0;
			arr[ctr + 1] = y;
			arr[ctr + 2] = z_0;

			arr[ctr + 3] = x_1;
			arr[ctr + 4] = y_next;
			arr[ctr + 5] = z_1;

			arr[ctr + 6] = x_2;
			arr[ctr + 7] = y;
			arr[ctr + 8] = z_2;

			ctr += 9;
		}

		//Left handed triangles \| upside down
		for (int col = 1; col < cols; col += 2) {
			float angle_0 = angle_step * col;
			float angle_1 = angle_step * (col + 1);

			float x_0 = radius_next*get_x(angle_0);
			float z_0 = radius_next*get_z(angle_0);

			float x_1 = radius_next*get_x(angle_1);
			float z_1 = radius_next*get_z(angle_1);

			float x_2 = radius*get_x(angle_1);
			float z_2 = radius*get_z(angle_1);

			arr[ctr] = x_0;
			arr[ctr + 1] = y_next;
			arr[ctr + 2] = z_0;

			arr[ctr + 3] = x_1;
			arr[ctr + 4] = y_next;
			arr[ctr + 5] = z_1;

			arr[ctr + 6] = x_2;
			arr[ctr + 7] = y;
			arr[ctr + 8] = z_2;

			ctr += 9;
		}

		/*Right handed triangles |\    */
		for (int col = 1; col < cols; col += 2) {
			float angle_0 = angle_step * col;
			float angle_1 = angle_step * (col + 1);

			float x_0 = radius*get_x(angle_0);
			float z_0 = radius*get_z(angle_0);

			float x_1 = radius_next*get_x(angle_0);
			float z_1 = radius_next*get_z(angle_0);

			float x_2 = radius*get_x(angle_1);
			float z_2 = radius*get_z(angle_1);

			arr[ctr] = x_0;
			arr[ctr + 1] = y;
			arr[ctr + 2] = z_0;

			arr[ctr + 3] = x_1;
			arr[ctr + 4] = y_next;
			arr[ctr + 5] = z_1;

			arr[ctr + 6] = x_2;
			arr[ctr + 7] = y;
			arr[ctr + 8] = z_2;

			ctr += 9;
		}

		/*Right handed triangles |/  upside down   */
		for (int col = 0; col < cols; col += 2) {
			float angle_0 = angle_step * col;
			float angle_1 = angle_step * (col + 1);

			float x_0 = radius_next*get_x(angle_0);
			float z_0 = radius_next*get_z(angle_0);

			float x_1 = radius_next*get_x(angle_1);
			float z_1 = radius_next*get_z(angle_1);

			float x_2 = radius*get_x(angle_0);
			float z_2 = radius*get_z(angle_0);

			arr[ctr] = x_0;
			arr[ctr + 1] = y_next;
			arr[ctr + 2] = z_0;

			arr[ctr + 3] = x_1;
			arr[ctr + 4] = y_next;
			arr[ctr + 5] = z_1;

			arr[ctr + 6] = x_2;
			arr[ctr + 7] = y;
			arr[ctr + 8] = z_2;

			ctr += 9;
		}
	}
}
