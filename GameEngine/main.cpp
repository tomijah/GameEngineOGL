#include <iostream>
#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include <string>
#include "MeshShader.h"
#include <sstream>
#include "Model.h"
#include "Camera.h"
#include "Scene.h"
#define WIDTH 800
#define HEIGHT 600

int main(int argc, char** argv) {
	InputManager input;
	Display display(WIDTH, HEIGHT, "Game");
	display.InitGL(false);
	
	//camera setup
	Camera camera;
	camera.Target = glm::vec3(0.0f, 2.0f, 0.0f);
	camera.Position = glm::vec3(0.0f, 2.0f, 2.0f);
	
	//shader setup
	MeshShader shader;
	shader.Use();
	glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
	GLuint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");

	//models
	Model suit("models/nanosuit/nanosuit.obj");
	suit.Scale = glm::vec3(0.2f, 0.2f, 0.2f);
	suit.Position = glm::vec3(0, 0, 0);
	Model plane("models/plane.obj");
	plane.Position = glm::vec3(0, 0, 0);
	Model skeleton("models/fgc_skeleton.obj");
	skeleton.Position = glm::vec3(2.0f, 0, 0);
	skeleton.Scale = glm::vec3(0.1f, 0.1f, 0.1f);
	Model human("models/human.obj");
	human.Position = glm::vec3(4.0f, 0, 0);

	//scene
	Scene scene;
	scene.models.push_back(&plane);
	scene.models.push_back(&suit);
	scene.models.push_back(&skeleton);
	scene.models.push_back(&human);

	scene.directionLight.apply = false;
	scene.directionLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);

	PointLight p;
	p.apply = true;
	p.position = glm::vec3(0, 1.0f, 1.0f);
	p.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	scene.pointLights.push_back(p);
	PointLight* pl = &scene.pointLights[0];

	while (!display.IsClosed()) {
		input.update();
		display.HandleInput(&input);
		display.UpdateTime();

		//update
		
		glm::mat4 view = camera.getMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		if (input.isKeyPressed(SDL_BUTTON_LEFT)) {
			GLfloat winZ;
			glm::vec2 mousePos = input.getMouseCoords();
			glReadPixels((int)mousePos.x, HEIGHT - (int)mousePos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
			glm::vec3 objPt = glm::unProject(glm::vec3(mousePos.x, HEIGHT - mousePos.y, winZ), camera.getMatrix(), projection, glm::vec4(0, 0, WIDTH, HEIGHT));
			suit.Position.x = objPt.x;
			suit.Position.z = objPt.z;
			suit.recalculateMatrix = true;
		}


		if (input.isKeyDown(SDLK_UP)) {
			camera.Position.y = camera.Position.y + display.deltaTime / 1000.0f;
			camera.Target.y = camera.Target.y + display.deltaTime / 1000.0f;
			camera.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_DOWN)) {
			camera.Position.y = camera.Position.y - display.deltaTime / 1000.0f;
			camera.Target.y = camera.Target.y - display.deltaTime / 1000.0f;
			camera.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_LEFT)) {
			camera.Position.x = camera.Position.x - display.deltaTime / 1000.0f;
			camera.Target.x = camera.Target.x - display.deltaTime / 1000.0f;
			camera.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_RIGHT)) {
			camera.Position.x = camera.Position.x + display.deltaTime / 1000.0f;
			camera.Target.x = camera.Target.x + display.deltaTime / 1000.0f;
			camera.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_w)) {
			camera.Position.z = camera.Position.z - display.deltaTime / 1000.0f;
			camera.Target.z = camera.Target.z - display.deltaTime / 1000.0f;
			camera.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_s)) {
			camera.Position.z = camera.Position.z + display.deltaTime / 1000.0f;
			camera.Target.z = camera.Target.z + display.deltaTime / 1000.0f;
			camera.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_a)) {
			suit.Rotation = glm::vec3(0, suit.Rotation.y - display.deltaTime / 1000.f, 0);
			suit.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_d)) {
			suit.Rotation = glm::vec3(0, suit.Rotation.y + display.deltaTime / 1000.f, 0);
			suit.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_q)) {
			pl->position.y += display.deltaTime / 1000.f;
			scene.updateLights = true;
		}

		if (input.isKeyDown(SDLK_e)) {
			pl->position.y -= display.deltaTime / 1000.f;
			scene.updateLights = true;
		}

		//draw
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		scene.Draw(shader);
		display.SwapBuffers();

		std::stringstream ss;

		ss << "Game engine | fps: " << display.fps;

		SDL_SetWindowTitle(display.window, ss.str().c_str());
	}

	return 0;
}