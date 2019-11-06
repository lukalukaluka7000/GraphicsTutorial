#include<iostream>
#include<Windows.h>
#include"MainGame.h"
//#include <SDL/include/SDL.h>
//creates its own function 
#include<glm/glm.hpp>
//#include<include/GL/glew.h>

int main(int argc, char** argv) {
	//SDL_Init(SDL_INIT_EVERYTHING);
	MainGame mainGame;
	mainGame.run();

	//std::cout << "Enter any key to quit...";
	//int a;
	//std::cin >> a;
	return 0;
}
