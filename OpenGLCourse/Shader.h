#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint _shaderId, _uniformProjection, _uniformModel;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint programId, const char* shaderCode, GLenum shaderType);
};

