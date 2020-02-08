#include "Material.h"

Material::Material()
{
	_specularIntensity = 0;
	_shininess = 0;
}

Material::Material(GLfloat specularIntensity, GLfloat shininess)
{
	_specularIntensity = specularIntensity;
	_shininess = shininess;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, _specularIntensity);
	glUniform1f(shininessLocation, _shininess);
}

Material::~Material()
{

}
