#pragma once
#include <iostream>
#include <SDL/SDL.h>
#include <string>
#include "InputManager.h"

class Display
{
public:
	Display(int width, int height, const std::string& title);
	~Display();
	void HandleInput(InputManager * inputManager);
	void SwapBuffers();
	bool IsClosed();
	void Close();
	void EnableCursor();
	void DisableCursor();
	void InitGL(bool vsync);
	void UpdateTime();
	virtual void Log(const std::string& message);
	float fps;
	float totalTime;
	float deltaTime;
	SDL_Window * window;
private:
	bool _isClosed;
	SDL_GLContext _glContext;
	int _width;
	int _height;
	float prevTime = 0;
	int frames = 0;
	float startSample = 0;
};
