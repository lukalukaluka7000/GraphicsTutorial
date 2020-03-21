#pragma once
#pragma once
#include<glm/glm.hpp>
#include<Engine/ResourceManager.h>
#include<vector>
#include<Engine/Vertex.h>
#include<Engine/SpriteBatch.h>

#include"Gun.h"
class Gun;
class Human;
class Agent;
class Zombie;

const float BULLET_RADIUS = 6.0f;

class Bullet
{
public:
	Bullet(glm::vec2 pos, float speed, glm::vec2 direction);
	~Bullet();

	void updatePosition(float deltaTime);

	void draw(Engine::SpriteBatch& spriteBatch);

	bool collideWithWorld(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	glm::vec2 getPosition() { return _position; }
private:
	glm::vec2 _position;
	float _speed;
	glm::vec2 _direction;


};
