#include "ParticleEngine2D.h"
#include "SpriteBatch.h"
#include"ParticleBatch2D.h"

namespace Engine {

	ParticleEngine2D::ParticleEngine2D() {

	}
	ParticleEngine2D::~ParticleEngine2D() {
		for (auto& b : _batches) {
			delete b;
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch) {
		_batches.push_back(particleBatch);
	}

	void ParticleEngine2D::update(float deltaTime) {
		for (auto& b : _batches) {
			b->update(deltaTime);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
		for (auto& b : _batches) {
			spriteBatch->begin();
			b->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}
	}

}