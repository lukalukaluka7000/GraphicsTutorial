#include<SDL/SDL.h>
#include<GL/glew.h>

#include"Engine.h"
namespace Engine {
	void init() {
		//Initialize sdL
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //tells SDL that we want double buffer, one drawing other is cleared,pomaze kod flickera
	}
}