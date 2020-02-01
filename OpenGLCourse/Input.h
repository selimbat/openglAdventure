#pragma once
#include <GLFW\glfw3.h>

class Input
{
public:
	Input() { }
	Input(GLFWwindow* attachedWindow);
	bool IsKeyValid(int key);
	void OnKeyPressed(int key);
	void OnKeyReleased(int key);
	bool IsKeyPressed(int key);
	GLfloat GetXChange();
	GLfloat GetYChange();
	bool IsLeftPressed() { return _keys[GLFW_KEY_Q]; }
	bool IsTurnLeftPressed() { return _keys[GLFW_KEY_A] || _keys[GLFW_KEY_LEFT]; }
	bool IsRightPressed() { return _keys[GLFW_KEY_E]; }
	bool IsTurnRightPressed() { return _keys[GLFW_KEY_D] || _keys[GLFW_KEY_RIGHT]; }
	bool IsForwardsPressed() { return _keys[GLFW_KEY_W] || _keys[GLFW_KEY_UP]; }
	bool IsBackwardsPressed() { return _keys[GLFW_KEY_S] || _keys[GLFW_KEY_DOWN]; }
	bool IsUpPressed() { return _keys[GLFW_KEY_SPACE]; }
	bool IsDownPressed() { return _keys[GLFW_KEY_X]; }
	~Input() { }
private:
	bool _keys[1024];
	GLfloat _lastX, _lastY, _xChange, _yChange;
	bool _mouseFirstMouvement;
	void RegisterCallbacks(GLFWwindow* attachedWindow);
	static void HandleKeys(GLFWwindow* attachedWindow, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* attachedWindow, double xPos, double yPos);
};