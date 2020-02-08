#include "DirectionalLight.h"
#include <glm\gtc\type_ptr.hpp>

DirectionalLight::DirectionalLight() : Light()
{
	_direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(glm::vec3 color,
								   GLfloat ambiantIntensity,
								   GLfloat diffuseIntensity,
								   glm::vec3 direction) : Light(color, ambiantIntensity, diffuseIntensity)
{
	_direction = direction;
}

void DirectionalLight::UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3fv(ambiantColorLocation, 1, glm::value_ptr(_color));
	glUniform1f(ambiantIntensityLocation, _ambiantIntensity);
	glUniform3fv(directionLocation, 1, glm::value_ptr(_direction));
	glUniform1f(diffuseIntensityLocation, _diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
}
