#pragma once
#include<glm/glm.hpp>
#include"Vertex.h"
#include"SpriteBatch.h"
#include"GLTexture.h"

#include<functional> // access a function poitner

namespace Engine {

	class Particle2D {
	public:
		glm::vec2 position = glm::vec2(0.0);
		glm::vec2 velocity = glm::vec2(0.0f);
		Vertex::ColorRGBA8 color;
		float life = 0.0f;
		bool isActive;
		float width = 0.0f;
	};
	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		//friend class ParticleBatch2D; // you can touch my privates

		void update(float deltaTime);


		void init(	int maxParticles, 
					float decayRate,
					GLTexture texture, 
					std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate
				);

		void addParticle(const glm::vec2& pos,const glm::vec2& velocity, const Vertex::ColorRGBA8& color, float width);


		void draw(SpriteBatch *spriteBatch);


	private:
		int findFreeParticle();
		
		std::function<void(Particle2D&, float)> _updateFunc;

		float _decayRate;
		Particle2D* _particles = nullptr;
		int _maxParticles;
		
		GLTexture _texture;

		int _lastFreeParticle = 0;
	};
}

