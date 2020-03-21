#pragma once
#include<glm/glm.hpp>
#include<Engine/ResourceManager.h>
#include<vector>
#include<Engine/Vertex.h>
#include<Engine/SpriteBatch.h>
namespace Engine {
	class Bullet
	{
	public:
		Bullet(glm::vec2 pos, float speed, glm::vec2 direction, int lifeTime);
		~Bullet();

		void init();
		void update();
		void draw(Engine::SpriteBatch& spriteBatch);
		int _lifeTime;
	private:
		glm::vec2 _position;
		float _speed;
		glm::vec2 _direction;
		
		
	};
}
