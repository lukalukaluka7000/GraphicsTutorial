#include "Window.h"


namespace Engine {
	Window::Window()
	{

	}
	Window::~Window()
	{

	}
	int Window::initWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlag) {
		std::cout << screenWidth << " " << screenHeight << std::endl;

		//_screenWidth = screenWidth;
		//_screenHeight = screenHeight;

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlag & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlag & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlag & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//_window.create("Graphic Tutorial", _screenHeight, _screenWidth, 0);
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, currentFlag);

		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be opened or created!");
		}

		//OPEN GL CONTEXT VERTEX BUFFER TEXTURES, we will have context object, za sad samo window
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL GL_Context could not be or created!");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) { // to je nula
			fatalError("GLEW Init could not be instantiated or created!");
		}

		//check OpenGL version
		std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

		//set V-SYNC 1, disable 0 
		SDL_GL_SetSwapInterval(0);
		//wglSwapIntervalEXT(1)
		//printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
		return 0;
	}
	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}
}