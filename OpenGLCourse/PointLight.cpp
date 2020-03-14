#include "PointLight.h"

PointLight::PointLight() : Light()
{
	_position = glm::vec3(0.0f);
	_lightModel = LightAttenuationModel();
	_farPlane = 100.0f;
}

PointLight::PointLight(glm::vec3 color,
					   GLfloat ambiantIntensity,
					   GLfloat diffuseIntensity,
					   glm::vec3 position,
					   LightAttenuationModel lightModel,
					   GLuint shadowWidth,
					   GLuint shadowHeight,
					   GLfloat near,
					   GLfloat far) : Light(color, ambiantIntensity, diffuseIntensity)
{
	_position = position;
	_lightModel = lightModel;
	_farPlane = far;

	GLfloat aspectRatio = shadowWidth / shadowHeight;
	_lightProj = glm::perspective((GLfloat)glm::radians(90.0), aspectRatio, near, _farPlane);
	_shadowMap = new OmniShadowMap();
	_shadowMap->Init(shadowWidth, shadowHeight);
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

ShadowMap* PointLight::GetShadowMap()
{
	return _shadowMap;
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	// positive x
	lightMatrices.push_back(_lightProj * glm::lookAt(_position,
													 _position + glm::vec3(1.0f, 0.0f, 0.0f),
													 glm::vec3(0.0f, -1.0f, 0.0f)));
	// negative x
	lightMatrices.push_back(_lightProj * glm::lookAt(_position,
													 _position + glm::vec3(-1.0f, 0.0f, 0.0f),
													 glm::vec3(0.0f, -1.0f, 0.0f)));
	// positive y
	lightMatrices.push_back(_lightProj * glm::lookAt(_position,
													 _position + glm::vec3(0.0f, 1.0f, 0.0f),
													 glm::vec3(0.0f, 0.0f, 1.0f)));
	// negative y
	lightMatrices.push_back(_lightProj * glm::lookAt(_position,
													 _position + glm::vec3(0.0f, -1.0f, 0.0f),
													 glm::vec3(0.0f, 0.0f, -1.0f)));
	// positive z
	lightMatrices.push_back(_lightProj * glm::lookAt(_position,
													 _position + glm::vec3(0.0f, 0.0f, 1.0f),
													 glm::vec3(0.0f, -1.0f, 0.0f)));
	// negative z
	lightMatrices.push_back(_lightProj * glm::lookAt(_position,
													 _position + glm::vec3(0.0f, 0.0f, -1.0f),
													 glm::vec3(0.0f, -1.0f, 0.0f)));
	return lightMatrices;
}

glm::vec3 PointLight::GetPosition()
{
	return _position;
}

GLfloat PointLight::GetFarPlane()
{
	return _farPlane;
}

PointLight::~PointLight()
{
}
