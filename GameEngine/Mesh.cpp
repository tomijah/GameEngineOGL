#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material)
{
	this->vertices = vertices;
	this->indices = indices;
	this->material = material;
	this->setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(ShaderBase shader)
{
	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), this->material.shininess);
	glUniform3f(glGetUniformLocation(shader.Program, "material.color"), this->material.color.r, this->material.color.g, this->material.color.b);
	glUniform1i(glGetUniformLocation(shader.Program, "material.hasTexture"), this->material.hasTexture);
	if (this->material.diffuseTextureId > 0) {
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->material.diffuseTextureId);
	}

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::RemoveBuffers()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}
