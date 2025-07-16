#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>

#include "fileio.h"
#include "shader.h"

static const std::string VS_EXT = ".vert";
static const std::string FS_EXT = ".frag";

Shader::Shader(const std::string& path) {
	std::string v_src;
	std::string f_src;

	readFile(path + VS_EXT, v_src);
	readFile(path + FS_EXT, f_src);

	uint32_t vertexShader = compileShader(v_src, GL_VERTEX_SHADER);
	uint32_t fragmentShader = compileShader(f_src, GL_FRAGMENT_SHADER);

	handle = linkProgram(vertexShader, fragmentShader);
}

int Shader::uniformLocation(const char* name) {
	if (uniforms.contains(name)) {
		return uniforms[name];
	} else {
		int location = glGetUniformLocation(handle, name);
		if (location == -1) {
			printf("Could not find uniform %s\n", name);
		}
		uniforms[name] = location;
		return location;
	}
}

void Shader::uniform1f(const char* name, float value) {
	glUniform1f(uniformLocation(name), value);
}

void Shader::uniform2f(const char* name, const glm::vec2& value) {
	glUniform2f(uniformLocation(name), value.x, value.y);
}

void Shader::uniform1ui(const char* name, uint32_t value) {
	glUniform1ui(uniformLocation(name), value);
}

// pub fn set_uniform_vec2(&self, name: &str, v: &Vector2<f32>) {
//     unsafe {
//         let location = self.get_location(name);
//         if location != -1 {
//             gl::Uniform2fv(location, 1, v.as_ptr().cast());
//         }
//     }
// }

void Shader::uniformMatrix4(const char* name, const glm::mat4& mat) {
	glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::bind() {
	glUseProgram(handle);
}

uint32_t Shader::compileShader(const std::string& src, int type) {
	uint32_t shader = glCreateShader(type);
	const char* source = src.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		int maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];
		int length = 0;
		glGetShaderInfoLog(shader, maxLength, &length, infoLog);

		glDeleteShader(shader);

		switch (type) {
			case GL_VERTEX_SHADER:
				std::cout << "Vertex ";
				break;
			case GL_GEOMETRY_SHADER:
				std::cout << "Geometry ";
				break;
			case GL_FRAGMENT_SHADER:
				std::cout << "Fragment ";
				break;
		}
		std::cout << "shader compilation failure!" << std::endl << infoLog << std::endl;

		return 0;
	}

	return shader;
}

uint32_t Shader::linkProgram(uint32_t vertexShader, uint32_t fragmentShader) {
	uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	int isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE) {
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* infoLog = new char[maxLength];
		int length = 0;
		glGetProgramInfoLog(program, maxLength, &length, infoLog);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::cout << "Shader link failure!" << std::endl;
		std::cout << infoLog << std::endl;

		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
