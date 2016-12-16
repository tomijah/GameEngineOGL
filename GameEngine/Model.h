#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL.h>
#include "Mesh.h"
#include "TextureCache.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
public:
	Model(GLchar* path)
	{
		this->loadModel(path);
	}

	Model(std::vector<Mesh> meshes) {
		this->meshes = meshes;
	}

	void Draw(ShaderBase shader)
	{
		GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
		glm::mat4 model = this->getMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);
	}

	void CleanUp() {
		for (auto m = meshes.begin(); m != meshes.end(); m++) {
			m->RemoveBuffers();
		}
	}

	glm::mat4 getMatrix();

	glm::vec3 Position = glm::vec3(0);
	glm::vec3 Rotation = glm::vec3(0);
	glm::vec3 Scale = glm::vec3(1);
	bool recalculateMatrix = true;
	std::vector<Mesh> meshes;
private:
	
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	glm::mat4 modelMatrix;
	std::string directory;
};
