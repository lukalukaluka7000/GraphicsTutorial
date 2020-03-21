#include "Bullet.h"
#include"Agent.h"
#include <ZombieGame\Level.h>
Bullet::Bullet(glm::vec2 pos, float speed, glm::vec2 direction) {
	_position = pos;
	_speed = speed;
	_direction = direction;
}
Bullet::~Bullet() {

}
void Bullet::updatePosition(float deltaTime) {
	_direction = glm::normalize(_direction);
	_position += _speed * _direction * deltaTime;
}
void Bullet::draw(Engine::SpriteBatch& spriteBatch) {

	glm::vec4 pos(_position.x, _position.y, BULLET_RADIUS*2, BULLET_RADIUS*2);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Engine::GLTexture texture = Engine::ResourceManager::getTexture("./Textures/circle.png");

	Engine::Vertex::ColorRGBA8 color(88, 88, 88, 255);
	std::vector<Engine::Vertex::ColorRGBA8>colors = Engine::Vertex::ColorRGBA8::getVectorWithSameColor(color);

	spriteBatch.draw(texture.id, 0.0f, pos, uv, colors);
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
	//this mi je jedan bullet
	glm::vec2 centerOfBullet(floor(this->getPosition().x + BULLET_RADIUS), floor(this->getPosition().y + BULLET_RADIUS));

	glm::vec2 gridConvert(floor(centerOfBullet.x / SPRITE_SIZE), floor(centerOfBullet.y / SPRITE_SIZE));

	if (gridConvert.x <= 0 || gridConvert.y <= 0 || gridConvert.x >= levelData[0].size()-1 || gridConvert.y >= levelData.size()-1) {
		return true;
	}
	if (levelData[gridConvert.y][gridConvert.x] != '.') 
		return true;
	return false;

}
bool Bullet::collideWithAgent(Agent* agent2) {
	//check pair={this->agent, Agent*agent} against each other if they colliding 
	const float MIN_DISTANCE = BULLET_RADIUS + AGENT_RADIUS; // L
	
	//glm::vec2 centerAgentPosition = humans[indexOfCurrentAgent]->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerAgentPosition = this->getPosition() + glm::vec2(BULLET_RADIUS);
	glm::vec2 centerSecondAgentPosition = agent2->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 firstArrowSecond = centerSecondAgentPosition - centerAgentPosition;

	float distance = glm::length(firstArrowSecond);

	//float xDepth = (MIN_DISTANCE - abs(distVec.x));
	//float yDepth = (MIN_DISTANCE - abs(distVec.y));

	if (distance < MIN_DISTANCE) { // L < 2R -> we got a collision
		return true;
	}
	return false;
}
