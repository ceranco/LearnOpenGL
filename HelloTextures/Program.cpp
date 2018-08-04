#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window,  int width, int height) 
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

	while (!glfwWindowShouldClose(window))
	{

		// pool events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}