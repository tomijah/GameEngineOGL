#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up)
{
	this->Position = position;
	this->Up = up;
	this->Target = glm::vec3(0);
}

Camera::~Camera()
{
}

glm::mat4 Camera::getMatrix()
{
	if (recalculateMatrix) {
		viewMatrix = glm::lookAt(Position, Target, Up);
		recalculateMatrix = false;
	}

	return viewMatrix;
}
