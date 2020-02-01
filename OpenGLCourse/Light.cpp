#include "Light.h"

Light::Light()
{
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_ambiantIntencity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambiantIntencity)
{
	_color = glm::vec3(red, green, blue);
	_ambiantIntencity = ambiantIntencity;
}

void Light::UseLight(GLfloat ambiantIntencityLocation, GLfloat ambiantColorLocation)
{
	glUniform3f(ambiantColorLocation, _color.x, _color.y, _color.z);
	glUniform1f(ambiantIntencityLocation, _ambiantIntencity);
}

Light::~Light()
{

}
