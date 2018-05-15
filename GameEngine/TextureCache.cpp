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

GLuint TextureCache::loadCubeMap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i].c_str() << std::endl;
			SOIL_free_image_data(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Texture result;
	result.Width = width;
	result.Height = height;
	result.Id = textureID;

	return textureID;
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	unsigned char* image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	std::cout << fileName << " "<< width << " " << height << std::endl;
	if (generateMipmap || linear) {
		glGenerateMipmap(GL_TEXTURE_2D);
		GLfloat max;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
		std::cout << "Max ani: " << max << std::endl;
		glTexParameterf(textureID, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	Texture result;
	result.Width = width;
	result.Height = height;
	result.Id = textureID;
	return result;
}
