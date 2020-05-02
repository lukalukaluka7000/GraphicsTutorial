#pragma once //protecting from circular includes



#include<Engine/Sprite.h>
#include<Engine/GLTexture.h>
#include<Engine/GLSLProgram.h>
#include<Engine/Window.h>
#include<Engine/Camera2D.h>
#include<Engine/SpriteBatch.h>
#include<Engine/InputManager.h>
#include<Engine/Timing.h>
#include<Engine/Bullet.h>

#include<vector>
enum class GameState {PLAY,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	void drawGame(); // put triangels on screeen
	//void calculateFPS();
	
private:
	void initSystems();
	//initShaders called from inside of initSystems
	void initShaders();
	void gameLoop();
	void processInput();
	//SDL_Window* _window; //_ for private
	
	Engine::Window _window;
	int _screenWidth;
	int _screenHeight;
	float _time;
	GameState _gameState;

	//Sprite _sprite;
	//std::vector<Engine::Sprite*> _sprites; //vise nam ne treba jer imamo SpriteBatch
	Engine::GLSLProgram _colorProgram; //video 10 basic color program
	Engine::Camera2D _camera;
	//ZaKOMENTIRALI JER CEMO PUNO PAMETNIJI OVIME BARATAT
	//GLTexture _playerTexture;
	Engine::SpriteBatch _spriteBatch;
	Engine::InputManager _inputManager;
	Engine::FPSLimiter _fpsLimiter;
	std::vector<Engine::Bullet> _bullets;
	float _fps;
	float _maxFPS;
	int _counter;

};

