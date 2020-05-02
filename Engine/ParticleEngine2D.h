#pragma once

#include<vector>


namespace Engine {

	class ParticleBatch2D;
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		//after adding particle batch, ParticleEngine2D becomes responsible for deallocation
		void addParticleBatch(ParticleBatch2D* particleBatch);

		void update(float deltaTime);

		void draw(SpriteBatch * spriteBatch);

	private:
		std::vector<ParticleBatch2D*> _batches;
	};
}

