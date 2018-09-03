
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Shader.h"
#include "Camera.h"

#include "Mesh.h"

#include "Model.h"
#include "ModelObject.h"

#include "CubeMesh.h"
#include "KnobMesh.h"
#include "FloorMesh.h"
#include "RayMesh.h"


unsigned int load_texture(std::string path);
void process_input(GLFWwindow *window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


glm::vec3 pointer_pos(0.0f, 0.0f, 0.0f);

//Screen size settings
int screen_height = 600;
int screen_width = 800;


Camera camera(glm::vec3(0.0f, 20.0f, 0.0f));
float lastX = screen_width / 2;
float lastY = screen_height / 2;
bool firstMouse = true;

bool free_mouse = false;

ModelObject* test_object_1 = NULL;
ModelObject* test_object_2 = NULL;
ModelObject* test_object_3 = NULL;
ModelObject* test_object_4 = NULL;
ModelObject* test_object_5 = NULL;

glm::vec3 test_position;

glm::mat4 get_projection_matrix() {
	return glm::perspective(glm::radians(camera.Zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
}

int main() {

	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OSx

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "First Project", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Scroll
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, screen_width, screen_height); //TODO: Not needed?

	//Wireframe
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Enable z-buffer
	glEnable(GL_DEPTH_TEST);

	Shader shader("lighting.vert", "lighting.frag");
	Shader normalShader("normal.vs", "normal.fs", "normal.gs");
	Shader lampShader("2.2.lamp.vs", "2.2.lamp.fs");

	stbi_set_flip_vertically_on_load(true);
	unsigned int wood_texture_id = load_texture("wood.png");
	unsigned int face_texture_id = load_texture("awesomeface.png");
	unsigned int knob_texture_id = load_texture("knob_2.png");
	unsigned int ground_texture_id = load_texture("ground_2.jpg");

	Texture wood_texture = Texture{ wood_texture_id, "texture_diffuse" };
	Texture face_texture = Texture{ face_texture_id, "texture_diffuse" };
	Texture knob_texture = Texture{ knob_texture_id, "texture_diffuse" };
	Texture ground_texture = Texture{ ground_texture_id, "texture_diffuse" };

	shader.use();
	shader.setInt("texture_diffuse1", 0);

	CubeMesh cube_mesh = CubeMesh(face_texture);
	FloorMesh floor_mesh = FloorMesh(ground_texture);
	KnobMesh knob_mesh = KnobMesh(12, 30, knob_texture);
	RayMesh ray_mesh(ground_texture);

	Model floor_model = Model(floor_mesh);
	Model cube_model = Model(cube_mesh);
	Model knob_model = Model(knob_mesh);
	Model ray_model(ray_mesh);

	ModelObject cube_object(cube_model);
	ModelObject lamp_object(cube_model, glm::vec3(0.0f), glm::vec3(0.0f), 0.2f);
	ModelObject floor_object(floor_model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	ModelObject knob_object_1(knob_model, glm::vec3(5.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	ModelObject knob_object_2(knob_model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

	ModelObject ray_object(ray_model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

	ModelObject cube_object_2(cube_model);
	ModelObject cube_object_3(cube_model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);


	test_object_2 = &knob_object_2;
	test_object_1 = &knob_object_1;
	test_object_3 = &cube_object_2;
	test_object_4 = &cube_object_3;
	test_object_5 = &ray_object;

	int numFrames = 0;

	float time_ctr = 0;
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		numFrames++;
		if (time_ctr >= 1.0) { // If last prinf() was more than 1 sec ago
											 // printf and reset timer
			printf("%f ms/frame, %d frames\n", 1000.0 *  time_ctr / float(numFrames), numFrames);
			numFrames = 0;
			time_ctr = 0;
		}
		time_ctr += deltaTime;

		// lighting info
		// -------------
		glm::vec3 lightPos(0.0f, 5.0f * cosf(7.0f / 3.0f * (float)glfwGetTime()) + 10.5f, 0.0f);
		lamp_object.set_position(lightPos);

		process_input(window, deltaTime);

		//Rendering code
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);

		//Projection + View Matricies
		glm::mat4 projection = get_projection_matrix();
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		cube_object.rotate(glm::vec3(0.0f, glm::radians(0.01), 0.0f));
		cube_object.set_position(pointer_pos);
		lamp_object.rotate(glm::vec3(0.0f, glm::radians(0.1), 0.0f));
		knob_object_1.rotate(glm::vec3(0.0f, glm::radians(0.005), 0.0f));




		glm::mat4 rot;
		rot = glm::translate(rot, knob_object_1.get_position());
		rot = glm::rotate(rot, knob_object_1.get_angle().x, glm::vec3(1.0f, 0.0f, 0.0f));
		rot = glm::rotate(rot, knob_object_1.get_angle().y, glm::vec3(0.0f, 1.0f, 0.0f));
		rot = glm::rotate(rot, knob_object_1.get_angle().z, glm::vec3(0.0f, 0.0f, 1.0f));
		test_position = rot * glm::vec4(camera.Position - knob_object_1.get_position(), 1.0f);
		test_object_3->set_position(test_position);

		ray_object.set_position(test_position);




		// Draw objects
		floor_object.draw(shader);
		cube_object.draw(shader);
		knob_object_1.draw(shader);
		knob_object_2.draw(shader);

		//	test_object_3->draw(shader);
		//	test_object_4->draw(shader);

		//ray_object.draw(shader);



		// Lamp
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lamp_object.draw(lampShader);



		// Normals
//		normalShader.use();
//		normalShader.setMat4("view", view);
//		normalShader.setMat4("projection", projection);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void on_tab_pressed(GLFWwindow* window) {
	free_mouse = !free_mouse;

	if (!free_mouse) {
		firstMouse = true;
	}

	glfwSetInputMode(window, GLFW_CURSOR, free_mouse ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

bool tab_pressed = false;

void process_input(GLFWwindow *window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	int tab_state = glfwGetKey(window, GLFW_KEY_TAB);

	if (tab_state == GLFW_PRESS && !tab_pressed) {
		tab_pressed = true;
		on_tab_pressed(window);
	} else if (tab_state == GLFW_RELEASE) {
		tab_pressed = false;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {


	if (free_mouse) {

		glm::mat4 proj = get_projection_matrix();
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec4 viewport(0.0f, 0.0f, screen_width, screen_height);

		glm::vec3 screen_near(xpos, screen_height - ypos, 0.0f);
		glm::vec3 ray_near = glm::unProject(screen_near, view, proj, viewport);

		glm::vec3 screen_far(xpos, screen_height - ypos, 1.0f);
		glm::vec3 ray_far = glm::unProject(screen_far, view, proj, viewport);

		glm::vec3 ray = glm::normalize(ray_near - ray_far);


		//	std::cout << ray.x << " " << ray.y << " " << ray.z << std::endl;

	//	std::cout << test_object_1->get_angle().y << std::endl;


		float x = ray.x;
		float y = ray.y;
		float z = ray.z;

		float t = std::atan(z / x);
		float p = std::atan(y / z);



		//	rot = glm::rotate(rot, test_object_1->get_angle().x, glm::vec3(1.0f, 0.0f, 0.0f));




		float ay = glm::orientedAngle(glm::vec3(1.0f, 0.0f, 0.0f), ray, glm::vec3(0.0f, 1.0f, 0.0f));
		float az = glm::orientedAngle(glm::vec3(1.0f, 0.0f, 0.0f), ray, glm::vec3(0.0f, 0.0f, 1.0f));

		std::cout << ay << " : " << az << std::endl;

		glm::mat4 rot;
		rot = glm::rotate(rot, ay, glm::vec3(0.0f, 1.0f, 0.0f));
		rot = glm::rotate(rot, az, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 test_ray = glm::normalize(glm::vec3(rot * glm::vec4()));



		test_object_5->set_angle(glm::vec3(0.0f, ay, -az));




		//	test_object_4->set_position(test_position - test_ray);

		float count = camera.Position.y / ray.y;
		float dX = ray.x * count;
		float dZ = ray.z * count;

		pointer_pos = glm::vec3(camera.Position.x - dX, 0.0f, camera.Position.z - dZ);


		if (test_object_1->intersects(camera.Position, test_ray)) {
			std::cout << "1" << std::endl;
		}

		if (test_object_2->intersects(camera.Position, ray)) {
			//	std::cout << "2" << std::endl;
		}

		return;
	}

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, width, height);
}

unsigned int load_texture(std::string path) {
	unsigned int id;
	glGenTextures(1, &id);

	int width, height, numChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	if (data) {

		GLenum format;
		if (numChannels == 1)
			format = GL_RED;
		else if (numChannels == 3)
			format = GL_RGB;
		else if (numChannels == 4)
			format = GL_RGBA;
		else {
			throw std::runtime_error("Incompatible texture - Wrong number of channels: " + numChannels);
		}

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

	} else {
		std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
	}

	return id;
}