#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat ambiantIntensity, GLfloat diffuseIntensity);

	~Light();
protected:
	glm::vec3 _color;
	GLfloat _ambiantIntensity;
	GLfloat _diffuseIntensity;
};

