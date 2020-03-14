#pragma once
#include <glm\gtc\type_ptr.hpp>
#include "Light.h"
#include "ShadowMap.h"
class DirectionalLight : 
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(glm::vec3 color, GLfloat ambiantIntencity,
				     GLfloat diffuseIntencity, glm::vec3 direction,
					 GLfloat shadowWidth, GLfloat shadowHeight);

	virtual ShadowMap* GetShadowMap();

	void UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation,
				  GLuint diffuseIntensityLocation, GLuint directionLocation);
	
	glm::mat4 CalculateLightTransform();

	~DirectionalLight();
private:
	glm::vec3 _direction;
	ShadowMap* _shadowMap;
};

