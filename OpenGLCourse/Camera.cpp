#include "Camera.h"
#include <glm\gtx\string_cast.hpp>

Camera::Camera(glm::vec3 initialPos,
			   glm::vec3 worldUp,
			   GLfloat initialYaw,
			   GLfloat initialPitch,
			   GLfloat initialMoveSpeed,
			   GLfloat initialTurnSpeed,
			   Input* input)
{
	_input = input;
	_position = initialPos;
	_worldUp = worldUp;
	_yaw = initialYaw;
	_pitch = initialPitch;
	_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	
	_moveSpeed = initialMoveSpeed;
	_turnSpeed = initialTurnSpeed;

	Update(0.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(_position, _position + _forward, _up);
}

glm::vec3 Camera::GetPosition()
{
	return _position;
}

glm::vec3 Camera::GetDirection()
{
	return _forward;
}

glm::vec3 Camera::GetRight()
{
	return _right;
}

GLfloat Camera::GetYaw()
{
	return _yaw;
}

GLfloat Camera::GetPitch()
{
	return _pitch;
}

void Camera::Update(GLfloat deltatime)
{
	UpdateRotationAngles(deltatime);
	UpdateRotation();

	UpdatePositionMovement(deltatime);
}

void Camera::UpdateRotation()
{
	// no need to normalize forward
	_forward.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_forward.y = sin(glm::radians(_pitch));
	_forward.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_right = glm::normalize(glm::cross(_forward, _worldUp));
	_up = glm::normalize(glm::cross(_right, _forward));
}

void Camera::CircleAround(glm::vec3 position)
{
	GLfloat time = glfwGetTime();
	GLfloat angle = 1.0f * time;
	_position = position + 3.0f * glm::vec3(cos(angle), 0.5f, sin(angle));
	_pitch = -30.0f;
	_yaw = glm::degrees(angle) + 180.0f;
	UpdateRotation();
}

void Camera::UpdatePositionMovement(GLfloat deltatime)
{
	glm::vec3 mvtDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	if (_input->IsForwardsPressed())
	{
		mvtDirection.z += 1.0f;
	}
	if (_input->IsBackwardsPressed())
	{
		mvtDirection.z -= 1.0f;
	}
	if (_input->IsRightPressed())
	{
		mvtDirection.x += 1.0f;
	}
	if (_input->IsLeftPressed())
	{
		mvtDirection.x -= 1.0f;
	}
	if (_input->IsUpPressed())
	{
		mvtDirection.y += 1.0f;
	}
	if (_input->IsDownPressed())
	{
		mvtDirection.y -= 1.0f;
	}
	float lengthOfDirection = glm::length(mvtDirection);
	if (lengthOfDirection > 0)
	{
		mvtDirection = mvtDirection / lengthOfDirection;
	}

	_position += deltatime * _moveSpeed * (mvtDirection.x * _right + mvtDirection.y * _up + mvtDirection.z * _forward);
}

void Camera::UpdateRotationAngles(GLfloat deltatime)
{
	GLfloat deltaX = _input->GetXChange() * _turnSpeed;
	GLfloat deltaY = _input->GetYChange() * _turnSpeed;
	if (_input->IsTurnRightPressed())
	{
		_yaw += 500.0f * _turnSpeed * deltatime;
	}
	if (_input->IsTurnLeftPressed())
	{
		_yaw -= 500.0f * _turnSpeed * deltatime;
	}
	_yaw += deltaX;
	_pitch -= deltaY; // minus sign for the regular (not inverted) control

	_pitch = glm::max(glm::min(_pitch, 89.0f), -89.0f);
}
