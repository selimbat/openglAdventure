#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	_direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(glm::vec3 color,
								   GLfloat ambiantIntensity,
								   GLfloat diffuseIntensity,
								   glm::vec3 direction,
								   GLfloat shadowWidth,
								   GLfloat shadowHeight) : Light(color,
																 ambiantIntensity,
																 diffuseIntensity,
																 shadowWidth,
																 shadowHeight)
{
	_direction = glm::normalize(direction);
	_lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

void DirectionalLight::UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3fv(ambiantColorLocation, 1, glm::value_ptr(_color));
	glUniform1f(ambiantIntensityLocation, _ambiantIntensity);
	glUniform3fv(directionLocation, 1, glm::value_ptr(_direction));
	glUniform1f(diffuseIntensityLocation, _diffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return _lightProj * glm::lookAt(-_direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
}
