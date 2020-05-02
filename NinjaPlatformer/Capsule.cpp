#include "Capsule.h"

void Capsule::init(b2World* world,
					const glm::vec2& position,
					const glm::vec2& dimensions,
					float density,
					float friction,
					std::vector<Engine::Vertex::ColorRGBA8> color) {
	_dimensions = dimensions;
	_color = color;
	_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	// body def
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = !_fixedRotation ? false : true;

	//1make the body
	//create  body and store it in private
	_body = world->CreateBody(&bodyDef);

	//2create fixture  	//3 create shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, (dimensions.y -  dimensions.x) / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	_fixtures[0] = _body->CreateFixture(&fixtureDef); //store fixture pointer and creat it using the world

	//2create fixtures and 3body for circles
	b2CircleShape circleShape;
	circleShape.m_radius = _dimensions.x / 2.0f;
	
	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = density;
	circleDef.friction = friction;

	circleShape.m_p.Set(0.0f, -dimensions.y / 2.0f + dimensions.x / 2.0f);
	_fixtures[1] = _body->CreateFixture(&circleDef);
	circleShape.m_p.Set(0.0f, dimensions.y / 2.0f - dimensions.x / 2.0f);
	_fixtures[2] = _body->CreateFixture(&circleDef);


}

void Capsule::drawDebug(Engine::DebugRenderer& debugRenderer, Engine::Vertex::ColorRGBA8 color) {
	glm::vec4 destRect;
	destRect.x = _body->GetPosition().x - _dimensions.x / 2.0f;
	destRect.y = _body->GetPosition().y - ( _dimensions.y - _dimensions.x) / 2.0f;
	destRect.z = _dimensions.x;
	destRect.w = _dimensions.y - _dimensions.x;

	debugRenderer.drawBox(destRect, color, _body->GetAngle());

	debugRenderer.drawCircle(glm::vec2(destRect.x + _dimensions.x / 2.0f, destRect.y), color, _dimensions.x / 2.0f);
	debugRenderer.drawCircle(glm::vec2(destRect.x + _dimensions.x / 2.0f, destRect.y + (_dimensions.y - _dimensions.x)), color, _dimensions.x / 2.0f);

}

void Capsule::destroy(b2World* world) {
	if (_body) {
		world->DestroyBody(_body);
		_body = nullptr;
	}
}