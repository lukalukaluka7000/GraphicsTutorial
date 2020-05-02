#pragma once
#include<Box2D/Box2D.h>
#include<glm/glm.hpp>
#include<Engine/Vertex.h>
#include<Engine/SpriteBatch.h>
#include<Engine/GLTexture.h>
class Box
{
public:
	Box();
	~Box();

	void init(b2World* world,
				const glm::vec2& position,
				const glm::vec2& dimensions, 
				std::vector<Engine::Vertex::ColorRGBA8> color,
				Engine::GLTexture texture,
				glm::vec4 uvRect = glm::vec4(0.0f,0.0f,1.0f,1.0f),
				bool fixedRot = false);

	void draw(Engine::SpriteBatch& spriteBatch);

	b2Body* getBody() const { return _body; }

	b2Fixture* getFixture() const { return _fixture; }

	const glm::vec2& getDimensions()const { return _dimensions; }

	std::vector<Engine::Vertex::ColorRGBA8> getColor() { return _color; }

	Engine::GLTexture getTexture() { return _texture; }

	void destroy(b2World* world);

	bool isDynamic() const { return _body->GetType() == b2_dynamicBody; }

	//test point is in box
	bool testPoint(float x, float y)const { return _fixture->TestPoint(b2Vec2(x, y)); }

	glm::vec2 getPosition() { return _position; }
private:
	b2Body* _body = nullptr;
	b2Fixture* _fixture = nullptr;
	glm::vec2 _dimensions;
	std::vector<Engine::Vertex::ColorRGBA8> _color;
	Engine::GLTexture _texture;
	glm::vec4 _uvRect;
	glm::vec2 _position;
	bool _fixedRotation = false;
};

