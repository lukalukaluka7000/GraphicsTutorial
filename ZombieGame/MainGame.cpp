#include "MainGame.h"

#include<Engine/SpriteBatch.h>
#include<iostream>
#include<random>
#include<ctime>

#include<glm/glm.hpp>
#include"Zombie.h"
#include<SDL/SDL_mixer.h>

#include<glm/gtx/rotate_vector.hpp>
#include<Engine/Vertex.h>

const float HUMAN_SPEED = 0.8f;
const float PLAYER_SPEED = 6.0f;
const float ZOMBIE_SPEED = 1.2f;

MainGame::MainGame() :	
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_maxFPS(120.0f), 
	_fps(0.0f), 
	_player(nullptr),
	_counter(0),
	_frameCounter(0.0f),
	_numberOfZombiesKilled(0),
	_numberOfHumansKilled(0)
{

}


MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
	for (int i = 0; i < _humans.size(); i++) {
		delete _humans[i];
	}
	for (int i = 0; i < _zombies.size(); i++) {
		delete _zombies[i];
	}
	for (int i = 0; i < _guns.size() ; i++) {
		delete _guns[i];
	}
}


void MainGame::run() {
	initSystems();

	
	Engine::Music music = _audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1); // loop forever with -1

	gameLoop();


}
//void updateBloodParticle(Engine::Particle2D& p, float deltaTime) {
//	p.position += p.velocity * deltaTime;
//	p.color.a = GLubyte(p.life * 255.0f);
//}
void MainGame::initSystems() {
	Engine::init();
	
	_audioEngine.init();

	_window.initWindow("Zombie Game", _screenWidth, _screenHeight, Engine::FULLSCREEN);

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();

	_agentSpriteBatch.init();
	_hudSpriteBatch.init();

	spriteFont = new Engine::SpriteFont("Fonts/chintzy.ttf", 64);

	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);
	_hudCamera.setPosition(glm::vec2(_screenWidth / 2.0f, _screenHeight / 2.0f));

	_bloodParticleBatch = new Engine::ParticleBatch2D;
	//_bloodParticleBatch->init(	1000, 
	//							0.05f, 
	//							Engine::ResourceManager::getTexture("./Textures/particle.png"),
	//							updateBloodParticle
	//						);
	//INSTEAD LAMBDA = INLINE DECLARATION
	_bloodParticleBatch->init(	1000, 
								0.05f, 
								Engine::ResourceManager::getTexture("./Textures/particle.png"),
								[](Engine::Particle2D& p, float deltaTime) {
										p.position += p.velocity * deltaTime;
										p.color.a = GLubyte(p.life * 255.0f);
								}
							);

	_particleEngine.addParticleBatch(_bloodParticleBatch);

	initLevels();

}
void MainGame::initShaders() {
	_textureProgram.compileShaders("./Shaders/colorShading.vert", "./Shaders/colorShading.frag");
	std::cout << "1" << std::endl;
	_textureProgram.addAttribute("vertexPosition");
	std::cout << "2" << std::endl;
	_textureProgram.addAttribute("vertexColor");
	std::cout << "3" << std::endl;
	_textureProgram.addAttribute("vertexUV");
	std::cout << "4" << std::endl;

	_textureProgram.linkShaders();
	std::cout << "5" << std::endl;


}

void MainGame::initLevels() {
	_levels.push_back(new Level("./Levels/level1.txt"));
	_levels.back()->init();

	_player = new Player();

	//find out what is dependency injection whit this input manager and camera
	_player->init(PLAYER_SPEED, _levels.back()->getStartPlayerPosition(), &_inputManager, &_camera);

	_humans.push_back(_player);

	//std::string gunName, float fireRate, float damage, float spread, float bulletPerShot
	_guns.push_back(new Gun("Revolver", 20, 25.0f, 5.0f, 1.0f, _audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_guns.push_back(new Gun("Shotgun", 20, 60.0f, 30.0f, 6.0f, _audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_guns.push_back(new Gun("Negev", 4, 15.0f, 10.0f, 1.0f, _audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
	

	// create random generator
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randXpos(1, _levels.back()->getWidth() - 2);
	std::uniform_int_distribution<int> randYpos(1, _levels.back()->getHeight() - 2);

	// spawn humans
	for (int i = 0; i < _levels.back()->getNumberHumans(); i++) {
		_humans.push_back(new Human());

		glm::vec2 randPos(randXpos(randomEngine) * SPRITE_SIZE, randYpos(randomEngine) * SPRITE_SIZE);
		_humans.back()->init(HUMAN_SPEED, randPos);
	}


	// spawn initial zombies
	std::vector<glm::vec2> zombiePositions = _levels.back()->getStartZombiePositions();
	/*std::cout << "aaaaaaaa" << _levels.back()->getNumberZombies() << std::endl;
	for (int i = 0; i < _levels.back()->getNumberZombies(); i++) {
		std::cout << zombiePositions[i].x << " " << zombiePositions[i].y << std::endl;
	}*/
	for (int i = 0; i < _levels.back()->getNumberZombies(); i++) {
		_zombies.push_back(new Zombie());
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}
	
	
}

void MainGame::gameLoop() {
	Engine::FPSLimiter _fpsLimiter;
	_fpsLimiter.init(_maxFPS);

	_camera.setScale(_camera.getScale() / 2.0f);

	//maximum steps we should simulate so we dont get spiral of death
	const int MAX_PHYSICS_STEPS = 6;

	const float DESIRED_FPS = 100.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	
	const float MAX_DELTA_TIME = 1.0f; // max step in Semi fixed partitioning, red vertical lines
	float previousTicks = SDL_GetTicks();

	while (_gameState != GameState::EXIT) {
		float startTicks = _fpsLimiter.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = SDL_GetTicks() - previousTicks;
		previousTicks = newTicks;
		float verticalStep = frameTime / DESIRED_FRAMETIME;

		_inputManager.update();

		processInput();
		

		int i = 0;
		// semi fixed update
		while (verticalStep >= 0.0f && i < MAX_PHYSICS_STEPS) {
			//120 FPS=>manji ce bit verticalStep (0.5) pola koraka, ako iammo sporije npr 30 fpsa onda ce bit step velicine 1 NICE
			float deltaTime = std::min(verticalStep, MAX_DELTA_TIME);
			updateBullets(deltaTime);

			
			// NE OVO, NEGO updateAgents() zato i koristim nasljedivanje
			//_player->update();
			updateAgents(deltaTime);
			_particleEngine.update(deltaTime);

			verticalStep -= deltaTime;
			i++;
		}

		

		checkEndGame();

		_camera.setPosition(_player->getPlayerPosition());
		_camera.update();
		_hudCamera.update();

		drawGame();

		_fps = _fpsLimiter.end();
		if(_counter%40 == 0)
			std::cout << _fps << std::endl;


		_counter++;
	}
}
void MainGame::checkEndGame() {
	if (_defeat) {
		std::cout << "Oh noo... Zombie contaminated you and you are no longer able to move. " << std::endl << "***END OF GAME***" << std::endl;
		Engine::fatalError("");
	}
	if (_zombies.empty()) {
		std::printf("You WON, you WON, you WON. You saved the world from zombies...\n");
		std::printf("***STATS***\n");
		std::printf("Number of zombies killed: %d\n", _numberOfZombiesKilled);
		std::printf("Number of humans killed: %d\n", _numberOfHumansKilled);
		float percentagePeopleSaved = ((float)(_humans.size() - 1) / (float)_levels.back()->getNumberHumans()) * 100.0f;
		std::printf("You saved %d/%d people in this map (%.2f %%)\n", _humans.size() - 1, _levels.back()->getNumberHumans(), percentagePeopleSaved);
		if (percentagePeopleSaved > 50.0f)
			std::printf("***WELL DONE***\n");
		else
			std::printf("You could have done better without losing all those poor humans!\n");
		Engine::fatalError("");
	}
}
void MainGame::updateBullets(float deltaTime) { // je zapravo updateGuns()
	bool collidedWithAgent = false;
	bool hasCollidedWithHuman = false;

	// change position anc check collision with grid if there are bullets of that gun in the scene
	for (int i = 0; i < _guns.size(); i++) {
		if(!_guns[i]->_bullets.empty())
			_guns[i]->updatePositionAndCheckGrid(_levels.back()->getLevelData(), deltaTime);
	}

	//index of gun player is holding
	int indexOfGun = _player->currentGunIndex;

	for (int i = 0; i < _guns[indexOfGun]->_bullets.size(); i++) {
		hasCollidedWithHuman = false;
		for (int j = 1; j < _humans.size();) {
			collidedWithAgent = _guns[indexOfGun]->_bullets[i].collideWithAgent(_humans[j]);
			if (collidedWithAgent == true) {

				addBlood(_guns[indexOfGun]->_bullets[i].getPosition() , 15);

				_guns[indexOfGun]->_bullets[i] = _guns[indexOfGun]->_bullets.back();
				_guns[indexOfGun]->_bullets.pop_back();
				collidedWithAgent = false;
				i = i - 1; // za ne preskocit jer njega jos nismo provjerili!

				_humans[j]->setHealth(_humans[j]->getHealth() - _guns[indexOfGun]->getDamage());
				if (_humans[j]->getHealth() <= 0.0f) {
					delete _humans[j];
					_humans[j] = _humans.back();
					_humans.pop_back();
					_numberOfHumansKilled++;
				}
				else {
					// ONLY WHEN WE DID NOT DELETED FROM VECTOR - COOL TRICK
					j++;
				}
				hasCollidedWithHuman = true;
				break;
			}
			else {
				// ONLY WHEN WE DID NOT DELETED FROM VECTOR - COOL TRICK
				j++;
			}
		}
		if (!hasCollidedWithHuman) {
			for (int j = 0; j < _zombies.size();) {
				collidedWithAgent = _guns[indexOfGun]->_bullets[i].collideWithAgent(_zombies[j]);
				if (collidedWithAgent == true) {

					addBlood(_guns[indexOfGun]->_bullets[i].getPosition(), 15);

					_guns[indexOfGun]->_bullets[i] = _guns[indexOfGun]->_bullets.back();
					_guns[indexOfGun]->_bullets.pop_back();
					collidedWithAgent = false;
					i = i - 1;

					_zombies[j]->setHealth(_zombies[j]->getHealth() - _guns[indexOfGun]->getDamage());
					if (_zombies[j]->getHealth() <= 0) {
						delete _zombies[j];
						_zombies[j] = _zombies.back();
						_zombies.pop_back();
						_numberOfZombiesKilled++;
					}
					else {
						j++;
					}
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}
void MainGame::updateAgents(float deltaTime) {

	//update humans
	for (int i = 0; i < _humans.size(); i++) {
		// collide with Level and DO THE UPDATE POSITION of humans
		_humans[i]->update(_levels.back()->getLevelData(), _humans, _zombies, _guns, deltaTime);
		for (int j = i + 1; j < _humans.size(); j++) {
			// collide HUman with Human
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}

	bool collidedWithHuman = false;
	//update zombies
	for (int i = 0; i < _zombies.size(); i++) {
		// collide with Level
		_zombies[i]->update(_levels.back()->getLevelData(), _humans, _zombies, _guns, deltaTime);
		
		for (int j = i + 1; j < _zombies.size(); j++) {
			// collide zombie with zombie
			_zombies[i]->collideWithAgent(_zombies[j]);
		}

		for (int j = 0; j < _humans.size(); j++) {
			// collide zombie with player and humans
			collidedWithHuman = _zombies[i]->collideWithAgent(_humans[j]);
			if (collidedWithHuman == true) {
				//postavljam na j-to misto zadnjega
				// i tog zadnjega unistavam da ga vise nema
				glm::vec2 deadHumanPosition = _humans[j]->getPosition();
				_humans[j] = _humans.back();
				_humans.pop_back();

				_zombies.push_back(new Zombie());
				_zombies.back()->init(ZOMBIE_SPEED, deadHumanPosition);
				
				// if collided with player GAME OVER
				if(j==0)
					std::cout << "Oh noo... Zombie contaminated you and you are no longer human. " << std::endl << "***END OF GAME***" << std::endl;
			}
			collidedWithHuman = false;
		}
	}


}

void MainGame::processInput() {
	//use sdl for this function
	   //returning 1 if true zero is false 
	//const float CAMERA_SPEED = 2.0f;
	//const float SCALE_SPEED = 0.01f;

	SDL_Event evnt;

	//passing by reference this up pointer NAJS
	while (SDL_PollEvent(&evnt) == true) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.keyPressed(evnt.button.button);
			//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.keyRelease(evnt.button.button);
			break;
		case SDL_KEYDOWN:
			_inputManager.keyPressed(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.keyRelease(evnt.key.keysym.sym);
			break;
		}
	
	}

	
}

void MainGame::drawGame() {
	//what depth opengl should clear to
	glClearDepth(1.0);
	//clear the screen, clean buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint projectionMatrixLocation = _textureProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	// draw level // ovaj level koristi svoj spriteBatch, mi tribamo napravit novoga za _humans.draw(agentSpritebatch)
	_levels.back()->draw();

	// draw player (humans)
	_agentSpriteBatch.begin();

	glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	for (int i = 0; i < _humans.size(); i++) {
		if (_camera.isBoxInView(_humans[i]->getPosition(), agentDims))
			_humans[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDims))
			_zombies[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _guns.size(); i++) {
		for (int j = 0; j < _guns[i]->_bullets.size(); j++) {
			_guns[i]->_bullets[j].draw(_agentSpriteBatch);
		}
	}

	_agentSpriteBatch.end();

	_agentSpriteBatch.renderBatch();
	
	//render the particles
	_particleEngine.draw(&_agentSpriteBatch);

	drawHud();

	_textureProgram.unuse();

	//SDL_GL_SwapWindow(_window);
	_window.swapBuffer();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
void MainGame::drawHud() {
	char buffer[256];
	_hudSpriteBatch.init();

	glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hudSpriteBatch.begin();

	sprintf_s(buffer, "Number of humans: %d", _humans.size());
	spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 0), glm::vec2(0.5), 0.0f, Engine::Vertex::ColorRGBA8(0, 0, 0, 255));

	sprintf_s(buffer, "Number of zombies: %d", _zombies.size());
	spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 36), glm::vec2(0.5), 0.0f, Engine::Vertex::ColorRGBA8(0, 0, 0, 255));

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}
void MainGame::addBlood(const glm::vec2& position, int numParticles) {
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);


	glm::vec2 vel(2.0f, 0.0f);
	Engine::Vertex::ColorRGBA8 color(180, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), color, 20.0f);
	}
}
