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
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

	void Draw(ShaderBase *shader, bool simpl = false)
	{
		glm::mat4 model = this->getMatrix();
		shader->SetMatrix4("model", model);
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader, simpl);
	}

	void CleanUp() {
		for (auto m = meshes.begin(); m != meshes.end(); m++) {
			m->RemoveBuffers();
		}
	}

	glm::mat4 getMatrix();

	glm::vec3 Position = glm::vec3(0);
	glm::quat Rotation = glm::quat();
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
