#include "Shader.h"

Shader::Shader()
{
	_shaderId = 0;
	_uniformModel = 0;
	_uniformProjection = 0;
	_uniformView = 0;

	_pointLightCount = 0;
	_spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);
	
	if (!fileStream.is_open())
	{
		printf("ERROR: Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	_shaderId = glCreateProgram();

	if (!_shaderId)
	{
		printf("ERROR: Creating shader program failed!\n");
		return;
	}

	AddShader(_shaderId, vertexCode, GL_VERTEX_SHADER);
	AddShader(_shaderId, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(_shaderId);
	glGetProgramiv(_shaderId, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
		printf("ERROR: Linking shader program failed! '%s'\n", eLog);
		return;
	}

	glValidateProgram(_shaderId);
	glGetProgramiv(_shaderId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
		printf("ERROR: Validating program failed! '%s'\n", eLog);
		return;
	}

	_uniformProjection = glGetUniformLocation(_shaderId, "projection");
	_uniformModel = glGetUniformLocation(_shaderId, "model");
	_uniformView = glGetUniformLocation(_shaderId, "view");
	_uniformDirectionalLight.UniformColor = glGetUniformLocation(_shaderId, "directionalLight.base.color");
	_uniformDirectionalLight.UniformAmbiantIntensity = glGetUniformLocation(_shaderId, "directionalLight.base.ambiantIntensity");
	_uniformDirectionalLight.UniformDirection = glGetUniformLocation(_shaderId, "directionalLight.direction");
	_uniformDirectionalLight.UniformDiffuseIntensity = glGetUniformLocation(_shaderId, "directionalLight.base.diffuseIntensity");
	_uniformSpecularIntensity = glGetUniformLocation(_shaderId, "material.specularIntensity");
	_uniformShininess = glGetUniformLocation(_shaderId, "material.shininess");
	_uniformEyePosition = glGetUniformLocation(_shaderId, "eyePosition");

	_uniformPointLightCount = glGetUniformLocation(_shaderId, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		_uniformPointLights[i].UniformColor = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambiantIntensity", i);
		_uniformPointLights[i].UniformAmbiantIntensity = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		_uniformPointLights[i].UniformDiffuseIntensity = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		_uniformPointLights[i].UniformPosition = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		_uniformPointLights[i].UniformConstant = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		_uniformPointLights[i].UniformLinear = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].quadratic", i);
		_uniformPointLights[i].UniformQuadratic = glGetUniformLocation(_shaderId, locBuff);
	}

	_uniformSpotLightCount = glGetUniformLocation(_shaderId, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		_uniformSpotLights[i].UniformColor = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambiantIntensity", i);
		_uniformSpotLights[i].UniformAmbiantIntensity = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		_uniformSpotLights[i].UniformDiffuseIntensity = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		_uniformSpotLights[i].UniformPosition = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		_uniformSpotLights[i].UniformConstant = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		_uniformSpotLights[i].UniformLinear = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.quadratic", i);
		_uniformSpotLights[i].UniformQuadratic = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		_uniformSpotLights[i].UniformDirection = glGetUniformLocation(_shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].cutOffCos", i);
		_uniformSpotLights[i].UniformCutOffCos = glGetUniformLocation(_shaderId, locBuff);
	}

}

GLuint Shader::GetProjectionLocation()
{
	return _uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return _uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return _uniformView;
}
GLuint Shader::GetAmbiantColorLocation()
{
	return _uniformDirectionalLight.UniformColor;
}
GLuint Shader::GetAmbiantIntensityLocation()
{
	return _uniformDirectionalLight.UniformAmbiantIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return _uniformDirectionalLight.UniformDiffuseIntensity;
}

GLuint Shader::GetLightDirectionLocation()
{
	return _uniformDirectionalLight.UniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return _uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return _uniformShininess;
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight)
{
	directionalLight->UseLight(_uniformDirectionalLight.UniformAmbiantIntensity,
							   _uniformDirectionalLight.UniformColor,
							   _uniformDirectionalLight.UniformDiffuseIntensity,
							   _uniformDirectionalLight.UniformDirection);
}

void Shader::SetPointLights(PointLight* pointLights, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
	{
		lightCount = MAX_POINT_LIGHTS;
	}
	glUniform1i(_uniformPointLightCount, lightCount);

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		pointLights[i].UseLight(_uniformPointLights[i].UniformAmbiantIntensity,
								_uniformPointLights[i].UniformColor,
								_uniformPointLights[i].UniformDiffuseIntensity,
								_uniformPointLights[i].UniformPosition,
								_uniformPointLights[i].UniformConstant,
								_uniformPointLights[i].UniformLinear,
								_uniformPointLights[i].UniformQuadratic);
	}
}

void Shader::SetSpotLights(SpotLight* spotLights, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS)
	{
		lightCount = MAX_SPOT_LIGHTS;
	}
	glUniform1i(_uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		spotLights[i].UseLight(_uniformSpotLights[i].UniformAmbiantIntensity,
							   _uniformSpotLights[i].UniformColor,
							   _uniformSpotLights[i].UniformDiffuseIntensity,
							   _uniformSpotLights[i].UniformPosition,
							   _uniformSpotLights[i].UniformDirection,
							   _uniformSpotLights[i].UniformConstant,
							   _uniformSpotLights[i].UniformLinear,
							   _uniformSpotLights[i].UniformQuadratic,
							   _uniformSpotLights[i].UniformCutOffCos);
	}
}

GLuint Shader::GetEyePositionLocation()
{
	return _uniformEyePosition;
}

void Shader::UseShader()
{
	glUseProgram(_shaderId);
}

void Shader::ClearShader()
{
	if (_shaderId != 0)
	{
		glDeleteProgram(_shaderId);
		_shaderId = 0;
	}

	_uniformModel = 0;
	_uniformProjection = 0;
}

void Shader::AddShader(GLuint programId, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("ERROR: Compiling the %d shader failed! '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(programId, shader);
}

Shader::~Shader()
{
	ClearShader();
}