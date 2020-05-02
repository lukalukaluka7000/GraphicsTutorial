#include "Player.h"
#include<SDL/SDL.h>
#include<Engine/ResourceManager.h>
#include<glm/glm.hpp>
Player::Player() {

}
Player::~Player() {

}
void Player::init(b2World* world,
					const glm::vec2& position,
					const glm::vec2& drawdimensions,
					const glm::vec2& collisiondimensions, // capsule dimensions
					std::vector<Engine::Vertex::ColorRGBA8> color,
					Engine::TileSheet tileSheet
					/*glm::vec4 uvRectPlayer*/) {
	_tileSheetPlayer = tileSheet;
	_drawDimensions = drawdimensions;
	_collisiondimensions = collisiondimensions;
	_tileDims = glm::vec2(10.0f, 2.0f);
	_position = position;
	//_uvRectPlayer = ;// glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // default uvRect

	_tileSheetPlayer.init(_tileSheetPlayer._texture, _tileDims);
	_playerColor = color;
	_capsule.init(world, position, collisiondimensions, 1.0f, 0.1f, color /*, _texture, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), true*/);

}
//void Player::init(const glm::vec2& position,
//					const glm::vec2& drawdimensions,
//					std::vector<Engine::Vertex::ColorRGBA8> color,
//					Engine::TileSheet tileSheet) {
//	_tileSheetPlayer = tileSheet;
//	_drawDimensions = drawdimensions;
//	_tileDims = glm::vec2(10.0f, 2.0f);
//	_position = position;
//	_tileSheetPlayer.init(_tileSheetPlayer._texture, _tileDims);
//}

void Player::draw(Engine::SpriteBatch& spriteBatch) {
	//_collisionBox.draw(spriteBatch);
	Engine::Vertex::ColorRGBA8 color(255, 0, 255, 255);
	glm::vec4 destRect;
	b2Body* body = _capsule.getBody();
	destRect.x = body->GetPosition().x - _drawDimensions.x / 2.0f;
	destRect.y = body->GetPosition().y - /*_drawDimensions.y*/_collisiondimensions.y / 2.0f;
	destRect.z = _drawDimensions.x;
	destRect.w = _drawDimensions.y;


	int tileIndex;
	int numTiles;
	float animationSpeed = 0.2f;
	glm::vec2 velocity(body->GetLinearVelocity().x, body->GetLinearVelocity().y);

	if (_onGround) {
		if (_isPunching) {
			numTiles = 4;
			tileIndex = 1;
			if (_playerState != PLAYERSTATE::PUNCHING) {
				_playerState = PLAYERSTATE::PUNCHING;
				_animationTime = 0.0f;
			}
		}
		else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && _direction > 0) || (velocity.x < 0 && _direction < 0) )) {
			if (_playerState != PLAYERSTATE::RUNNING) {
				_playerState = PLAYERSTATE::RUNNING;
				_animationTime = 0.0f;
			}
			if (abs(velocity.x) < 5.0f) {
				animationSpeed = abs(velocity.x) * 0.025f;
				numTiles = 1;
				tileIndex = 14;
			}
			else {
				animationSpeed = abs(velocity.x) * 0.025f;
				numTiles = 6;
				tileIndex = 10;
			}
		}
		else {
			_playerState = PLAYERSTATE::STAYING;
			numTiles = 1;
			tileIndex = 0;
		}
	}
	else {
		//u zraku je
		if (_isPunching) {
			numTiles = 1;
			tileIndex = 18;
			animationSpeed *= 0.25;
			if (_playerState != PLAYERSTATE::PUNCHING) {
				_playerState = PLAYERSTATE::PUNCHING;
				_animationTime = 0.0f;
			}
		}
		else if (abs(velocity.x) > 10.0f) {
			_playerState = PLAYERSTATE::IN_AIR;
			numTiles = 1;
			tileIndex = 10;
		}
		else if (velocity.y > 0.0f) {
			_playerState = PLAYERSTATE::IN_AIR;
			numTiles = 1;
			tileIndex = 16;
		}
		else {
			_playerState = PLAYERSTATE::IN_AIR;
			numTiles = 1;
			tileIndex = 17;
		}
	}

	_animationTime += animationSpeed;

	if (_animationTime > numTiles) {
		_isPunching = false;
	}
	// apply animation linearno fino
	tileIndex = tileIndex + (int)_animationTime % numTiles;
	
	_uvRectPlayer = _tileSheetPlayer.getUV(tileIndex);
	if (_direction == -1) {
		_uvRectPlayer.x += _tileSheetPlayer._widthOfOneTile;
		_uvRectPlayer.z *= -1; 
	}

	spriteBatch.draw(_tileSheetPlayer._texture.id, 0.0f, destRect, _uvRectPlayer, /*color.getVectorWithSameColor(color)*/_playerColor, body->GetAngle());

}
void Player::drawDebug(Engine::DebugRenderer& debugRenderer, Engine::Vertex::ColorRGBA8 color) {
	_capsule.drawDebug(debugRenderer, color);
}

void Player::update(Engine::InputManager& inputManager) {
	b2Body* body = _capsule.getBody();
	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f));
		_direction = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f));
		_direction = 1;
	}
	else if (inputManager.isKeyDown(SDLK_SPACE)) {
		_isPunching = true;
	}
	else {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y));
	}

	float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x > MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	_onGround = false;
	//jumping - need to loop through all contact points
	for (b2ContactEdge* ce /*contact edge*/ = body->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* c = ce->contact; // c mi je i-ti contact
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold); // get manifold of contact

			// i asd cekiramo jeli ispod nas kontakt jer u protivnom nemos skakat
			bool below = false;
			_onGround = true;
			for (size_t i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - _collisiondimensions.y / 2.0f + _collisiondimensions.x / 2.0f + 0.01f) {
					below = true;
					break;
				}
			}
			if (below) {
				if (inputManager.isKeyPressed(SDLK_w)) {
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f));
					break;
				}
			}
		}
	}

}
void Player::destroy(b2World* world) {
	_capsule.destroy(world);
}