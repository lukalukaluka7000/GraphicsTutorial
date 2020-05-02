#define M_PI           3.14159265358979323846f
#include "Gun.h"
#include<ctime>
#include<random>
#include<glm/gtx/rotate_vector.hpp>
Gun::Gun(std::string gunName, float fireRate, float damage, float spread, float bulletPerShot, Engine::SoundEffect fireEffect) {
	_fireEffect = fireEffect;
	_fireRate = fireRate;
	_damage = damage;
	_spread = spread;
	_bulletsPerShot = bulletPerShot;
	_frameCounterofGun = 0;
	_currentGun = 0;
}
Gun::~Gun() {

}

void Gun::fire( glm::vec2 direction, glm::vec2 position) {
	static std::mt19937 randomEngine(time(nullptr));
	//std::cout << -_spread * (M_PI / 180.0f) << " " << _spread * (M_PI / 180.0f) << std::endl;
	//static std::uniform_real_distribution<float> randRotate(-_spread * (M_PI / 180.0f), _spread * (M_PI / 180.0f));
	static std::uniform_real_distribution<float> randRotate(-_spread , _spread );

	_fireEffect.play(); 

	//watch this like = GUN PRODUCES BULLETS TO SHOT
	for (int i = 0; i < _bulletsPerShot; i++) {
		// Add a new bullet
		float rand = randRotate(randomEngine) * (M_PI / 180.0f);
		//std::cout << rand << std::endl;
		_bullets.emplace_back(position, BULLET_SPEED, glm::rotate(direction, rand));

	}
}

void Gun::updatePositionAndCheckGrid(const std::vector<std::string>& levelData, float deltaTime) { // gun ce znat kad metak probije nekoga ali nezna nista vise
	//watch this like = GUN ACTUALY CONTROLS (updates) FLOW OF BULLETS
	bool collided = false;
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].updatePosition(deltaTime);

		collided = _bullets[i].collideWithWorld(levelData);

		if (collided) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
			collided = false;
		}

		
	}

	// After a certain number of frames has passed we fire our gun
	
}