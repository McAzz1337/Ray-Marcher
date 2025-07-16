#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shadow.h>

#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.h"

struct Vertex {
	glm::vec3 pos;

	Vertex(const glm::vec3 pos) : pos(pos) {
	}
};

int main() {
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		glfwTerminate();
		return 1;
	}

	int width = 1440;
	int height = 1080;
	float aspect = (float)width / (float)height;
	char title[12] = "Ray Marcher";

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit()) {
		printf("Failed to initialize GLEW\n");
		glfwTerminate();
		return 1;
	}

	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	uint32_t vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	{
		const int vertexCount = 4;
		float halfAspect = aspect * 0.5;
		Vertex vertices[vertexCount] = {
		    Vertex({-halfAspect, -0.5f, 0.0}),
		    Vertex({halfAspect, -0.5f, 0.0}),
		    Vertex({halfAspect, 0.5f, 0.0}),
		    Vertex({-halfAspect, 0.5f, 0.0}),
		};
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	}

	uint32_t ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	const int indexCount = 6;
	{
		uint32_t indices[indexCount] = {0, 1, 2, 0, 2, 3};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);

	Shader shader("shaders/shader");
	shader.bind();
	shader.uniform2f("resolution", {width, height});

	glClearColor(0.3, 0.3, 0.3, 1.0);

	glm::mat4 proj = glm::perspective((float)M_PI / 3.0f, aspect, 0.001f, 100.0f);
	glm::mat4 cam = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -0.86f});
	const glm::mat4 mvp = proj * cam;

	auto last = std::chrono::steady_clock::now();
	auto startTime = last;
	uint64_t frames = 0;

	while (!glfwWindowShouldClose(window)) {
		auto now = std::chrono::steady_clock::now();
		float delta = std::chrono::duration<float>(now - last).count();
		last = now;
		float elapsedSeconds = std::chrono::duration<float>(now - startTime).count();
		shader.uniform1f("time", elapsedSeconds);
		shader.uniform1ui("frame", frames);

		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		shader.uniformMatrix4("mvp", mvp);

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

		frames++;
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
