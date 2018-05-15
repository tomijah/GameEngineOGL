#include "ProceduralTest.h"
#include "Utils.h"
#include "Camera.h"
#include "Scene.h"
#include "MeshShader.h"

#define WIDTH 1280
#define HEIGHT 720


ProceduralTest::ProceduralTest()
{
}


ProceduralTest::~ProceduralTest()
{
}

void ProceduralTest::Run()
{
	InputManager input;
	Display display(WIDTH, HEIGHT, "Procedural");
	display.InitGL(false);
	Camera camera;
	Scene scene;
	scene.directionLight.apply = true;
	scene.directionLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	MeshShader shader;

	while (!display.IsClosed()) {
		input.update();
		display.HandleInput(&input);
		display.UpdateTime();
		glm::mat4 view = camera.getViewMatrix();

		//draw
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		scene.Draw(&shader, &camera);

		display.SwapBuffers();
	}
}
