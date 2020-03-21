#define _USE_MATH_DEFINES
#include "Zombie.h"
#include<ctime>
#include<random>
#include<glm/gtx/rotate_vector.hpp>
#include"Human.h"
#include"MainGame.h"
class Human;

const float PLAYER_IN_VICINITY = PLAYER_SIZE * 10.0f; // player in vicinity of 10 times their size = CHASE HIM

Zombie::Zombie() {  
	
}
Zombie::~Zombie() {

}
void Zombie::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;
	_health = 150.0f;

	Engine::Vertex::ColorRGBA8 color(0, 180, 6, 255);
	_color = Engine::Vertex::ColorRGBA8::getVectorWithSameColor(color);

	static std::mt19937 randomEngine;
	//randomEngine.seed(time(nullptr));
	static std::uniform_real_distribution<float> direction(-5.0, 5.0);
	static std::uniform_int_distribution<int> rotateIndex(20, 50);

	_direction = glm::normalize(glm::vec2(direction(randomEngine), direction(randomEngine)));
	_rotateEveryFrame = rotateIndex(randomEngine);

	_textureID = Engine::ResourceManager::getTexture("./Textures/zombie.png").id;
}
int Zombie::getNearestHumanIndex(std::vector<Human*>& humans) {
	float cloesesDistance = 999999.0f; float length; int indexOfClosestHuman = 0;
	for (int i = 1; i < humans.size(); i++) {
		glm::vec2 zombieArrowAgent = humans[i]->getPosition() - _position;
		length = glm::length(zombieArrowAgent);
		if (length < cloesesDistance) {
			cloesesDistance = length;
			indexOfClosestHuman = i;
		}
	}
	return indexOfClosestHuman;
}
void Zombie::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, std::vector<Gun*>& guns, float deltaTime) {

	//Top priority for zombie is to chase PLAYER
	glm::vec2 zombieArrowAgent = humans[0]->getPosition() - _position;
	if (glm::length(zombieArrowAgent) <= PLAYER_IN_VICINITY) {
		_direction = _speed * glm::normalize(zombieArrowAgent);
	}
	else {
		_direction = glm::normalize(humans[getNearestHumanIndex(humans)]->getPosition() - _position);
	}
	

	_position += _direction * _speed * deltaTime;

	if (_frames % _rotateEveryFrame == 0) { // rotate direction every rotateEveryFrame-th frame
		// create random generator
		static std::mt19937 randomEngine(time(nullptr));
		static std::uniform_real_distribution<float> rotateValue(-40.0f * (M_PI / 180.0f), 40.0f * (M_PI / 180.0f));
		_direction = glm::rotate(_direction, rotateValue(randomEngine));
	}

	_frames++;

	//true if agent(zombie) is currently beside the wall, so he can get out -> give him new direction
	bool at_wall = collideWithLevel(levelData);
	if (at_wall) {
		static std::mt19937 randomEngine(time(nullptr));
		static std::uniform_real_distribution<float> directionChange(-1.0f, 1.0f);
		//_direction = glm::vec2(-1.0f,-1.0f) * _direction;
		_direction = glm::normalize(glm::vec2(directionChange(randomEngine), directionChange(randomEngine)));
	}
	_direction = glm::normalize(_direction);
}
