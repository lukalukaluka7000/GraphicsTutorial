#pragma once

#include<glm/glm.hpp>
#include<Engine/SpriteBatch.h>
#include<iostream>

class Human;
class Zombie;
class Gun;

const int PLAYER_SIZE = 55;
const float AGENT_RADIUS = PLAYER_SIZE / 2.0f;


class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData, 
						std::vector<Human*>& humans, 
						std::vector<Zombie*>& zombies, 
						std::vector<Gun*>& guns, 
						float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent *agent);

	glm::vec2 getPosition() { return _position; }
	void setPosition(glm::vec2 newPosition) { _position = newPosition; }

	void draw(Engine::SpriteBatch& _spriteBatch);

	void setHealth(float health) { _health = health; }
	float getHealth() { return _health; }

	float getColor() { return _color.back().b; }
	
protected:
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePos);
	glm::vec2 _position;
	float _speed;
	float _health;
	std::vector<Engine::Vertex::ColorRGBA8> _color;
	
	GLuint _textureID;

	glm::vec2 _direction = glm::vec2(1.0f, 0.0f);
};

