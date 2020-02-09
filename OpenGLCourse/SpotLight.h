#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec3 color,
			  GLfloat ambiantIntensity,
			  GLfloat diffuseIntensity,
			  glm::vec3 position,
			  glm::vec3 direction,
			  GLfloat cutOffAngle,
			  LightAttenuationModel lightModel);
	void UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation,
				  GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
				  GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation, GLuint cutOffAngleLocation);

	void SetAt(glm::vec3 position, glm::vec3 direction);
	~SpotLight();
private:
	glm::vec3 _direction;
	void SetPosition(glm::vec3 position);
	void SetDirection(glm::vec3 direction);
	GLfloat _cutOffAngle, _cutOffCos;
};

