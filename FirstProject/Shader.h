#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glad/glad.h> // include glad to get all the required OpenGL headers

class Shader {

public:
	unsigned int ID;

	Shader(std::string vertex, std::string fragment, std::string geometryPath = "") {

		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;

		try {
			vertexCode = read_shader(vertex);
			fragmentCode = read_shader(fragment);

			if (geometryPath != "") {
				geometryCode = read_shader(geometryPath);
			}

		} catch (std::ifstream::failure e) {
			throw e;
		}

		int vertexShader;
		int fragmentShader;
		int geometryShader;

		try {
			vertexShader = compile_shader(vertexCode, GL_VERTEX_SHADER);
			fragmentShader = compile_shader(fragmentCode, GL_FRAGMENT_SHADER);

			unsigned int geometry;
			if (geometryPath != "") {
				geometryShader = compile_shader(geometryCode, GL_GEOMETRY_SHADER);
			}
		} catch (std::runtime_error e) {
			throw e;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);

		if (geometryPath != "") {
			glAttachShader(ID, geometryShader);
		}

		glLinkProgram(ID);

		try {
			check_link_error(ID);
		} catch (std::runtime_error e) {
			throw e;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void use() {
		glUseProgram(ID);
	}

	void setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setVec2(const std::string &name, const glm::vec2 &value) const {
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void setVec3(const std::string &name, const glm::vec3 &value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec4(const std::string &name, const glm::vec4 &value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void setMat2(const std::string &name, const glm::mat2 &mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const std::string &name, const glm::mat3 &mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const std::string &name, const glm::mat4 &mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void set4f(const std::string &name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

private:
	std::string read_shader(std::string filename) {
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			file.open(filename);
			std::stringstream stream;
			stream << file.rdbuf();
			file.close();
			return stream.str();
		} catch (std::ifstream::failure e) {
			throw e;
		}
	}

	int compile_shader(std::string code, GLenum type) {
		unsigned int id;
		int success;
		char infoLog[1024];

		const char * code_str = code.c_str();

		id = glCreateShader(type);
		glShaderSource(id, 1, &code_str, NULL);
		glCompileShader(id); 

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, 1024, NULL, infoLog);
			throw std::runtime_error(std::string(infoLog));
		}
		return id;
	}

	void check_link_error(unsigned int program) {
		int success;
		char infoLog[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 1024, NULL, infoLog);
			throw std::runtime_error("Linking error: " + std::string(infoLog));
		}
	}
};

#endif