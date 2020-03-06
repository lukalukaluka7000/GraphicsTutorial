#pragma once
#include<iostream>
#include<string>

#include<SDL/SDL.h>
#include<GL/glew.h>

#include<Engine/Errors.h>

namespace Engine {
	enum  WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };
	class Window
	{
	public:
		Window();
		~Window();
		int initWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int windowFlag);
		int getScreenWidth() { _screenWidth; }
		int getScreenHeight() { _screenHeight; }
		void swapBuffer();
	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;
	};

}