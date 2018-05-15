#pragma once
#include <iostream>
#include <map>
#include <GL/glew.h>
#include <SOIL.h>
#include <vector>

struct Texture {
	int Width;
	int Height;
	GLuint Id;
	void bind() {
		glBindTexture(GL_TEXTURE_2D, Id);
	}
};

class TextureCache
{
public:
	static Texture getTexture(std::string path);
	static GLuint loadCubeMap(std::vector<std::string> faces);
	static void releaseAll();
private:
	static Texture Generate(const char* fileName, bool linear = false, bool generateMipmap = true);
	static std::map<std::string, Texture> _textureMap;
};

