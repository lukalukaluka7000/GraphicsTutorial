#pragma once //protecting from circular includes

#include <SDL/include/SDL.h>
#include<include/GL/glew.h>
#include<Engine/Sprite.h>
#include<Engine/GLTexture.h>
#include<Engine/GLSLProgram.h> //include our shader video 10
//#include<Engine/Window.h>
#include<vector>
enum class GameState {PLAY,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	void drawGame(); // put triangels on screeen
	void calculateFPS();

private:
	void initSystems();
	//initShaders called from inside of initSystems
	void initShaders();
	void gameLoop();
	void processInput();
	SDL_Window* _window; //_ for private
	//Window _window;
	int _screenWidth;
	int _screenHeight;
	float _time;
	GameState _gameState;
	float _maxFPS;
	//Sprite _sprite;
	std::vector<Sprite*> _sprites;
	GLSLProgram _colorProgram; //video 10 basic color program
	//just a member variable
	//ZKOMENTIRALI JER CEMO PUNO PAMETNIJI OVIME BARATAT
	//GLTexture _playerTexture;
	//let us initialize shaders in initSystems
	float _fps;
	float _frameTime;
};

