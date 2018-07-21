#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shaders.h"
#include "Data.h"

using std::cout;
using std::endl;

void frame_buffersize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char** argv)
{
	// initialize GLFW
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		return -1;
	}

	// create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 800, "HelloTriangle", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffersize_callback);
	glfwSetKeyCallback(window, key_callback);

	// load OpenGL Process'
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to load OpenGL" << endl;
		glfwTerminate();
		return -1;
	}

	// shaders
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &shader_sources::vertex, nullptr);
	glShaderSource(fragmentShader, 1, &shader_sources::fragment, nullptr);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	auto checkCompilation = [](GLuint shader, const char* errorMsg) {
		int success;
		char infoLog[512];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			cout << errorMsg << "\n" << infoLog << endl;
			return false;
		}
		return true;
	};
	checkCompilation(vertexShader, "ERROR: VERTEX SHADER COMPILATION FAILED");
	checkCompilation(fragmentShader, "ERROR: VERTEX FRAGMENT COMPILATION FAILED");

	// program
	auto shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	auto checkLinking = [](GLuint program, const char* errorMsg) {
		int success;
		char infoLog[512];

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			cout << errorMsg << "\n" << infoLog;

			return false;
		}
		return true;
	};
	checkLinking(shaderProgram, "ERROR: SHADER PROGRAM LINKING FAILED");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// array objects
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data::vertices), data::vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// setup
	glUseProgram(shaderProgram);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// draw
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}