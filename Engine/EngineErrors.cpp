#pragma once
#include<iostream>
#include<cstdlib>
#include"EngineErrors.h"

namespace Engine {
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		//triba kliknit enter pa nek bude TODO
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(69);
	}
}