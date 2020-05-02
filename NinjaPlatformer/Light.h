#pragma once
#include<glm/glm.hpp>
#include<Engine/SpriteBatch.h>
class Light {
public:
	void init(float radius, glm::vec4 uvRectLight, glm::vec2 position, Engine::Vertex::ColorRGBA8 color) {
		_radius = radius;
		_uvRectLight = uvRectLight;
		_position = position;
		_color = color;
	}
	void draw(Engine::SpriteBatch& spriteBatch) {

		glm::vec4 destRect;
		destRect.x = _position.x - _radius;
		destRect.y = _position.y - _radius;
		destRect.z = 2* _radius;
		destRect.w = 2* _radius;

		//without any texture
		GLuint a = 0;
		spriteBatch.draw(a, 0.0f, destRect, _uvRectLight, _color.getVectorWithSameColor(_color));
	}
	void update(glm::vec2 position) {
		_position = position;
	}
	glm::vec2 getPosition() { return _position; }
	float getRadius() { return _radius; }
	Engine::Vertex::ColorRGBA8 getColor() { return _color; }
private:
	float _radius;
	glm::vec4 _uvRectLight;
	glm::vec2 _position;
	Engine::Vertex::ColorRGBA8 _color;
};