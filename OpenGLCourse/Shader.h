#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation = nullptr);

	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetEyePositionLocation();
	GLuint GetAmbiantIntensityLocation();
	GLuint GetAmbiantColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetLightDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetPointLightPositionLocation();
	GLuint GetFarPlaneLocation();

	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLights(PointLight* pointLights, unsigned int lightCount, unsigned int textureUnit);
	void SetSpotLights(SpotLight* spotLights, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int _pointLightCount;
	int _spotLightCount;

	GLuint _shaderId,
		   _uniformProjection,
		   _uniformModel,
		   _uniformView,
		   _uniformEyePosition,
		   _uniformSpecularIntensity,
		   _uniformShininess,
		   _uniformTexture,
		   _uniformDirectionalLightTranform,
		   _uniformDirectionalShadowMap,
		   _uniformOmniLightPos,
		   _uniformFarPlane;

	GLuint _uniformLightMatrices[6];

	struct
	{
		GLuint UniformColor;
		GLuint UniformAmbiantIntensity;
		GLuint UniformDiffuseIntensity;
		GLuint UniformDirection;
	} _uniformDirectionalLight;

	GLuint _uniformPointLightCount;

	struct
	{
		GLuint UniformColor;
		GLuint UniformAmbiantIntensity;
		GLuint UniformDiffuseIntensity;
		GLuint UniformPosition;
		GLuint UniformConstant;
		GLuint UniformLinear;
		GLuint UniformQuadratic;
	} _uniformPointLights[MAX_POINT_LIGHTS];

	GLuint _uniformSpotLightCount;

	struct
	{
		GLuint UniformColor;
		GLuint UniformAmbiantIntensity;
		GLuint UniformDiffuseIntensity;
		GLuint UniformPosition;
		GLuint UniformConstant;
		GLuint UniformLinear;
		GLuint UniformQuadratic;
		GLuint UniformDirection;
		GLuint UniformCutOffCos;
	} _uniformSpotLights[MAX_SPOT_LIGHTS];

	struct
	{
		GLuint shadowMap;
		GLfloat farPlane;
	} _uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr);
	void CompileProgram(bool& retflag);
	void AddShader(GLuint programId, const char* shaderCode, GLenum shaderType);
};

