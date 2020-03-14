#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	_cutOffAngle = 15.0f;
	_cutOffCos = cosf(glm::radians(_cutOffAngle));
}

SpotLight::SpotLight(glm::vec3 color,
					 GLfloat ambiantIntensity,
					 GLfloat diffuseIntensity,
					 glm::vec3 position,
					 glm::vec3 direction,
					 GLfloat cutOffAngle,
					 LightAttenuationModel lightModel,
					 GLuint shadowWidth,
					 GLuint shadowHeight,
					 GLfloat near,
					 GLfloat far) : PointLight(color, ambiantIntensity, diffuseIntensity, position,
											   lightModel, shadowWidth, shadowHeight, near, far)
{
	_direction = glm::normalize(direction);
	_cutOffAngle = cutOffAngle;
	_cutOffCos = cosf(glm::radians(cutOffAngle));
}

void SpotLight::UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation, GLuint cutOffAngleLocation)
{
	glUniform3fv(ambiantColorLocation, 1, glm::value_ptr(_color));
	glUniform1f(ambiantIntensityLocation, _ambiantIntensity);
	glUniform1f(diffuseIntensityLocation, _diffuseIntensity);
	glUniform3fv(positionLocation, 1, glm::value_ptr(_position));
	glUniform1f(constantLocation, _lightModel.constant());
	glUniform1f(linearLocation, _lightModel.linear());
	glUniform1f(quadraticLocation, _lightModel.quadratic());
	glUniform3fv(directionLocation, 1, glm::value_ptr(_direction));
	glUniform1f(cutOffAngleLocation, _cutOffCos);
}

void SpotLight::SetAt(glm::vec3 position, glm::vec3 direction)
{
	SetPosition(position);
	SetDirection(direction);
}


void SpotLight::SetPosition(glm::vec3 position)
{
	_position = position;
}

void SpotLight::SetDirection(glm::vec3 direction)
{
	_direction = direction;
}

SpotLight::~SpotLight()
{
}