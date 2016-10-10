#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	void setMouseCoords(float x, float y);
	void setWheel(float wheel);
	bool isKeyDown(unsigned int keyID);

	bool isKeyPressed(unsigned int keyID);

	glm::vec2 getMouseCoords() const { return _mouseCoords; }
	float getWheel();
private:
	bool wasKeyDown(unsigned int keyID);
	float wheel = 0;
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords;
};

