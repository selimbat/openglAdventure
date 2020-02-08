#pragma once
#include "Light.h"
class DirectionalLight : 
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(glm::vec3 color, GLfloat ambiantIntencity,
				     GLfloat diffuseIntencity, glm::vec3 direction);

	void UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation,
				  GLuint diffuseIntensityLocation, GLuint directionLocation);
	
	~DirectionalLight();
private:
	glm::vec3 _direction;
};

