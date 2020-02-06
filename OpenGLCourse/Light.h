#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat ambiantIntencity,
		  glm::vec3 direction, GLfloat diffuseIntencity);

	void UseLight(GLuint ambiantIntencityLocation, GLuint ambiantColorLocation, 
				  GLuint diffuseIntencityLocation, GLuint directionLocation);
	~Light();
private:
	glm::vec3 _color;
	GLfloat _ambiantIntencity;

	glm::vec3 _direction;
	GLfloat _diffuseIntencity;
};

