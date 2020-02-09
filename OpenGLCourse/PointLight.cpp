#include "PointLight.h"

PointLight::PointLight() : Light()
{
	_position = glm::vec3(0.0f);
	_lightModel = LightAttenuationModel();
}

PointLight::PointLight(glm::vec3 color,
					   GLfloat ambiantIntensity,
					   GLfloat diffuseIntensity,
					   glm::vec3 position,
					   LightAttenuationModel lightModel) : Light(color, ambiantIntensity, diffuseIntensity)
{
	_position = position;
	_lightModel = lightModel;
}

void PointLight::UseLight(GLuint ambiantIntensityLocation,
						  GLuint ambiantColorLocation,
						  GLuint diffuseIntensityLocation,
						  GLuint positionLocation,
						  GLuint constantLocation,
						  GLuint linearLocation,
						  GLuint quadraticLocation)
{
	glUniform3fv(ambiantColorLocation, 1, glm::value_ptr(_color));
	glUniform1f(ambiantIntensityLocation, _ambiantIntensity);
	glUniform1f(diffuseIntensityLocation, _diffuseIntensity);
	glUniform3fv(positionLocation, 1, glm::value_ptr(_position));
	glUniform1f(constantLocation, _lightModel.constant());
	glUniform1f(linearLocation, _lightModel.linear());
	glUniform1f(quadraticLocation, _lightModel.quadratic());
}

PointLight::~PointLight()
{
}
