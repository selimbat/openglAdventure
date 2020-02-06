#include "Light.h"

Light::Light()
{
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_ambiantIntencity = 1.0f;

	_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	_diffuseIntencity = 0.0f;
}

Light::Light(glm::vec3 color, GLfloat ambiantIntencity,
			 glm::vec3 direction, GLfloat diffuseIntencity)
{
	_color = color;
	_ambiantIntencity = ambiantIntencity;
	_direction = direction;
	_diffuseIntencity = diffuseIntencity;
}

void Light::UseLight(GLuint ambiantIntencityLocation, GLuint ambiantColorLocation,
					 GLuint diffuseIntencityLocation, GLuint directionLocation)
{
	glUniform3f(ambiantColorLocation, _color.x, _color.y, _color.z);
	glUniform1f(ambiantIntencityLocation, _ambiantIntencity);
	glUniform3f(directionLocation, _direction.x, _direction.y, _direction.z);
	glUniform1f(diffuseIntencityLocation, _diffuseIntencity);
}

Light::~Light()
{

}
