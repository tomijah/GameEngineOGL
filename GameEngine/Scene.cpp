#include "Scene.h"

void Scene::Draw(ShaderBase shader)
{
	if (updateLights) {
		setLights(shader);
		updateLights = false;
	}

	for (auto i = models.begin(); i != models.end(); i++) {
		(*i)->Draw(shader);
	}
}

void Scene::setLights(ShaderBase shader)
{
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), directionLight.direction.x, directionLight.direction.y, directionLight.direction.z);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), directionLight.ambient.r, directionLight.ambient.g, directionLight.ambient.b);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), directionLight.diffuse.r, directionLight.diffuse.g, directionLight.diffuse.b);
	glUniform1i(glGetUniformLocation(shader.Program, "dirLight.apply"), directionLight.apply);

	if (pointLights.size() == 0) {
		return;
	}

	for (GLuint i = 0; i < 16; i++) {
		std::string number = std::to_string(i);
		if (i > pointLights.size() - 1) {
			glUniform1i(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].apply").c_str()), false);
			continue;
		}
				
		PointLight light = pointLights[i];
		glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].position").c_str()), light.position.x, light.position.y, light.position.z);
		glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].ambient").c_str()), light.ambient.r, light.ambient.g, light.ambient.b);
		glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].diffuse").c_str()), light.diffuse.r, light.diffuse.g, light.diffuse.b);
		glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].constant").c_str()), light.constant);
		glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].linear").c_str()), light.linear);
		glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].quadratic").c_str()), light.quadratic);
		glUniform1i(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].apply").c_str()), light.apply);
	}
}
