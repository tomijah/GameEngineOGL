#include "TextureCache.h"

std::map<std::string, Texture> TextureCache::_textureMap;

Texture TextureCache::getTexture(std::string path)
{
	if (_textureMap.find(path) == _textureMap.end()) {
		Texture generated = TextureCache::Generate(path.c_str(), true, true);
		_textureMap[path] = generated;
	}

	return _textureMap[path];
}

void TextureCache::releaseAll()
{
	for (auto it = _textureMap.begin(); it != _textureMap.end(); ++it)
	{
		glDeleteTextures(1, &it->second.Id);
	}

	_textureMap.clear();
}

Texture TextureCache::Generate(const char * fileName, bool linear, bool generateMipmap)
{
	GLuint textureID;
	int width, height;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (linear) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	unsigned char* image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	std::cout << fileName << " "<< width << " " << height << std::endl;
	if (generateMipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	Texture result;
	result.Width = width;
	result.Height = height;
	result.Id = textureID;
	return result;
}
