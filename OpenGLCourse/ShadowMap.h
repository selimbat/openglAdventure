#pragma once
#include <stdio.h>
#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();
	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	GLuint GetShadowWidth();
	GLuint GetShadowHeight();
	~ShadowMap();
protected:
	GLuint _FBOId;
	GLuint _shadowMapTextureId;
	GLuint _width, _height;
	bool IsFrameBufferComplete();
};

