#include "TestGame.h"
#include <iostream>
#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MeshShader.h"
#include <sstream>
#include "Model.h"
#include "Camera.h"
#include "Scene.h"
#include "Hero.h"
#include "ShaderFromFile.h"
#include "Skybox.h"
#include <vector>
#include "Framebuffer.h"
#include "QuadRenderer.h"
#include "Fps.h"

#define WIDTH 1280
#define HEIGHT 720

using namespace glm;
TestGame::TestGame()
{
}


TestGame::~TestGame()
{
}

void TestGame::Run()
{
	InputManager input;
	Display display(WIDTH, HEIGHT, "Game");
	display.InitGL(true);

	//camera setup
	Camera camera;
	camera.Position = glm::vec3(0.0f, 5.0f, 5.0f);
	camera.Look(glm::vec3(0.0f, 1.0f, 0.0f));
	//shader setup
	ShaderFromFile shader;
	//MeshShader shader;
	shader.Add("../GameEngine/Shaders/mesh.vert", GL_VERTEX_SHADER);
	shader.Add("../GameEngine/Shaders/mesh.frag", GL_FRAGMENT_SHADER);
	shader.Link();

	ShaderFromFile dpShader;
	dpShader.Add("../GameEngine/Shaders/simple.vert", GL_VERTEX_SHADER);
	dpShader.Add("../GameEngine/Shaders/simple.frag", GL_FRAGMENT_SHADER);
	dpShader.Link();

	ShaderFromFile skyboxShader;
	skyboxShader.Add("../GameEngine/Shaders/skybox.vert", GL_VERTEX_SHADER);
	skyboxShader.Add("../GameEngine/Shaders/skybox.frag", GL_FRAGMENT_SHADER);
	skyboxShader.Link();

	shader.Use();
	shader.SetMatrix4("projection", camera.Projection);

	skyboxShader.Use();
	skyboxShader.SetInteger("skybox", 0);

	QuadRenderer qr;

	std::vector<std::string> faces
	{
		"models/skybox/right.jpg",
		"models/skybox/left.jpg",
		"models/skybox/top.jpg",
		"models/skybox/bottom.jpg",
		"models/skybox/front.jpg",
		"models/skybox/back.jpg"
	};

	Skybox skybox(faces);
	//shadows
	Framebuffer shadowMap;
	GLuint shadowMapTexture = shadowMap.PrepareDepthMap(2048, 2048);
	ShaderFromFile simpleDepthShader;
	simpleDepthShader.Add("../GameEngine/Shaders/depth.vert", GL_VERTEX_SHADER);
	simpleDepthShader.Add("../GameEngine/Shaders/depth.frag", GL_FRAGMENT_SHADER);
	simpleDepthShader.Link();

	//models
	Model suit("models/stormtrooper.dae");
	//Model suit("models/queen/queen-of-cancelot.obj");
	suit.Scale = glm::vec3(1.5f, 1.5f, 1.5f);
	suit.Position = glm::vec3(0, 0, 0);
	for (GLuint i = 0; i < suit.meshes.size(); i++) {
		suit.meshes[i].material.reflect = true;
		suit.meshes[i].material.shininess = 1.0f;
	}

	Model plane("models/plane.obj");
	plane.Position = glm::vec3(0, 0, 0);
	Model skeleton("models/deadpool/dblend.obj");
	skeleton.Position = glm::vec3(3.0f, 0, 0);
	skeleton.Scale = glm::vec3(0.8f, 0.8f, 0.8f);
	for (GLuint i = 0; i < skeleton.meshes.size(); i++)
		skeleton.meshes[i].material.reflect = true;

	Model cube("models/cube.dae");
	cube.Scale = glm::vec3(0.2f, 0.2f, 0.2f);
	cube.Position = glm::vec3(4.0f, 0, 0);
	Material mat;
	mat.color = vec3(1.0f, 1.0f, 1.0f);
	mat.applyLights = false;
	cube.meshes[0].material = mat;
	
	//scene
	Scene scene;
	scene.models.push_back(&plane);
	scene.models.push_back(&suit);
	scene.models.push_back(&cube);
	scene.models.push_back(&skeleton);

	//lights
	scene.directionLight.apply = true;
	scene.directionLight.ambient = glm::vec3(0.4f);
	scene.directionLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);

	PointLight p;
	p.apply = true;
	p.constant = 1.0f;
	p.linear = 0.01f;
	p.quadratic = 0.05f;
	p.position = glm::vec3(-2.0f, 1.0f, 1.0f);
	p.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	scene.pointLights.push_back(p);

	PointLight* pl = &scene.pointLights[0];

	Hero hero(&skeleton);
	bool mouseRelative = false;
	Fps fpp(&display, &camera, &input);
	while (!display.IsClosed()) {
		input.update();
		display.HandleInput(&input);
		display.UpdateTime();

		//update
		pl->position.x = glm::sin(display.totalTime / 800.0f) * 4.0f;
		pl->position.z = glm::cos(display.totalTime / 800.0f) * 5.0f;
		cube.Position = pl->position;
		cube.recalculateMatrix = true;
		scene.updateLights = true;

		if (input.isKeyDown(SDL_BUTTON_LEFT) && !mouseRelative) {
			GLfloat winZ;
			glm::vec2 mousePos = input.getMouseCoords();
			glReadPixels((int)mousePos.x, HEIGHT - (int)mousePos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
			glm::vec3 objPt = glm::unProject(glm::vec3(mousePos.x, HEIGHT - mousePos.y, winZ), camera.getViewMatrix(), camera.Projection, glm::vec4(0, 0, WIDTH, HEIGHT));
			hero.move(glm::vec3(objPt.x, 0, objPt.z));
		}

		if (input.isKeyPressed(SDLK_m)) {
			mouseRelative = !mouseRelative;
		}

		if (input.isKeyDown(SDLK_q)) {
			skeleton.Rotation = glm::rotate(skeleton.Rotation, -display.deltaTime / 1000.f, glm::vec3(0, 1, 0));
			skeleton.recalculateMatrix = true;
		}

		if (input.isKeyDown(SDLK_e)) {
			skeleton.Rotation = glm::rotate(skeleton.Rotation, display.deltaTime / 1000.f, glm::vec3(0, 1, 0));
			skeleton.recalculateMatrix = true;
		}

		fpp.Update();
		hero.update(display.deltaTime);

		//draw
		glClearDepth(1.0);
		glClearColor(0.0f, 0.0f, 0.0F, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		simpleDepthShader.Use();
		glViewport(0, 0, 2048, 2048);
		shadowMap.Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		scene.DrawShadowMap(&simpleDepthShader, &scene.directionLight);
		//glCullFace(GL_BACK);

		Framebuffer::BindDefault();
		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.Use();
		
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
		GLint loc = glGetUniformLocation(shader.Program, "shadowMap");
		glUniform1i(loc, 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureId);
		GLint loc2 = glGetUniformLocation(shader.Program, "skybox");
		glUniform1i(loc2, 4);

		scene.Draw(&shader, &camera);
		skybox.Draw(&skyboxShader, &camera);

		glm::mat4 qq;
		qq = glm::translate(qq, vec3(0.7f,-0.6f, 0.0f));
		qq = glm::scale(qq, vec3(0.3f,0.4f,0.0f));
		qr.Render(&dpShader, qq, shadowMapTexture);

		display.SwapBuffers();
		std::stringstream ss;
		ss << "Game engine | fps: " << display.fps;
		SDL_SetWindowTitle(display.window, ss.str().c_str());
	}
}
