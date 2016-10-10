#include <iostream>
#include "Utils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include <string>
#include "MeshShader.h"
#include "Model.h"
#include "Camera.h"

#define WIDTH 1800
#define HEIGHT 900

int main(int argc, char** argv) {
	InputManager input;
	Display display(WIDTH, HEIGHT, "Game");
	display.InitGL(false);

	MeshShader shader;
	Model wolf("models/nanosuit/nanosuit.obj");
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	shader.Use();
	glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
	GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	GLuint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
	while (!display.IsClosed()) {
		input.update();
		display.HandleInput(&input);
		display.UpdateTime();

		//update
		if (input.isKeyDown(SDLK_UP)) {
			camera.ProcessKeyboard(FORWARD, display.deltaTime / 1000.0f);
		}
		if (input.isKeyDown(SDLK_DOWN)) {
			camera.ProcessKeyboard(BACKWARD, display.deltaTime / 1000.0f);
		}

		if (input.isKeyDown(SDLK_LEFT)) {
			camera.ProcessKeyboard(LEFT, display.deltaTime / 1000.0f);
		}

		if (input.isKeyDown(SDLK_RIGHT)) {
			camera.ProcessKeyboard(RIGHT, display.deltaTime / 1000.0f);
		}

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//draw
		
		
						// Transformation matrices
		
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::rotate(model, display.totalTime/1000.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		wolf.Draw(shader);


		display.SwapBuffers();

		std::stringstream ss;

		ss << "Game engine | fps: " << display.fps;

		SDL_SetWindowTitle(display.window, ss.str().c_str());
	}

	return 0;
}