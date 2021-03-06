#pragma once
#include "Utils.h"
#include "Model.h"
#include "ShaderBase.h"
#include "Camera.h"

struct DirLight {
	glm::vec3 direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	glm::vec3 ambient = glm::vec3(0);
	glm::vec3 diffuse = glm::vec3(1);
	glm::mat4 getViewMatrix() {
		glm::vec3 lightPosition = glm::vec3(12.0f, 12.0f, 12.0f); //glm::vec3(0) - direction * 5.0f;
		return glm::lookAt(lightPosition,
			glm::vec3(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));;
	}
	bool apply;
};

struct PointLight {
	glm::vec3 position = glm::vec3(0);

	GLfloat constant = 1.0f;
	GLfloat linear = 0.09f;
	GLfloat quadratic = 0.032f;

	glm::vec3 ambient = glm::vec3(0);
	glm::vec3 diffuse = glm::vec3(1);
	bool apply;
};

class Scene
{
public:
	void CleanUp() {
		for (auto m = models.begin(); m != models.end(); m++) {
			(*m)->CleanUp();
		}
	}
	std::vector<Model*> models;
	std::vector<PointLight> pointLights;
	void Draw(ShaderBase *shader, Camera *camera, bool simpl = false);
	void DrawShadowMap(ShaderBase *shader, DirLight *light);
	DirLight directionLight;
	bool updateLights = true;
private:
	void setLights(ShaderBase *shader);
};

