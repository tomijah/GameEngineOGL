#include "Model.h"
using namespace glm;

glm::mat4 Model::getMatrix()
{
	if (recalculateMatrix) {
		glm::mat4 trans;
		trans = glm::translate(trans, this->Position);
		glm::mat4 scale;
		scale = glm::scale(scale, this->Scale);
		this->modelMatrix = trans * mat4_cast(this->Rotation) * scale;
		this->recalculateMatrix = false;
	}

	return modelMatrix;
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material material;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}

		vertices.push_back(vertex);
	}
	
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		for (GLuint j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		float shininessStrength = 0;
		mat->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
		std::cout << shininessStrength << std::endl;
		material.color = glm::vec3(color.r, color.g, color.b);
		material.shininess = shininessStrength;
		
		material.hasTexture = false;

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString texturePath;
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
			std::string filename = texturePath.C_Str();
			filename = directory + '/' + filename;
			Texture tex = TextureCache::getTexture(filename);
			material.diffuseTextureId = tex.Id;
			material.hasTexture = true;
		}

		if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
			aiString texturePath;
			mat->GetTexture(aiTextureType_SPECULAR, 0, &texturePath);
			std::string filename = texturePath.C_Str();
			filename = directory + '/' + filename;
			Texture tex = TextureCache::getTexture(filename);
			material.specularMapTextureId = tex.Id;
			material.hasSpecularMap = true;
		}
		
		//std::cout << mat->GetTextureCount(aiTextureType_HEIGHT) << std::endl;
	}

	return Mesh(vertices, indices, material);
}