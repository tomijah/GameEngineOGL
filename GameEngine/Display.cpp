#include "Display.h"
#include "Utils.h"
#include <string>

Display::Display(int width, int height, const std::string& title) : _width(width), _height(height)
{
	_isClosed = false;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL);
	_glContext = SDL_GL_CreateContext(window);
}

Display::~Display()
{
	SDL_GL_DeleteContext(this->_glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::HandleInput(InputManager * inputManager)
{
	SDL_Event e;
	inputManager->setWheel(0.0f);
	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
		case SDL_QUIT:
			_isClosed = true;
			break;
		case SDL_MOUSEMOTION:
			inputManager->setMouseCoords((float)e.motion.x, (float)e.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager->pressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager->releaseKey(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager->pressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager->releaseKey(e.button.button);
			break;
		case SDL_MOUSEWHEEL:
			inputManager->setWheel((float)e.wheel.y);
		default:
			break;
		}
	}
}

void Display::SwapBuffers()
{
	SDL_GL_SwapWindow(window);
}

bool Display::IsClosed()
{
	return _isClosed;
}

void Display::EnableCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void Display::DisableCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
}

void Display::InitGL(bool vsync)
{
	glewExperimental = true;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//antialiasing
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//vsync
	if (vsync) {
		SDL_GL_SetSwapInterval(1);
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, _width, _height);
}

void Display::UpdateTime()
{
	totalTime = (float)SDL_GetTicks();
	deltaTime = totalTime - prevTime;

	prevTime = totalTime;
	frames++;
	if (totalTime - startSample >= 1000.0f) {
		fps = round(frames * (1000.0f / (totalTime - startSample)));
		startSample = totalTime;
		frames = 0;
	}
}

void Display::Log(const std::string& message)
{
	std::cout << message;
}
