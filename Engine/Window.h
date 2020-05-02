#pragma once
#include<iostream>
#include<string>

#include<SDL/SDL.h>
#include<GL/glew.h>

#include<Engine/EngineErrors.h>

namespace Engine {
	enum  WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };
	class Window
	{
	public:
		Window();
		~Window();
		int initWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int windowFlag);
<<<<<<< HEAD
		int getScreenWidth() { return _screenWidth; }
		int getScreenHeight() { return _screenHeight; }
=======
		int getScreenWidth() { _screenWidth; }
		int getScreenHeight() { _screenHeight; }
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5
		void swapBuffer();
	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;
	};

}