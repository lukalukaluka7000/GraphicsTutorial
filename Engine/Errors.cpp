#pragma once
#include<iostream>
#include<cstdlib>
#include"Errors.h"


void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(69);
}