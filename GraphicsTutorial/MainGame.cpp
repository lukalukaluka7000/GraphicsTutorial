#include "MainGame.h"
#include <iostream>
#include <string>
#include<Engine/Engine.h>
#include<Engine/Sprite.h>
#include<Engine/EngineErrors.h>
#include<Engine/ImageLoader.h>
#include<Engine/ResourceManager.h>
#include<Engine/Vertex.h>



MainGame::MainGame()
{
	_screenWidth = 500;
	_screenHeight = 500;
	_gameState = GameState::PLAY; //:: scope
	_time = 0.0f;
	_maxFPS = 1000.0f;
	_camera.init(_screenWidth, _screenHeight);
	_counter = 0;
}


MainGame::~MainGame()
{
}


void MainGame::run() {
	initSystems();

	
	/*//_sprite.init(-0.7f, -0.7f, 1.2f, 1.2f);
	_sprites.push_back(new Engine::Sprite());
	//_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "./Textures/Dudes/telescope_01.png");//NDC
	_sprites.back()->init(-_screenWidth / 2, -_screenHeight / 2, _screenWidth/2, _screenHeight/2, "./Textures/Dudes/telescope_01.png"); //WorldCoord

	_sprites.push_back(new Engine::Sprite());
	//_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "./Textures/Dudes/telescope_01.png");
	_sprites.back()->init(0.0f, 0.0f, _screenWidth / 2, _screenHeight / 2, "./Textures/Dudes/store_shoppeKeeper.png");*/
	

	//_playerTexture = ImageLoader::loadPNG("./Textures/Dudes/store_shoppeKeeper.png");
	//now use the texture
	
	gameLoop();
}

void MainGame::initSystems() {
	Engine::init();

	_window.initWindow("Game Engine", _screenWidth, _screenHeight, Engine::FULLSCREEN);

	//initShaders called from inside of initSystems
	initShaders(); //VIDEO 10  

	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
}
void MainGame::initShaders() {
	std::cout << "Usa u initShaders" << std::endl;

	_colorProgram.compileShaders("./Shaders/colorShading.vert", "./Shaders/colorShading.frag");
	std::cout << "1" << std::endl;
	_colorProgram.addAttribute("vertexPosition");
	std::cout << "2" << std::endl;
	_colorProgram.addAttribute("vertexColor");
	std::cout << "3" << std::endl;
	_colorProgram.addAttribute("vertexUV");
	std::cout << "4" << std::endl;

	_colorProgram.linkShaders();
	std::cout << "5" << std::endl;


}


void MainGame::gameLoop() {
 	while (_gameState != GameState::EXIT) {
		
		// start of frame
		float startTicks = _fpsLimiter.	begin();

		processInput();

		_time += 0.01;

		for (int i = 0; i < _bullets.size(); i++) {
			_bullets[i].update();
			if (_bullets[i]._lifeTime == 0) {
				//nismo mi ovdi swapali, nego postavili itoga da je onaj zadnji i sad brisemo duplikata koji je na zadnjem mistu i na i tom mistu
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
		}
		_camera.update();

		drawGame();

		_fps = _fpsLimiter.end();
		if (_counter % 1000 == 0) {
			std::cout << _fps << std::endl;
		}
		_counter++;
	}
}

void MainGame::processInput() {
 //use sdl for this function
	//returning 1 if true zero is false 
	SDL_Event evnt;
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.01f;
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
	if(_inputManager.isKeyPressed(SDL_BUTTON_RIGHT))
		std::cout << "JES" << std::endl;
	
	if (_inputManager.isKeyPressed(SDLK_w))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	if (_inputManager.isKeyPressed(SDLK_s))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	if (_inputManager.isKeyPressed(SDLK_a))
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	if (_inputManager.isKeyPressed(SDLK_d))
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	if (_inputManager.isKeyPressed(SDLK_r)) {
		_camera.setPosition(glm::vec2(0.0f, 0.0f));
		_camera.setScale(1.0f);
	}
	if (_inputManager.isKeyPressed(SDLK_q))
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	if (_inputManager.isKeyPressed(SDLK_e))
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mousecord = _inputManager.getMouseCoords();
		mousecord = _camera.ScreenToScene(mousecord);
		if (_counter % 6 == 0) {
			_bullets.emplace_back(glm::vec2(0.0f), 1.0f, mousecord, 200);
			std::cout << mousecord.x << " " << mousecord.y << std::endl;
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
	
	
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);


	//GLint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time); //send variable to gpu

	GLint projectionMatrixLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	/*for (int i = 0; i < _sprites.size(); i++) {
		_sprites[i]->draw();
	}*/
	_spriteBatch.begin();

	//glm::vec4 pos(-_screenWidth / 2, -_screenHeight / 2, _screenWidth/2, _screenHeight/2);
	//glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f); // store_shoppeKeeper.png
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Engine::GLTexture texture = Engine::ResourceManager::getTexture("./Textures/Dudes/store_shoppeKeeper.png");

	std::vector<Engine::Vertex::ColorRGBA8> colors;
	Engine::Vertex::ColorRGBA8 color;
	color.r = 0;
	color.g = 255;
	color.b = 0;
	color.a = 255;
	colors.push_back(color);
	
	color.r = 0;
	color.g = 0;
	color.b = 255;
	color.a = 255;
	colors.push_back(color);

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	colors.push_back(color);

	color.r = 0;
	color.g = 255;
	color.b = 0;
	color.a = 255;
	colors.push_back(color);

	_spriteBatch.draw(texture.id, 0.0f, pos, uv, colors);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	//glm::vec4 pos2(0, 0, _screenWidth/2, _screenHeight/2);
	//glm::vec4 uv2(0.0f, 0.0f, 1.0f, 1.0f);
	//_spriteBatch.draw(texture.id, 0.0f, pos2, uv2, colors);

	_spriteBatch.end();
	_spriteBatch.renderBatch();


	glBindTexture(GL_TEXTURE_2D, 0);

	
	_colorProgram.unuse();


	//SDL_GL_SwapWindow(_window);
	_window.swapBuffer();

	glClearColor(0.0, 0.0, 1.0, 1.0);

	//initShaders();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


