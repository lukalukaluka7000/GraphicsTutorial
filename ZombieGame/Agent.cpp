#include "Agent.h"
#include<Engine/GLTexture.h>
#include<Engine/ResourceManager.h>
#include"Level.h"
#include"Human.h"
#include"MainGame.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

class Human;

Agent::Agent() {
	//_color.reserve(4);
}
Agent::~Agent() {

}
glm::vec2 rotatePoint(glm::vec2 pos, float angle) {
	glm::vec2 newVec;
	newVec.x = pos.x * cos(angle) - pos.y * sin(angle);
	newVec.y = pos.x * sin(angle) + pos.y * cos(angle);
	/*newVec.x = pos.x * cos(angle) + pos.y * sin(angle);
	newVec.y = -pos.x * sin(angle) + pos.y * cos(angle);*/

	return newVec;
}
void Agent::draw(Engine::SpriteBatch& _spriteBatch) {
	
	Engine::GLTexture texture;
	//texture.id = Engine::ResourceManager::getTexture("./Textures/circle.png").id;
	glm::vec4 destRect(_position.x, _position.y, PLAYER_SIZE, PLAYER_SIZE);
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	//if (_color.back().b == 200) {
	//	const glm::vec2 right(1.0f, 0.0f);

	//	float angle = acos(glm::dot(right, _direction));

	//	//angle = angle * (180.0f / M_PI);
	//	if (_direction.y < 0.0f) angle = -angle;

	//	glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

	//	// Get points centered at origin
	//	glm::vec2 tl(-halfDims.x, halfDims.y);
	//	glm::vec2 bl(-halfDims.x, -halfDims.y);
	//	glm::vec2 br(halfDims.x, -halfDims.y);
	//	glm::vec2 tr(halfDims.x, halfDims.y);


	//	// Rotate the points
	//	tl = rotatePoint(tl, angle) + halfDims;
	//	bl = rotatePoint(bl, angle) + halfDims;
	//	br = rotatePoint(br, angle) + halfDims;
	//	tr = rotatePoint(tr, angle) + halfDims;
	//	std::cout << tl.x << " " << bl.x << " " << br.x << " " << tr.x << std::endl;
	//	std::cout << tl.y << " " << bl.y << " " << br.y << " " << tr.y << std::endl;
	//	std::cout << angle << std::endl;
	//	std::cout << std::endl;
	//}
	if (_color.back().b == 6) {
		std::cout << _direction.x << " " << _direction.y << std::endl;
		std::cout << std::endl;
	}
	_spriteBatch.draw(_textureID, 0.0f, destRect, uvRect, _color, _direction/*glm::vec2(1.0f,0.0f)*/);
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	
	std::vector<glm::vec2> collideTilePositions;
	
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);
	checkTilePosition(levelData, collideTilePositions, _position.x + SPRITE_SIZE, _position.y);
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + SPRITE_SIZE);
	checkTilePosition(levelData, collideTilePositions, _position.x + SPRITE_SIZE, _position.y + SPRITE_SIZE);

	// return true if there are any that are colliding, so we can invert direction so they dont slide on walls
	if (!collideTilePositions.empty()) {
		//do the collision for the ones that are colliding
		for (int i = 0; i < collideTilePositions.size(); i++) {
			collideWithTile(collideTilePositions[i]);
		}
		return true;
	}

	return false;
}
bool Agent::collideWithAgent(Agent* agent2) { //std::vector<Human*>& humans but i dont need to send vector because all for loops are in MainGame.cpp
	//check pair={this->agent, Agent*agent} against each other if they colliding 
	const float MIN_DISTANCE = AGENT_RADIUS + AGENT_RADIUS; // L

	//glm::vec2 centerAgentPosition = humans[indexOfCurrentAgent]->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerAgentPosition = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerSecondAgentPosition = agent2->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 firstArrowSecond = centerSecondAgentPosition - centerAgentPosition;

	float distance = glm::length(firstArrowSecond);
	
	float collisionDepth = MIN_DISTANCE - distance;
	//float xDepth = (MIN_DISTANCE - abs(distVec.x));
	//float yDepth = (MIN_DISTANCE - abs(distVec.y));

	if (distance < MIN_DISTANCE) { // L < 2R -> we got a collision
		
		// orientation * value = vector how much to push them both / 2
		glm::vec2 collisionDepthOrient = glm::normalize(firstArrowSecond) * collisionDepth;

		agent2->setPosition(agent2->getPosition() + collisionDepthOrient/2.0f);
		_position = _position - collisionDepthOrient / 2.0f;	
		return true;
	}
	return false;
}
	

void Agent::checkTilePosition(	const std::vector<std::string>& levelData,
								std::vector<glm::vec2>& collideTilePositions,
								float x,
								float y) {
	// Check four corners
	glm::vec2 cornerPos = glm::vec2(	floor(x / (float)SPRITE_SIZE),
										floor(y / (float)SPRITE_SIZE)) ;
	// is the tile collidable?
	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		/*collideTilePositions.push_back(glm::vec2(cornerPos.x * (float)SPRITE_SIZE + ((float)SPRITE_SIZE / 2.0f), 
												 cornerPos.y * (float)SPRITE_SIZE + ((float)SPRITE_SIZE / 2.0f) ) );*/
		collideTilePositions.push_back(cornerPos * (float)SPRITE_SIZE + glm::vec2((float)SPRITE_SIZE / 2.0f));
	}


}
//AABB
void Agent::collideWithTile(glm::vec2 tilePos) {

	const float AGENT_RADIUS = (float)PLAYER_SIZE / 2.0f;
	const float TILE_RADIUS = (float)SPRITE_SIZE / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPosition = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPosition - tilePos;
	//glm::vec2 distVec = _position - tilePos;


	//check x direction
	//depth je koliko je zarilo u drugi tile, sta je pozitivniji i veci to je vise unutra
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	/*if (xDepth > 0 || yDepth > 0) {
		if ( max(xDepth,0.0f) < max(yDepth, 0.0f)) {
			if (distVec.x > 0) {
				_position += glm::vec2(int(xDepth) - ceil(SPRITE_SIZE / 2) + 5, 0.0f);
			}
			else {
				_position -= glm::vec2(int(xDepth) - ceil(SPRITE_SIZE / 2) + 5, 0.0f);
			}
		}
		else {
			if (distVec.x > 0) {
				_position += glm::vec2(0.0f, (int)yDepth - ceil(SPRITE_SIZE / 2) + 5);
			}
			else {
				_position -= glm::vec2(0.0f, (int)yDepth - ceil(SPRITE_SIZE / 2) + 5);
			}
		}
	}*/
	if (xDepth > 0 && yDepth > 0) {
		if (max(xDepth, 0.0f) < max(yDepth, 0.0f)) {
			if (distVec.x < 0) {
				_position.x -= xDepth;
			}
			else {
				_position.x += xDepth;
			}
		}
		else {
			if (distVec.y < 0) {
				_position.y -= yDepth;
			}
			else {
				_position.y += yDepth;
			}
		}
	}
}