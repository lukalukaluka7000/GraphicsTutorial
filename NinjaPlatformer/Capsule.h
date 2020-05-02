#pragma once
#include<Box2D/Box2D.h>
#include<glm/glm.hpp>
#include<vector>
#include <Engine\Vertex.h>
#include<Engine/GLTexture.h>
#include<Engine/DebugRenderer.h>
class Capsule
{
public:

	void init(b2World* world,
		const glm::vec2& position,
		const glm::vec2& dimensions,
		float density,
		float friction,
		std::vector<Engine::Vertex::ColorRGBA8> color);

	void drawDebug(Engine::DebugRenderer& debugRenderer, Engine::Vertex::ColorRGBA8 color);

	b2Body* getBody() const { return _body; }
	b2Fixture* getFixture(int i) const { return _fixtures[i]; }
	//const glm::vec2& getDimensions()const { return _dimensions; }

	void destroy(b2World* world);
private:
	b2Body* _body = nullptr;
	b2Fixture* _fixtures[3];
	glm::vec2 _dimensions;
	std::vector<Engine::Vertex::ColorRGBA8> _color;
	glm::vec4 _uvRect;
	bool _fixedRotation = true;

	
};

