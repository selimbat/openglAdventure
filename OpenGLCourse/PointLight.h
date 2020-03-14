#pragma once
#include <glm\gtc\type_ptr.hpp>
#include <vector>

#include "Light.h"
#include "LightAttenuationModel.h"
#include "OmniShadowMap.h"

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(glm::vec3 color, GLfloat ambiantIntensity, GLfloat diffuseIntensity,
			   glm::vec3 position, LightAttenuationModel lightModel,
			   GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far);
	void UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation,
				  GLuint diffuseIntensityLocation, GLuint positionLocation,
				  GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation);
	virtual ShadowMap* GetShadowMap();
	std::vector<glm::mat4> CalculateLightTransform();
	
	glm::vec3 GetPosition();
	GLfloat GetFarPlane();

	~PointLight();
protected:
	glm::vec3 _position;
	LightAttenuationModel _lightModel;
	GLfloat _farPlane;
	OmniShadowMap* _shadowMap;
};

