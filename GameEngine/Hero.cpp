#include "Hero.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

Hero::~Hero() 
{
}

Hero::Hero(Model * soureModel) :model(soureModel)
{
	moveTarget = soureModel->Position;
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

		
		glm::quat targetRot;
		targetRot = glm::rotate(targetRot, rotationTarget, glm::vec3(0, 1, 0));
		model->Rotation = slerp(model->Rotation, targetRot, 0.1f);
		if (glm::dot(model->Rotation, targetRot) < 0.01f) {
			model->Rotation = targetRot;
			rotating = false;
		}

		model->recalculateMatrix = true;
	}

	
}
