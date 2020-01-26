#include "Shader.h"

Shader::Shader()
{
	_shaderId = 0;
	_uniformModel = 0;
	_uniformProjection = 0;
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

	_uniformModel = glGetUniformLocation(_shaderId, "model");
	_uniformProjection = glGetUniformLocation(_shaderId, "projection");

}

GLuint Shader::GetProjectionLocation()
{
	return _uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return _uniformModel;
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