#pragma once
#include <GL\glew.h>
struct LightAttenuationModel
{
public:
	LightAttenuationModel();
	LightAttenuationModel(GLfloat constant, GLfloat linear, GLfloat quadratic);
	GLfloat constant() { return _constant; }
	GLfloat linear() { return _linear; }
	GLfloat quadratic() { return _quadratic; }
	~LightAttenuationModel();
private:
	GLfloat _constant, _linear, _quadratic;
};

