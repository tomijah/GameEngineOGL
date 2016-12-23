#include "Hero.h"



Hero::~Hero() 
{
}


Hero::Hero(Model * soureModel) :model(soureModel)
{
	moveTarget = soureModel->Position;
	model->Rotation.y = glm::pi<float>() * 2;
	rotationTarget = soureModel->Rotation.y;
}

void Hero::move(glm::vec3 target)
{
	moveTarget = target;
	glm::vec3 moveVector = moveTarget - model->Position;
	rotationTarget = glm::atan(moveVector.x, moveVector.z);

	moving = true;
	rotating = true;
}

void Hero::update(GLfloat deltaT)
{
	if (moving || rotating) {
		glm::vec3 moveVector = moveTarget - model->Position;
		GLfloat distance = glm::length(moveVector);
		glm::vec3 moveVectorNormalized = glm::normalize(moveVector);

		if (moving) {
			
			GLfloat moveDistance = deltaT * moveSpeed / 1000.0f;

			if (distance > moveDistance) {
				model->Position = model->Position + moveVectorNormalized * moveDistance;
			}
			else {
				model->Position = moveTarget;
				moving = false;
			}
		}

		if (rotating) {
			GLfloat rotationDelta = deltaT * rotationSpeed / 1000.0f;
			GLfloat rotationDistance = glm::abs(model->Rotation.y - rotationTarget);
			
			if (rotationDistance > rotationDelta) {
				if (model->Rotation.y < rotationTarget) {
					model->Rotation.y += rotationDelta;
				}
				else {
					model->Rotation.y -= rotationDelta;
				}
			}
			else
			{
				model->Rotation.y = rotationTarget;
				rotating = false;
			}
		}

		std::cout << model->Rotation.y << std::endl;

		model->recalculateMatrix = true;
	}

	
}
