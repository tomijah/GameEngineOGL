#include "Scene.h"

void Scene::Draw(ShaderBase *shader, Camera *camera, bool simpl)
{
	//shader->Use();
	shader->SetMatrix4("view", camera->getViewMatrix());
	shader->SetMatrix4("projection", camera->Projection);
	shader->SetVector3f("viewPos", glm::vec3(camera->Position.x, camera->Position.y, camera->Position.z));
	/*float near_plane = 0.5f, far_plane = 140.0f;
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
	shader->SetMatrix4("view", directionLight.getViewMatrix());
	shader->SetMatrix4("projection", lightProjection);
	shader->SetVector3f("viewPos", glm::vec3(50.0f, 50.0f, 50.0f));*/
	if (updateLights) {
		setLights(shader);
		updateLights = false;
	}

	for (auto i = models.begin(); i != models.end(); i++) {
		(*i)->Draw(shader, simpl);
	}
}

void Scene::DrawShadowMap(ShaderBase * shader, DirLight * light)
{
	//shader->Use();
	float near_plane = 0.1f, far_plane = 150.0f;
	glm::mat4 lightProjection = glm::ortho(-25.0f, 25.0f, -15.0f, 15.0f, near_plane, far_plane);
	glm::mat4 lightView = light->getViewMatrix();

	shader->SetMatrix4("view", lightView);
	shader->SetMatrix4("projection", lightProjection);

	for (auto i = models.begin(); i != models.end(); i++) {
		(*i)->Draw(shader);
	}
}

void Scene::setLights(ShaderBase *shader)
{
	glUniform3f(glGetUniformLocation(shader->Program, "dirLight.direction"), directionLight.direction.x, directionLight.direction.y, directionLight.direction.z);
	glUniform3f(glGetUniformLocation(shader->Program, "dirLight.ambient"), directionLight.ambient.r, directionLight.ambient.g, directionLight.ambient.b);
	glUniform3f(glGetUniformLocation(shader->Program, "dirLight.diffuse"), directionLight.diffuse.r, directionLight.diffuse.g, directionLight.diffuse.b);
	glUniform1i(glGetUniformLocation(shader->Program, "dirLight.apply"), directionLight.apply);
	shader->SetMatrix4("lightView", directionLight.getViewMatrix());
	float near_plane = 0.1f, far_plane = 150.0f;
	glm::mat4 lightProjection = glm::ortho(-25.0f, 25.0f, -15.0f, 15.0f, near_plane, far_plane);
	glm::vec3 lightPosition = glm::vec3(12.0f,12.0f, 12.0f);
	shader->SetMatrix4("lightProjection", lightProjection);
	shader->SetVector3f("lightPos", lightPosition);

	if (pointLights.size() == 0) {
		return;
	}

	for (GLuint i = 0; i < 16; i++) {
		std::string number = std::to_string(i);
		if (i > pointLights.size() - 1) {
			glUniform1i(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].apply").c_str()), false);
			continue;
		}
				
		PointLight light = pointLights[i];
		glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].position").c_str()), light.position.x, light.position.y, light.position.z);
		glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].ambient").c_str()), light.ambient.r, light.ambient.g, light.ambient.b);
		glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].diffuse").c_str()), light.diffuse.r, light.diffuse.g, light.diffuse.b);
		glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].constant").c_str()), light.constant);
		glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].linear").c_str()), light.linear);
		glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].quadratic").c_str()), light.quadratic);
		glUniform1i(glGetUniformLocation(shader->Program, ("pointLights[" + number + "].apply").c_str()), light.apply);
	}
}
