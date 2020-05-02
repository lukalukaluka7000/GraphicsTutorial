#pragma once
#include<iostream>
#include<unordered_map>
#include<glm/glm.hpp>

namespace Engine {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void keyPressed(unsigned int keyID);
		void keyRelease(unsigned int keyID);

		//return true if key is held down
		bool isKeyDown(unsigned int  keyID);

		//returns true if the key was just pressed
		bool isKeyPressed(unsigned int  keyID);

		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords();
	private:
		bool wasKeyDown(unsigned int  keyID);
		std::unordered_map<unsigned int, bool> _keyMap; 
		std::unordered_map<unsigned int, bool> _previouskeyMap;
		glm::vec2 _mouseCoords;
	};
}
