#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	_FBOId = 0;
	_shadowMapTextureId = 0;
	_width = 0;
	_height = 0;
}

bool ShadowMap::Init(GLuint width, GLuint height)
{
	_width = width;
	_height = height;
	glGenFramebuffers(1, &_FBOId);
	glGenTextures(1, &_shadowMapTextureId);
	glBindTexture(GL_TEXTURE_2D, _shadowMapTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, _FBOId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMapTextureId, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	bool framebufferStatus = IsFrameBufferComplete();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return framebufferStatus;
}

bool ShadowMap::IsFrameBufferComplete()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %i\n", status);
		return false;
	}

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBOId);
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
	if (_FBOId)
	{
		glDeleteFramebuffers(1, &_FBOId);
	}
	if (_shadowMapTextureId)
	{
		glDeleteTextures(1, &_shadowMapTextureId);
	}
}
