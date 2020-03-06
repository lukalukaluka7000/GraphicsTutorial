#include "Bullet.h"

namespace Engine {
	Bullet::Bullet(glm::vec2 pos, float speed, glm::vec2 direction, int lifeTime) {
		_position = pos;
		_speed = speed;
		_direction = direction;
		_lifeTime = lifeTime;
	}
	Bullet::~Bullet() {

	}
	void Bullet::init() {

	}
	void Bullet::update() {
		_direction = glm::normalize(_direction);
		_position += _speed * _direction;
		_lifeTime--;
	}
	void Bullet::draw(Engine::SpriteBatch& spriteBatch) {

		glm::vec4 pos(_position.x, _position.y, 50.0f, 50.0f);
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		Engine::GLTexture texture = Engine::ResourceManager::getTexture("./Textures/Dudes/telescope_01.png");
		std::vector<Engine::Vertex::Color> colors;
		Engine::Vertex::Color color;
		color.r = 0;
		color.g = 255;
		color.b = 0;
		color.a = 255;
		colors.push_back(color);

		color.r = 0;
		color.g = 0;
		color.b = 255;
		color.a = 255;
		colors.push_back(color);

		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 255;
		colors.push_back(color);

		color.r = 0;
		color.g = 255;
		color.b = 0;
		color.a = 255;
		colors.push_back(color);

		spriteBatch.draw(texture.id, 0.0f, pos, uv, colors);
	}
}