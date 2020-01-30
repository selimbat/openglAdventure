#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLocation);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();
	~Texture();
private:
	GLuint _textureId;
	int _width, _height, _bitDepth;
	
	char* _fileLocation;
};

