#include "Box.h"


Box::Box() {

}
Box::~Box() {

}

void Box::init(b2World* world, 
				const glm::vec2& position, 
				const glm::vec2& dimensions, 
				std::vector<Engine::Vertex::ColorRGBA8> color,
				Engine::GLTexture texture,
				glm::vec4 uvRect /*= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/,
				bool fixedRot /*= false*/) {

	_dimensions = dimensions;
	_color = color;
	_texture = texture;
	_uvRect = uvRect;
	_fixedRotation = fixedRot;
	_position = position;
	//another body def
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = !_fixedRotation ? false  : true;

	//1make the body
	//create  body and store it in private
	_body = world->CreateBody(&bodyDef);

	//2create fixture  	//3 create shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_fixture = _body->CreateFixture(&fixtureDef); //store fixture pointer and creat it using the world
}
void Box::draw(Engine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect;
	
	destRect.x = _body->GetPosition().x - getDimensions().x / 2.0f;
	destRect.y = _body->GetPosition().y - getDimensions().y / 2.0f;
	destRect.z = getDimensions().x;
	destRect.w = getDimensions().y;
	spriteBatch.draw(_texture.id, 0.0f, destRect, _uvRect, _color, _body->GetAngle());
}
void Box::destroy(b2World* world) {

}
