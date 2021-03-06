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
	GLuint specularMapTextureId = 0;
	bool hasTexture = false;
	bool hasSpecularMap = false;
	bool applyLights = true;
	bool reflect = false;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material);
	~Mesh();
	void Draw(ShaderBase *shader, bool simpl = false);
	void RemoveBuffers();
	void UpdateGeometry(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	Material material;
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;
	void setupMesh();
};