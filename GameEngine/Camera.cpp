#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float width, float height)
{
	this->Position = position;
	this->Up = up;
	this->Target = glm::vec3(1.0f, 0.0f, 0.0f);
	this->Projection = glm::perspective(45.0f, width / height, 0.1f, 100.0f);
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	if (recalculateMatrix) {
		viewMatrix = glm::lookAt(Position, Position + Target, Up);
		recalculateMatrix = false;
	}

	return viewMatrix;
}

void Camera::Look(glm::vec3 target)
{
	Target = glm::normalize(target - Position);
	recalculateMatrix = true;
}
