#include "InputManager.h"


namespace Engine {
	InputManager::InputManager() {
		_mouseCoords.x = 0;
		_mouseCoords.y = 0;
	}
	InputManager::~InputManager() {

	}

	void InputManager::keyPressed(unsigned int keyID) {
		_keyMap[keyID] = true;
	}
	void InputManager::keyRelease(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyPressed(unsigned int  keyID) {
		//std::unordered_map<unsigned int, bool>::iterator
		auto it = _keyMap.find(keyID);
		if (it == _keyMap.end()) {
			return false;
		}
		else {
			return it->second;
		}
	}
	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
	glm::vec2 InputManager::getMouseCoords() {
		return _mouseCoords;
	}
}