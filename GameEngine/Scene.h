#pragma once
#include "Utils.h"
#include "Model.h"
#include "ShaderBase.h"

struct DirLight {
	glm::vec3 direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	glm::vec3 ambient = glm::vec3(0);
	glm::vec3 diffuse = glm::vec3(1);
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
	void Draw(ShaderBase shader);
	DirLight directionLight;
	bool updateLights = true;
private:
	void setLights(ShaderBase shader);
};

