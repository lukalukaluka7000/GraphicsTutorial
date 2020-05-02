#pragma once
#include"Box.h"
#include<Engine/InputManager.h>
#include<Engine/DebugRenderer.h>
#include"Capsule.h"
#include<Engine/GLTexture.h>
#include<Engine/TileSheet.h>
enum class PLAYERSTATE {
	RUNNING,
	IN_AIR,
	PUNCHING,
	STAYING
};
class Player
{
public:
	Player();
	~Player();

	void init(b2World* world,
				const glm::vec2& position,
				const glm::vec2& drawdimensions,
				const glm::vec2& collisiondimensions,
				std::vector<Engine::Vertex::ColorRGBA8> color,
				Engine::TileSheet tileSheet
				/*glm::vec4 uvRectPlayer*/);
	// INIT for static World
	/*void init(const glm::vec2& position,
		const glm::vec2& drawdimensions,
		std::vector<Engine::Vertex::ColorRGBA8> color,
		Engine::TileSheet tileSheet);*/
	void draw(Engine::SpriteBatch& spriteBatch);
	void drawDebug(Engine::DebugRenderer& debugRenderer, Engine::Vertex::ColorRGBA8 color);

	void update(Engine::InputManager& inputManager);

	const Capsule& getCapsule() const { return _capsule; }
	const glm::vec2& getPosition() const { return _position; }
	const glm::vec2& getDrawDims() const { return _drawDimensions; }
	const std::vector<Engine::Vertex::ColorRGBA8> getColorVector() const { return _playerColor; }
	void destroy(b2World* world);
private:
	glm::vec2 _position;
	glm::vec2 _drawDimensions;
	glm::vec2 _collisiondimensions;
	Capsule _capsule;

	/*Engine::GLTexture _texture;*/
	Engine::TileSheet _tileSheetPlayer;
	glm::vec2 _dimensions;
	glm::vec4 _uvRectPlayer;
	glm::vec2 _tileDims;
	
	bool _onGround = false;
	PLAYERSTATE _playerState = PLAYERSTATE::IN_AIR;
	int _direction = 1;
	float _animationTime;
	bool _isPunching = false;

	std::vector<Engine::Vertex::ColorRGBA8> _playerColor;
};

