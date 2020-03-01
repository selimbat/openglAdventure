#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);

	bool LoadTexture(bool shouldLoadAlpha = false);
	void UseTexture();
	void ClearTexture();
	~Texture();
private:
	GLuint _textureId;
	int _width, _height, _bitDepth;
	
	const char* _fileLocation;
};

