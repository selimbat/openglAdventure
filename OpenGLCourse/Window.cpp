#include "Window.h"

Window::Window()
{
	_width = 800;
	_height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	_width = windowWidth;
	_height = windowHeight;

}

int Window::Initialise()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("ERROR: GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Setting profile to core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	_window = glfwCreateWindow(_width, _height, "Test Window", NULL, NULL);
	if (!_window)
	{
		printf("ERROR: GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(_window, &_bufferWidth, &_bufferHeight);

	// Set Context for GLEW to use
	glfwMakeContextCurrent(_window);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("ERROR: GLEW initialization failed!");
		glfwDestroyWindow(_window);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, _bufferWidth, _bufferHeight);
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}