#pragma once
#include "Light.h"
#include "LightAttenuationModel.h"
class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(glm::vec3 color, GLfloat ambiantIntensity, GLfloat diffuseIntensity, glm::vec3 position, LightAttenuationModel lightModel);
	void UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation,
				  GLuint diffuseIntensityLocation, GLuint positionLocation,
				  GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation);
	~PointLight();
private:
	glm::vec3 _position;
	LightAttenuationModel _lightModel;
};

