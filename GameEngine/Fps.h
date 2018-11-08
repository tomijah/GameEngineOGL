#pragma once
#include "Display.h"
#include "Camera.h"
#include "InputManager.h"
class Fps
{
public:
	Fps(Display* display, Camera* camera, InputManager* input);
	~Fps();
	void Update();
private:
	Display* display;
	Camera* camera;
	InputManager* input;
	bool mouseRelative = false;
};

