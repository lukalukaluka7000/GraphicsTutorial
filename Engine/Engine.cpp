#include<SDL/SDL.h>
#include<GL/glew.h>

#include"Engine.h"
namespace Engine {
	void init() {
		//Initialize sdL
		SDL_Init(SDL_INIT_EVERYTHING);
<<<<<<< HEAD

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //OpenGL 3+
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); //OpenGL 3.3
=======
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //tells SDL that we want double buffer, one drawing other is cleared,pomaze kod flickera
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5
	}
}