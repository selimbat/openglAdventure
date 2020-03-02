#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	_FBO = 0;
	_shadowMapTextureId = 0;
	_width = 0;
	_height = 0;
}

bool ShadowMap::Init(GLuint width, GLuint height)
{
	_width = width;
	_height = height;
	glGenFramebuffers(1, &_FBO);
	glGenTextures(1, &_shadowMapTextureId);
	glBindTexture(GL_TEXTURE_2D, _shadowMapTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMapTextureId, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %i\n", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _shadowMapTextureId);
}

GLuint ShadowMap::GetShadowWidth()
{
	return _width;
}

GLuint ShadowMap::GetShadowHeight()
{
	return _height;
}

ShadowMap::~ShadowMap()
{
	if (_FBO)
	{
		glDeleteFramebuffers(1, &_FBO);
	}
	if (_shadowMapTextureId)
	{
		glDeleteTextures(1, &_shadowMapTextureId);
	}
}
