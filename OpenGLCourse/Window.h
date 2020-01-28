#pragma once

#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat GetBufferWidth() { return (GLfloat)_bufferWidth; }
	GLfloat GetBufferHeight() { return (GLfloat)_bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(_window); }
	GLFWwindow* GetGLFWWindow() { return _window; }
	void SwapBuffers() { glfwSwapBuffers(_window); }

	~Window();
private:
	GLFWwindow* _window;
	GLint _width, _height;
	GLint _bufferWidth, _bufferHeight;
};

