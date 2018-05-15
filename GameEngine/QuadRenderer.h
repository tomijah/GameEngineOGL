#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "Utils.h"
#include "ShaderBase.h"

class QuadRenderer
{
public:
	QuadRenderer();
	~QuadRenderer();
	void Render(ShaderBase *shader, glm::mat4 transform, GLint texture);
private:
	GLuint VAO, VBO;
};

