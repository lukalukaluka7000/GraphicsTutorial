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

		void keyPressed(unsigned int keyID);
		void keyRelease(unsigned int keyID);

		bool isKeyPressed(unsigned int  keyID);
		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords();
	private:
		std::unordered_map<unsigned int, bool> _keyMap; 
		glm::vec2 _mouseCoords;
	};
}
