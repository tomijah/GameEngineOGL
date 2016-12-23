#pragma once
#include "Model.h"
#include "Utils.h"

class Hero
{
public:
	Hero(Model* soureModel);
	~Hero();
	Model* model;
	glm::vec3 moveTarget;
	GLfloat rotationTarget;
	void move(glm::vec3 target);
	void update(GLfloat deltaT);
private:
	bool moving = false;
	bool rotating = false;
	GLfloat rotationSpeed = 10.0f;
	GLfloat moveSpeed = 10.0f;
};

