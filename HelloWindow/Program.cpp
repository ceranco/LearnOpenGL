#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>

void framebuffer_callback(GLFWwindow* window, int width, int height)
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
	// initialize GLFW library
	if (!glfwInit())
	{
		std::cout << "Failed initialize GLFW" << std::endl;
		return -1;
	}

	// set OpenGL window hint and create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 800, "Hello Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_callback);
	glfwSetKeyCallback(window, key_callback);

	// loading OpenGL using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// swap the buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
