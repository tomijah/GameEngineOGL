#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "Utils.h"
#include "ShaderBase.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Material {
	glm::vec3 color = glm::vec3(1);
	GLfloat shininess = 0.0f;
	GLuint diffuseTextureId = 0;
	bool hasTexture = false;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material);
	~Mesh();
	void Draw(ShaderBase shader);
	void RemoveBuffers();
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material material;
	GLuint VAO, VBO, EBO;
	void setupMesh();
};

