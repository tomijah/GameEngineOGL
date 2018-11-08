#include "Fps.h"



Fps::Fps(Display * display, Camera * camera, InputManager * input): display(display), camera(camera), input(input)
{
	
}

Fps::~Fps()
{
}

void Fps::Update()
{
	if (this->input->isKeyPressed(SDLK_m)) {
		mouseRelative = !mouseRelative;
		SDL_SetRelativeMouseMode(mouseRelative ? SDL_TRUE : SDL_FALSE);
	}
	if (this->input->isKeyPressed(SDLK_ESCAPE)) {
		this->display->Close();
	}

	if (this->input->isKeyDown(SDLK_UP)) {
		this->camera->Position.y = this->camera->Position.y + this->display->deltaTime / 1000.0f;
		this->camera->Target.y = this->camera->Target.y + this->display->deltaTime / 1000.0f;
		this->camera->recalculateMatrix = true;
	}

	if (this->input->isKeyDown(SDLK_DOWN)) {
		this->camera->Position.y = this->camera->Position.y - this->display->deltaTime / 1000.0f;
		this->camera->Target.y = this->camera->Target.y - this->display->deltaTime / 1000.0f;
		this->camera->recalculateMatrix = true;
	}

	if (this->input->isKeyDown(SDLK_LEFT)) {
		this->camera->Position.x = this->camera->Position.x - this->display->deltaTime / 1000.0f;
		this->camera->Target.x = this->camera->Target.x - this->display->deltaTime / 1000.0f;
		this->camera->recalculateMatrix = true;
	}

	if (this->input->isKeyDown(SDLK_RIGHT)) {
		this->camera->Position.x = this->camera->Position.x + this->display->deltaTime / 1000.0f;
		this->camera->Target.x = this->camera->Target.x + this->display->deltaTime / 1000.0f;
		this->camera->recalculateMatrix = true;
	}

	if (this->input->isKeyDown(SDLK_w)) {
		this->camera->Position += this->camera->Target * this->display->deltaTime / 400.0f;
		this->camera->recalculateMatrix = true;
	}

	if (this->input->isKeyDown(SDLK_s)) {
		this->camera->Position -= this->camera->Target * this->display->deltaTime / 400.0f;
		this->camera->recalculateMatrix = true;
	}

	if (this->input->isKeyDown(SDLK_a)) {
		glm::vec3 plane = glm::cross(this->camera->Target, this->camera->Up);
		this->camera->Position -= plane * this->display->deltaTime / 300.0f;
		this->camera->recalculateMatrix = true;
	}

	if (this->input->isKeyDown(SDLK_d)) {
		glm::vec3 plane = glm::cross(this->camera->Target, this->camera->Up);
		this->camera->Position += plane * this->display->deltaTime / 300.0f;
		this->camera->recalculateMatrix = true;
	}
	if (mouseRelative) {
		glm::mat4 rot;
		rot = glm::rotate(rot, this->input->getMouseDelta().x * (this->display->deltaTime / 2000.0f), this->camera->Up);
		rot = glm::rotate(rot, this->input->getMouseDelta().y * (this->display->deltaTime / 2000.0f), glm::cross(this->camera->Target, this->camera->Up));
		glm::vec4 a = glm::vec4(this->camera->Target.x, this->camera->Target.y, this->camera->Target.z, 1.0f) * rot;
		glm::vec3 t = glm::normalize(glm::vec3(a.x, a.y, a.z));
		this->camera->Target = t;
		this->camera->recalculateMatrix = true;
	}
}
