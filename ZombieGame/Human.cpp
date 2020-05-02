#define _USE_MATH_DEFINES
#include "Human.h"
#include<iostream>
#include<random>
#include<ctime>
#include<glm/gtx/rotate_vector.hpp>
#include<vector>
#include"Gun.h"

class Agent;

Human::Human() : _frames(0){
	
}
Human::~Human() {

}
void Human::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;
	_health = 100.0f;

	Engine::Vertex::ColorRGBA8 color(200, 0, 199, 255);
	_color = Engine::Vertex::ColorRGBA8::getVectorWithSameColor(color);

	static std::mt19937 randomEngine;
	//randomEngine.seed(time(nullptr));
	static std::uniform_real_distribution<float> direction(-5.0, 5.0);
	static std::uniform_int_distribution<int> rotateIndex(20, 50);

	_direction = glm::normalize(glm::vec2(direction(randomEngine), direction(randomEngine)));
	_rotateEveryFrame = rotateIndex(randomEngine);

	_textureID = Engine::ResourceManager::getTexture("./Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, std::vector<Gun*>& guns, float deltaTime) {
	
	/*glm::vec2 position_next = _position + _direction * _speed;
	if (abs(position_next.x-_position.x) <= 0.03f) {
		_direction = glm::vec2()
		return;
	}*/
	_position += _direction * _speed * deltaTime;
	
	if (_frames % _rotateEveryFrame == 0) { // rotate direction every rotateEveryFrame-th frame
		// create random generator
		static std::mt19937 randomEngine(time(nullptr));
		static std::uniform_real_distribution<float> rotateValue(-40.0f * (M_PI / 180.0f), 40.0f * (M_PI / 180.0f));
		_direction = glm::rotate(_direction, rotateValue(randomEngine));
	}

	_frames++;

	//true if agent(human) is currently beside the wall, so he can get out -> give him new direction
	bool at_wall = collideWithLevel(levelData);
	if (at_wall) {
		static std::mt19937 randomEngine(time(nullptr));
		static std::uniform_real_distribution<float> directionChange(-1.0f, 1.0f);
		//_direction = glm::vec2(-1.0f,-1.0f) * _direction;
		_direction = glm::normalize(glm::vec2(directionChange(randomEngine), directionChange(randomEngine)));
	}

}