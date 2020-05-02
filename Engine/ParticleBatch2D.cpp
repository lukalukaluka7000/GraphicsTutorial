#include "ParticleBatch2D.h"

namespace Engine {




	ParticleBatch2D::ParticleBatch2D() {
		//empty
	}
	ParticleBatch2D::~ParticleBatch2D() {
		delete[] _particles;
	}

	void ParticleBatch2D::init(	int maxParticles, 
								float decayRate, 
								GLTexture texture, 
								std::function<void(Particle2D&, float)> updateFunc /* defaultParticleUpdate */) {
		_maxParticles = maxParticles;
		_particles = new Particle2D[maxParticles];
		_decayRate = decayRate;
		_texture = texture;
		_updateFunc = updateFunc;
	}

	void ParticleBatch2D::addParticle(	const glm::vec2& pos, 
										const glm::vec2& velocity, 
										const Vertex::ColorRGBA8& color, float width) {
		//find first particle that is not in use and set its parameter
		int particleIndex = findFreeParticle();

		auto& p = _particles[particleIndex];
		
		p.life = 1.0f;
		p.position = pos;
		p.velocity = velocity;
		p.color = color;
		p.width = width;
	}

	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < _maxParticles; i++) {
			//check if it i sactive
			if (_particles[i].life > 0.0f) {
				_updateFunc(_particles[i], deltaTime);

				//_particles[i].update(deltaTime);
				_particles[i].life -= _decayRate * deltaTime;
			}
		}
	}
	void ParticleBatch2D::draw(SpriteBatch *spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < _maxParticles; i++) {
			//check if it i sactive
			auto& p = _particles[i];
			if (p.life > 0.0f) {
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				spriteBatch->draw(_texture.id, 0.0f, destRect, uvRect, Engine::Vertex::ColorRGBA8::getVectorWithSameColor(p.color));
			}
		}
	}

	int ParticleBatch2D::findFreeParticle() {
		for (int i = _lastFreeParticle; i < _maxParticles; i++) {
			if (_particles[i].life <= 0.0f) {
				_lastFreeParticle = i;
				return i;
			}
		}
		for (int i = 0; i < _lastFreeParticle; i++) {
			if (_particles[i].life <= 0.0f) {
				_lastFreeParticle = i;
				return i;
			}
		}

		//no particles free owerweire first one
		return 0;
	}
}