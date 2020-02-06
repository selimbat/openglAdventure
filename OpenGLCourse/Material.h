#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat specularIntencity, GLfloat shininess);
	void UseMaterial(GLuint specularIntencityLocation, GLuint shininessLocation);
	~Material();
private:
	GLfloat _specularIntencity;
	GLfloat _shininess;
};

