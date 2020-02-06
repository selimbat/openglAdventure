#include "Material.h"

Material::Material()
{
	_specularIntencity = 0;
	_shininess = 0;
}

Material::Material(GLfloat specularIntencity, GLfloat shininess)
{
	_specularIntencity = specularIntencity;
	_shininess = shininess;
}

void Material::UseMaterial(GLuint specularIntencityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntencityLocation, _specularIntencity);
	glUniform1f(shininessLocation, _shininess);
}

Material::~Material()
{

}
