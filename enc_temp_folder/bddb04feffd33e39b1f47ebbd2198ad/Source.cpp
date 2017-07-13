
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include "Shader.h"
#include "Camera.h"

#include "Mesh.h"

#include "Model.h"
#include "ModelObject.h"

#include "CubeMesh.h"
#include "KnobMesh.h"
#include "FloorMesh.h"


unsigned int load_texture(std::string path);
void process_input(GLFWwindow *window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


glm::vec3 pointer_pos(0.0f, 0.0f, 0.0f);

//Screen size settings
int SCR_HEIGHT = 600;
int SCR_WIDTH = 800;


Camera camera(glm::vec3(0.0f, 20.0f, 0.0f));
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool firstMouse = true;

bool free_mouse = false;

glm::mat4 get_projection_matrix() {
	return glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}



glm::vec3 create_mouse_ray_1(float mouseX, float mouseY) {
	float x_norm = mouseX / SCR_WIDTH * 2.0f - 1.0f;
	float y_norm = mouseY / SCR_HEIGHT * 2.0f - 1.0f;

	glm::mat4 proj = get_projection_matrix();
	glm::mat4 view = camera.GetViewMatrix();

	glm::vec4 screenPos = glm::vec4(x_norm, -y_norm, 0.0f, 1.0f);
	glm::vec4 worldPos = glm::inverse(proj * view) * screenPos;

	return glm::vec3(worldPos);
}

glm::vec3 create_mouse_ray_2(float mouseX, float mouseY) {
	float x = (2.0f * mouseX) / SCR_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / SCR_HEIGHT;
	float z = 0.0f;

	glm::vec3 screenPos(mouseX, SCR_HEIGHT-mouseY, z);

	glm::mat4 proj = get_projection_matrix();
	glm::mat4 view = camera.GetViewMatrix();

	glm::vec3 ray_wor = glm::unProject(screenPos, view, proj, glm::vec4(0.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT));
	ray_wor.x = -ray_wor.x;
	ray_wor.z = -ray_wor.z;
	return ray_wor;
}



int main() {

	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OSx

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First Project", NULL, NULL);
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

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //TODO: Not needed?

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

	Texture wood_texture = Texture{ wood_texture_id, "texture_diffuse" };
	Texture face_texture = Texture{ face_texture_id, "texture_diffuse" };
	Texture knob_texture = Texture{ knob_texture_id, "texture_diffuse" };

	shader.use();
	shader.setInt("texture_diffuse1", 0);

	// shader configuration
	// --------------------
//	shader.use();
//	shader.setInt("texture1", 0);


	CubeMesh cube_mesh = CubeMesh(wood_texture);
	FloorMesh floor_mesh = FloorMesh(wood_texture);
	KnobMesh knob_mesh = KnobMesh(12, 30, knob_texture);

	Model floor_model = Model(floor_mesh);
	Model cube_model = Model(cube_mesh);
	Model knob_model = Model(knob_mesh);

	ModelObject cube_object = ModelObject(cube_model);
	ModelObject lamp_object = ModelObject(cube_model, glm::vec3(0.0f), glm::vec3(0.0f), 0.2f);
	ModelObject floor_object = ModelObject(floor_model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	ModelObject knob_object_1 = ModelObject(knob_model, glm::vec3(3.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	ModelObject knob_object_2 = ModelObject(knob_model, glm::vec3(5.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

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
			printf("%f ms/frame, %d frames\n", 1000.0 *  time_ctr /  float(numFrames), numFrames);
			numFrames = 0;
			time_ctr = 0;
		}
		time_ctr += deltaTime;

		// lighting info
		// -------------
		glm::vec3 lightPos(1.0f, 20.0f * cosf(7.0f / 3.0f * (float)glfwGetTime()) + 20.5f, 1.0f);
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

		// Draw objects
		floor_object.draw(shader);
		cube_object.draw(shader);
		knob_object_1.draw(shader);
		knob_object_2.draw(shader);

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

		glm::vec3 dir1 = create_mouse_ray_1(xpos, ypos);
		glm::vec3 dir2 = create_mouse_ray_2(xpos, ypos);

		glm::vec3 ray = glm::normalize(camera.Position - dir2);

		float dY = camera.Position.y;
		float count = dY / ray.y;
		float dX = ray.x * count;
		float dZ = ray.z * count;

		glm::vec4 temp_ptr = glm::vec4(camera.Position.x + dX, 0.0f, camera.Position.z + dZ, 0.0f);

	//	glm::mat4 rot;
	//	rot = glm::rotate(rot, camera.Pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	//	rot = glm::rotate(rot, camera.Roll, glm::vec3(0.0f, 1.0f, 0.0f));
	//	temp_ptr = rot * temp_ptr;

		pointer_pos = glm::vec3(temp_ptr);



		std::cout << dir1.x << " : " << dir1.y << " : " << dir1.z << std::endl;
		std::cout << dir2.x << " : " << dir2.y << " : " << dir2.z << std::endl;
		std::cout << "=============" << std::endl;
		std::cout <<ray.x << " : " << ray.y << " : " << ray.z << std::endl;
		std::cout << "=================" << std::endl;

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
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
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