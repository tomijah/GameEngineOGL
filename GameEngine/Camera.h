#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 5.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float width = 1280.0f, float height = 720.0f);
	~Camera();
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Target;
	bool recalculateMatrix = true;
	glm::mat4 getViewMatrix();
	glm::mat4 Projection;
	void Look(glm::vec3 target);
private:
	glm::mat4 viewMatrix;
};

