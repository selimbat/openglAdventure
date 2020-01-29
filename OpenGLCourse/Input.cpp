#include <stdio.h>

#include "Input.h"

Input::Input(GLFWwindow* attachedWindow)
{
	for (size_t i = 0; i < 1024; i++)
	{
		_keys[i] = false;
	}
	_xChange = 0;
	_yChange = 0;
	_lastX = 0;
	_lastY = 0;

	glfwSetWindowUserPointer(attachedWindow, this);
	RegisterCallbacks(attachedWindow);
	glfwSetInputMode(attachedWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::RegisterCallbacks(GLFWwindow* attachedWindow)
{
	glfwSetKeyCallback(attachedWindow, HandleKeys);
	glfwSetCursorPosCallback(attachedWindow, HandleMouse);
}

bool Input::IsKeyValid(int key)
{
	return (key >= 0 && key < 1024);
}

void Input::OnKeyPressed(int key)
{
	if (IsKeyValid(key))
	{
		_keys[key] = true;
	}
}

void Input::OnKeyReleased(int key)
{
	if (IsKeyValid(key))
	{
		_keys[key] = false;
	}
}

bool Input::IsKeyPressed(int key)
{
	return IsKeyValid(key) && _keys[key];
}

GLfloat Input::GetXChange()
{
	GLfloat result = _xChange;
	_xChange = 0.0f;
	return result;
}

GLfloat Input::GetYChange()
{
	GLfloat result = _yChange;
	_yChange = 0.0f;
	return result;
}

void Input::HandleKeys(GLFWwindow* attachedWindow, int key, int code, int action, int mode)
{
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(attachedWindow));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(attachedWindow, GL_TRUE);
	}

	if (key >=0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
		{
			input->OnKeyPressed(key);
		}
		else if (action == GLFW_RELEASE)
		{
			input->OnKeyReleased(key);
		}
	}
}

void Input::HandleMouse(GLFWwindow* attachedWindow, double xPos, double yPos)
{
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(attachedWindow));
	if (input->_mouseFirstMouvement)
	{
		input->_lastX = xPos;
		input->_lastY = yPos;
		input->_mouseFirstMouvement = false;
	}
	input->_xChange = xPos - input->_lastX;
	input->_yChange = yPos - input->_lastY;
	input->_lastX = xPos;
	input->_lastY = yPos;
}