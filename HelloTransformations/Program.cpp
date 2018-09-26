#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "../Utils/ShaderProgram.h"
#include "Data.h"

void frame_buffersize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mask)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char** argv)
{
	// initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 800, "HelloTransformations", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, frame_buffersize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	// load OpenGL procedures
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load OpenGL procedures" << std::endl;
		glfwTerminate();
		return -1;
	}

	// shaders
	const std::string shaders_folder = "C:/Users/ceran/Desktop/Programming/LearnOpenGL/HelloTransformations/";
	utils::ShaderProgram shaderProgram{ shaders_folder + "shader.vert", shaders_folder + "shader.frag", utils::ShaderProgram::InputMode::File };

	// buffers setup
	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data::vertices), data::vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// pre-loop setup
	shaderProgram.use();
	glClearColor(0.3f, 0.23f, 0.6f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		// draw
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 transform{ 1.0f }; 
		transform = glm::translate(transform, glm::vec3{ 0.5f, -0.5f, 0.0f });
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3{ 0.0f, 0.0f, 1.0f });
		shaderProgram.setUniform("transform", transform);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		transform = glm::mat4{ 1.0f };
		transform = glm::translate(transform, glm::vec3{ -0.5f, 0.5f, 0.0f });
		transform = glm::scale(transform, glm::vec3{ std::sin((float)glfwGetTime()) });
		shaderProgram.setUniform("transform", transform);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}