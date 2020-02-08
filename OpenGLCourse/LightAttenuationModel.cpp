#include "LightAttenuationModel.h"

LightAttenuationModel::LightAttenuationModel()
{
	_constant = 1.0f;
	_linear = 0.0f;
	_quadratic = 1.0f;
}

LightAttenuationModel::LightAttenuationModel(GLfloat constant, GLfloat linear, GLfloat quadratic)
{
	_constant = constant;
	_linear = linear;
	_quadratic = quadratic;
}

LightAttenuationModel::~LightAttenuationModel()
{
}
