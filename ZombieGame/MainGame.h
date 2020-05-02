#pragma once //protecting from circular includes


#include<Engine/Window.h>
#include<Engine/GLSLProgram.h>
#include<Engine/InputManager.h>
#include<Engine/Camera2D.h>

#include<Engine/Engine.h>
#include<Engine/Timing.h>
#include<Engine/SpriteBatch.h>
#include<Engine/SpriteFont.h>
#include<Engine/AudioEngine.h>
#include"Level.h"

#include"Player.h"

#include"Gun.h"
#include"Bullet.h"

#include<Engine/ParticleEngine2D.h>
#include<Engine/ParticleBatch2D.h>
class Gun;
class Bullet;
enum class GameState { PLAY, EXIT };

class Gun;
class Bullet;
class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	

	
private:
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	float _maxFPS;

	void initSystems();

	void initShaders();

	void initLevels();

	void gameLoop();

	void checkEndGame();

	void updateBullets(float deltaTime);
	void updateAgents(float deltaTime);

	void processInput();

	void drawGame();

	void drawHud();

	//add bloods to particle egine
	void addBlood(const glm::vec2& position, int numParticles);

	Engine::Window _window;

	Engine::GLSLProgram _textureProgram;

	Engine::InputManager _inputManager;

	Engine::Camera2D _camera;
	Engine::Camera2D _hudCamera;

	Engine::SpriteBatch _agentSpriteBatch;
	Engine::SpriteBatch _hudSpriteBatch;

	float _fps;
	int _counter;
	float _frameCounter;
	bool _defeat = false;

	int _numberOfZombiesKilled;
	int _numberOfHumansKilled;

	std::vector<Level*> _levels;

	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;

	Player* _player;

	std::vector<Gun*> _guns;

	Engine::SpriteFont* spriteFont;

	Engine::AudioEngine _audioEngine;

	Engine::ParticleEngine2D _particleEngine;
	Engine::ParticleBatch2D* _bloodParticleBatch;
};

