#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambiantIntencity);
	void UseLight(GLfloat ambiantIntencityLocation, GLfloat ambiantColorLocation);
	~Light();
private:
	glm::vec3 _color;
	GLfloat _ambiantIntencity;

};

