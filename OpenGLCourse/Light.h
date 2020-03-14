#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat ambiantIntensity, GLfloat diffuseIntensity);

	virtual ShadowMap* GetShadowMap() = 0;

	~Light();
protected:
	glm::vec3 _color;
	GLfloat _ambiantIntensity;
	GLfloat _diffuseIntensity;

	glm::mat4 _lightProj;
};

