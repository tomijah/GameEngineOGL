#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "Utils.h"
#include "ShaderBase.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox(std::vector<std::string> faces);
	~Skybox();
	void Draw(ShaderBase *shader, Camera *camera);
private:
	GLuint _textureId;
	GLuint _vao;
	GLuint _vbo;
};

