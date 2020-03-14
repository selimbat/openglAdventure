#include "Light.h"

Light::Light()
{
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_ambiantIntensity = 1.0f;
	_diffuseIntensity = 0.0f;
}

Light::Light(glm::vec3 color, GLfloat ambiantIntensity, GLfloat diffuseIntensity)
{
	_color = color;
	_ambiantIntensity = ambiantIntensity;
	_diffuseIntensity = diffuseIntensity;
}


Light::~Light()
{

}
