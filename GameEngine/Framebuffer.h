#pragma once
#include <GL/glew.h>
class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();
	void Bind();
	static void BindDefault();
	GLuint PrepareDepthMap(unsigned int width, unsigned int height);
private:
	GLuint _fbo;
};

