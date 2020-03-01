#include "Texture.h"

Texture::Texture()
{
	_textureId = 0;
	_width = 0;
	_height = 0;
	_bitDepth = 0;
	_fileLocation = new char;
}

Texture::Texture(const char* fileLocation)
{
	_textureId = 0;
	_width = 0;
	_height = 0;
	_bitDepth = 0;
	_fileLocation = fileLocation;
}

bool Texture::LoadTexture(bool shouldLoadAlpha)
{
	unsigned char* textureData = stbi_load(_fileLocation, &_width, &_height, &_bitDepth, 0);
	if (!textureData)
	{
		printf("Failed to find %s\n", _fileLocation);
		return false;
	}

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (shouldLoadAlpha) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &_textureId);
	_textureId = 0;
	_width = 0;
	_height = 0;
	_bitDepth = 0;
	_fileLocation = new char;
}

Texture::~Texture()
{
	ClearTexture();
}