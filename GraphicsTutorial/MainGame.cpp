#include "MainGame.h"
#include <iostream>
#include <string>
#include<Engine/Sprite.h>
#include<Engine/Errors.h>
#include<Engine/ImageLoader.h>
//#include<Engine/Window.h>
#include<include/GL/glew.h>

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::PLAY; //:: scope
	_time = 0.0f;
	_maxFPS = 10000.0f;
}


MainGame::~MainGame()
{
}


void MainGame::run() {
	initSystems();

	//_sprite.init(-0.7f, -0.7f, 1.2f, 1.2f);
	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "./Textures/Dudes/telescope_01.png");
	//_sprite.init(-1.0f, -1.0f, 1.0f, 1.0f, "./Textures/Dudes/store_shoppeKeeper.png");


	_sprites.push_back(new Sprite()); 
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "./Textures/Dudes/store_shoppeKeeper.png");
	//_sprite.init(0.0f, -1.0f, 2.0f, 2.0f, "./Textures/Dudes/store_shoppeKeeper.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "./Textures/Dudes/store_shoppeKeeper.png");
	
	//_sprite.init(0.0f, -1.0f, 2.0f, 2.0f, "./Textures/Dudes/store_shoppeKeeper.png");
	//for (int i = 0; i < 10; i++) {
	//	_sprites.push_back(new Sprite());
	//	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "./Textures/Dudes/store_shoppeKeeper.png");
	//}

	//_playerTexture = ImageLoader::loadPNG("./Textures/Dudes/store_shoppeKeeper.png");
	//now use the texture
	

	gameLoop();
}

void MainGame::initSystems() {

	//Initialize sdL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //tells SDL that we want double buffer, one drawing other is cleared,pomaze kod flickera
	
	//_window.create("Graphic Tutorial", _screenHeight, _screenWidth, 0);
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

	if (_window == nullptr) {
		fatalError("SDL Window could not be opened or created!");
	}

	//OPEN GL CONTEXT VERTEX BUFFER TEXTURES, we will have context object, za sad samo window
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL GL_Context could not be or created!");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) { // to je nula
		fatalError("GLEW Init could not be instantiated or created!");
	}

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	SDL_GL_SetSwapInterval(0);
	//wglSwapIntervalEXT(1)
	//printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

	//initShaders called from inside of initSystems
	initShaders(); //VIDEO 10  
}
void MainGame::initShaders() {
	std::cout << "Usa u initShaders" << std::endl;

	_colorProgram.compileShaders("./Shaders/colorShading.vert", "./Shaders/colorShading.frag");
	std::cout << "1" << std::endl;
	_colorProgram.addAttribute("vertexPosition");
	std::cout << "2" << std::endl;
	/*_colorProgram.addAttribute("vertexColor");
	std::cout << "3" << std::endl;
	_colorProgram.addAttribute("vertexUV");
	std::cout << "4" << std::endl;*/

	_colorProgram.linkShaders();
	std::cout << "5" << std::endl;


}
//A  B
//1. drawing A put it on screen, switch to B
//2. rendering B, clearing A

void MainGame::gameLoop() {
	int counter = 0;
 	while (_gameState != GameState::EXIT) {
		
		float startTicks = SDL_GetTicks();

		processInput();

		_time += 0.001;

		drawGame();
		calculateFPS();

		if (counter % 50 == 0) {
			std::cout << _fps << std::endl;
		}
		float frameTicks = SDL_GetTicks() - startTicks;
		if ((1000.0f / _maxFPS) > frameTicks) {
			SDL_Delay((1000.0f / _maxFPS) - frameTicks);
		}
		counter++;
	}
}

void MainGame::processInput() {
 //use sdl for this function
	//returning 1 if true zero is false 
	SDL_Event evnt;

	//passing by reference this up pointer NAJS
	while (SDL_PollEvent(&evnt) == true) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		}
	}
}

void MainGame::drawGame() {
	//what depth opengl should clear to
	glClearDepth(1.0); 

	//clear the screen, clean buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	
	//zzkomentirali u 17. videu
	//glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	
	
	//GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//glUniform1i(textureLocation, 0);


	//GLint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time); //send variable to gpu

	for (int i = 0; i < _sprites.size(); i++) {
		_sprites[i]->draw();
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);

	
	_colorProgram.unuse();


	SDL_GL_SwapWindow(_window);
	//_window.swapBuffer();

	//glClearColor(0.0, 0.0, 1.0, 1.0);

	//initShaders();
}


void MainGame::calculateFPS() {
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	
	static int currentFrame = 0;
	//std::cout << currentFrame << std::endl;
	static float previousTicks = SDL_GetTicks();
	float currentTicks;
	currentTicks = SDL_GetTicks();

	int count;

	_frameTime = currentTicks - previousTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	previousTicks = currentTicks;


	currentFrame++;

	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}
	else {
		count = NUM_SAMPLES;
	}

	float frameTimeAvgs = 0;
	float avg;

	for (int i = 0; i < count; i++) {
		frameTimeAvgs += frameTimes[i];
	}
	avg = frameTimeAvgs/count;

	if (avg > 0) {
		_fps = 1000 / avg;
	}
	else
	{
		_fps = 60.0f;
	}
}