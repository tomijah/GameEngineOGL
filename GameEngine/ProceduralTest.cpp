#include "ProceduralTest.h"
#include "Utils.h"
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include "ShaderFromFile.h"
#include "Fps.h"
#include <sstream>
#include "Perlin.h"
#define WIDTH 1280
#define HEIGHT 720
using namespace glm;

Model generateModel(vec3 up, int resolution);
void updateModel(vec3 up, int resolution, Model* mesh);
ProceduralTest::ProceduralTest()
{
}


ProceduralTest::~ProceduralTest()
{
}

void ProceduralTest::Run()
{
	int resolution = 20;
	InputManager input;
	Display display(WIDTH, HEIGHT, "Procedural");
	display.InitGL(true);
	Camera camera;
	camera.Position = glm::vec3(0.0f, 0.0f, 100.0f);
	camera.Look(glm::vec3(0.0f, 0.0f, 0.0f));

	Model custom1 = generateModel(vec3(0, 0, 1), resolution);
	Model custom2 = generateModel(vec3(0, 0, -1), resolution);
	Model custom3 = generateModel(vec3(1, 0, 0), resolution);
	Model custom4 = generateModel(vec3(-1, 0, 0), resolution);
	Model custom5 = generateModel(vec3(0, 1, 0), resolution);
	Model custom6 = generateModel(vec3(0, -1, 0), resolution);

	Scene scene;
	scene.models.push_back(&custom1);
	scene.models.push_back(&custom2);
	scene.models.push_back(&custom3);
	scene.models.push_back(&custom4);
	scene.models.push_back(&custom5);
	scene.models.push_back(&custom6);

	scene.directionLight.apply = true;
	scene.directionLight.ambient = glm::vec3(0.4f);
	scene.directionLight.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	scene.directionLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);

	ShaderFromFile shader;
	shader.Add("../GameEngine/Shaders/terr.vert", GL_VERTEX_SHADER);
	shader.Add("../GameEngine/Shaders/terr.frag", GL_FRAGMENT_SHADER);
	shader.Link();
	Fps fpp(&display, &camera, &input);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!display.IsClosed()) {
		input.update();
		display.HandleInput(&input);
		display.UpdateTime();
		//glm::mat4 view = camera.getViewMatrix();
		/*glm::quat targetRot;
		targetRot = glm::rotate(targetRot, display.deltaTime/700.0f, glm::vec3(0, 1, 0));
		custom.Rotation = custom.Rotation * targetRot;
		custom.recalculateMatrix = true;*/
		fpp.Update();
		
		if (input.isKeyPressed(SDLK_o)) {
			resolution+=resolution;
			updateModel(vec3(0, 0, 1), resolution, &custom1);
			updateModel(vec3(0, 0, -1), resolution, &custom2);
			updateModel(vec3(1, 0, 0), resolution, &custom3);
			updateModel(vec3(-1, 0, 0), resolution, &custom4);
			updateModel(vec3(0, 1, 0), resolution, &custom5);
			updateModel(vec3(0, -1, 0), resolution, &custom6);
		}

		if (input.isKeyPressed(SDLK_l)) {
			if (resolution > 2) {
				resolution--;
				updateModel(vec3(0, 0, 1), resolution, &custom1);
				updateModel(vec3(0, 0, -1), resolution, &custom2);
				updateModel(vec3(1, 0, 0), resolution, &custom3);
				updateModel(vec3(-1, 0, 0), resolution, &custom4);
				updateModel(vec3(0, 1, 0), resolution, &custom5);
				updateModel(vec3(0, -1, 0), resolution, &custom6);
			}
		}

		if (input.isKeyDown(SDLK_SPACE)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		//draw
		glClearColor(0.0f, 0.0f, 0.0F, 1.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		shader.Use();
		glPointSize(10);
		scene.Draw(&shader, &camera);

		display.SwapBuffers();
		std::stringstream ss;
		ss << "Procedural test | fps: " << display.fps;
		SDL_SetWindowTitle(display.window, ss.str().c_str());
	}
}

Model generateModel(vec3 up, int resolution) {
	vec3 y = -vec3(up.y, up.z, up.x);
	vec3 x = cross(up, y);

	std::vector<Mesh> meshes;
	Material mat;
	mat.applyLights = true;
	mat.color = glm::vec3(1.0f, 1.0f, 1.0f);
	mat.shininess = 1;
	//mat.applyLights = false;
	std::vector<Vertex> verts;
	std::vector<unsigned int> inds;

	float radious = 50;
	float spacing = 2.0f / (resolution - 1);

	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			vec3 posOnCube = -x + x * (spacing * i) - y + y * (spacing * j) + up;
			Vertex v;
			v.position = glm::normalize(posOnCube) * radious;
			v.normal = glm::normalize(v.position);
			verts.push_back(v);
			if (i != resolution - 1 && j != resolution - 1) {
				int ind = j * resolution + i;
				inds.push_back(ind);
				inds.push_back(ind + resolution + 1);
				inds.push_back(ind + resolution);

				inds.push_back(ind);
				inds.push_back(ind + 1);
				inds.push_back(ind + resolution + 1);
			}
		}
	}

	//done

	Mesh m(verts, inds, mat);
	meshes.push_back(m);
	Model result(meshes);
	return result;
}

void updateModel(vec3 up, int resolution, Model* mesh) {
	vec3 y = -vec3(up.y, up.z, up.x);
	vec3 x = cross(up, y);

	std::vector<Vertex> verts;
	std::vector<unsigned int> inds;

	float radious = 50;
	float spacing = 2.0f / (resolution - 1);
	const siv::PerlinNoise perlin(112312);
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			vec3 posOnCube = -x + x * (spacing * i) - y + y * (spacing * j) + up;
			Vertex v;
			vec3 posOnSphere = glm::normalize(posOnCube);
			double p = perlin.octaveNoise(posOnSphere.x*6000, posOnSphere.y*6000, posOnSphere.z*5000, 1)*2;
			//std::cout << p<<"\n";
			v.position = posOnSphere * (float)(radious + p);
			v.normal = glm::normalize(v.position);
			verts.push_back(v);
			if (i != resolution - 1 && j != resolution - 1) {
				int ind = j * resolution + i;
				inds.push_back(ind);
				inds.push_back(ind + resolution + 1);
				inds.push_back(ind + resolution);

				inds.push_back(ind);
				inds.push_back(ind + 1);
				inds.push_back(ind + resolution + 1);
			}
		}
	}

	mesh->meshes[0].UpdateGeometry(verts, inds);
}