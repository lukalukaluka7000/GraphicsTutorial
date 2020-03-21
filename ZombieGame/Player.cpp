#include "Player.h"

#include<Engine/InputManager.h>

#include<SDL/SDL.h>

#include"MainGame.h"

#include<Engine/GLTexture.h>
#include<Engine/ResourceManager.h>
#include"Level.h"
#include"MainGame.h"

Player::Player() {

}
Player::~Player() {

}
//find out what is dependency injection whit this input manager and camera
void Player::init(float speed, glm::vec2 pos, Engine::InputManager* inputManager, Engine::Camera2D* camera) {

	_speed = speed;
	_position = pos;
	_inputManager = inputManager;
	_camera = camera;
	currentGunIndex = 0;

	Engine::Vertex::ColorRGBA8 color(0, 0, 200, 255);
	_color = Engine::Vertex::ColorRGBA8::getVectorWithSameColor(color);

	_textureID = Engine::ResourceManager::getTexture("./Textures/player.png").id;
}
void Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, std::vector<Gun*>& guns, float deltaTime) {

	if (_inputManager->isKeyDown(SDLK_w))
		_position.y += _speed * deltaTime;
	if (_inputManager->isKeyDown(SDLK_s))
		_position.y -= _speed * deltaTime;
	if (_inputManager->isKeyDown(SDLK_a))
		_position.x -= _speed * deltaTime;
	if (_inputManager->isKeyDown(SDLK_d))
		_position.x += _speed * deltaTime;

	
	// current gun that player is holding
	int indexGun = currentGunIndex;

	guns[indexGun]->setFrameCounterOfGun(guns[indexGun]->getFrameCounterOfGun() + 1.0 * deltaTime);
	
	
	/*glm::vec2 mousecord = _inputManager->getMouseCoords();
	mousecord = _camera->ScreenToScene(mousecord);*/

	glm::vec2 mousecord = _inputManager->getMouseCoords();
	mousecord = _camera->ScreenToScene(mousecord);

	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
	_direction = glm::normalize(mousecord - centerPosition);
	//std::cout << _direction.x << " " << _direction.y << std::endl;
	if (_inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		
		// Counter of gun is greater than its firerate, after every Shot it is set to zero
		// meet the Firerate of gun
		if (guns[indexGun]->getFrameCounterOfGun() >= guns[indexGun]->getFireRate()) {

			guns[indexGun]->fire(_direction/*mousecord - this->getPlayerPosition()*/, centerPosition/*this->getPlayerPosition() + glm::vec2(AGENT_RADIUS)*/);
			
			guns[indexGun]->setFrameCounterOfGun(0.0f);
		}
	}
	if (_inputManager->isKeyDown(SDLK_1)) {
		currentGunIndex = 0;
	}
	if (_inputManager->isKeyDown(SDLK_2)) {
		currentGunIndex = 1;
	}
	if (_inputManager->isKeyDown(SDLK_3)) {
		currentGunIndex = 2;
	}
	//std::cout << _position.x << " " << _position.y << std::endl;

	collideWithLevel(levelData);
}
glm::vec2 Player::getPlayerPosition() {
	return glm::vec2(_position.x, _position.y);
}