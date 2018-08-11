#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <iostream>

#include "../Utils/ShaderProgram.h"
#include "Data.h"


using namespace utils;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 800, "Learn Textures", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	// load OpenGL process
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load OpenGL process" << std::endl;
		glfwTerminate();
	}

	utils::ShaderProgram shader{ "shader.vert", "shader.frag", utils::ShaderProgram::InputMode::File };

	// VAO and VBO
	unsigned int VAO, VBO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data::vertices), data::vertices, GL_STATIC_DRAW);

	// load textures
	auto loadTexture = [](unsigned int& texture, GLenum target, const char* image)
	{
		glGenTextures(1, &texture);
		glBindTexture(target, texture);

		int width, height, nrChannels;
		auto data = stbi_load(image, &width, &height, &nrChannels, 0);

		if (data == nullptr)
		{
			std::cout << "Failed to load texture:\n\t" << stbi_failure_reason() << std::endl;
			return false;
		}

		glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);

		stbi_image_free(data);
		return true;
	};

	unsigned int texture;
	if (loadTexture(texture, GL_TEXTURE_2D, "wall.jpg"))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// setup
	shader.use();
	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		// pool events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

