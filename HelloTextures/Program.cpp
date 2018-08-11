#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <iostream>
#include <memory>

#include "../Utils/ShaderProgram.h"
#include "Data.h"

using namespace utils;

std::unique_ptr<utils::ShaderProgram> shader;

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
	else
	{
		static float mixVariable = 0.0f;
		
		if (key == GLFW_KEY_UP)
		{
			mixVariable += 0.01;
			shader->setUniform("mixVariable", mixVariable);
		}
		else if (key == GLFW_KEY_DOWN)
		{
			mixVariable -= 0.01;
			shader->setUniform("mixVariable", mixVariable);
		}
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

	shader =  std::make_unique<utils::ShaderProgram>("shader.vert", "shader.frag", utils::ShaderProgram::InputMode::File);

	// VAO and VBO
	unsigned int VAO, VBO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data::vertices), data::vertices, GL_STATIC_DRAW);

	// load textures
	stbi_set_flip_vertically_on_load(true);
	auto loadTexture = [](unsigned int& texture, GLenum num_texture, const char* image)
	{
		glActiveTexture(num_texture);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		int width, height, nrChannels;
		auto data = stbi_load(image, &width, &height, &nrChannels, 0);

		if (data == nullptr)
		{
			std::cout << "Failed to load texture:\n\t" << stbi_failure_reason() << std::endl;
			return false;
		}

		auto format = nrChannels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		return true;
	};

	unsigned int texture1, texture2;
	if (loadTexture(texture1, GL_TEXTURE0, "wall.jpg"))
	{
		glBindTexture(GL_TEXTURE_2D, texture1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (loadTexture(texture2, GL_TEXTURE1, "awesomeface.png"))
	{
		glBindTexture(GL_TEXTURE_2D, texture2);

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
	shader->use();
	shader->setUniform("texture1", 0);
	shader->setUniform("texture2", 1);

	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		// pool events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	glfwTerminate();
}

