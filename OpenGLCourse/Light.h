#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat ambiantIntensity, GLfloat diffuseIntensity, GLfloat shadowWidth, GLfloat shadowHeight);

	ShadowMap* GetShadowMap();

	~Light();
protected:
	glm::vec3 _color;
	GLfloat _ambiantIntensity;
	GLfloat _diffuseIntensity;

	glm::mat4 _lightProj;

	ShadowMap* _shadowMap;
};

