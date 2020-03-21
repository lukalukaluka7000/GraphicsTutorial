#pragma once

#include<vector>
#include<string>

#include"Bullet.h"
#include<Engine/AudioEngine.h>
class Bullet;
class Human;
class Zombie;
const float NUM_GUNS = 3;
const float BULLET_SPEED = 15.0f;//PLAYER_SPEED * 2.0f
class Gun
{
public:
	Gun(std::string gunName, float fireRate, float damage, float spread, float bulletPerShot,Engine::SoundEffect fireEffect);
	~Gun();

	void fire( glm::vec2 direction, glm::vec2 position);

	float getFireRate() { return _fireRate; }
	float getBulletPerShot() { return _bulletsPerShot; }

	void updatePositionAndCheckGrid(const std::vector<std::string>& levelData, float deltaTime);
	std::vector<Bullet> _bullets;

	float getFrameCounterOfGun() { return _frameCounterofGun; }
	void setFrameCounterOfGun(float fr_cnt) { _frameCounterofGun = fr_cnt; }

	float getDamage() { return _damage; }
private:
	Engine::SoundEffect _fireEffect;
	int _currentGun;
	int _numberOfGuns;
	int _fireRate;
	float _damage;
	float _spread;
	float _bulletsPerShot;
	float _frameCounterofGun;

};

