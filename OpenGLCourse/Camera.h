#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Input.h"

class Camera
{
public:
	Camera() { }
	Camera(glm::vec3 initialPos, glm::vec3 worldUp, GLfloat initialYaw, GLfloat initialPitch, GLfloat initialMoveSpeed, GLfloat initialTurnSpeed, Input* input);
	glm::mat4 GetViewMatrix();
	void Update(GLfloat deltatime);
	~Camera() { }

private:
	Input* _input;

	glm::vec3 _position;
	glm::vec3 _forward;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	GLfloat _yaw;
	GLfloat _pitch;

	GLfloat _moveSpeed;
	GLfloat _turnSpeed;


	void UpdatePositionMovement(GLfloat deltatime);
	void UpdateRotationMovement(GLfloat deltatime);
};

