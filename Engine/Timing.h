#pragma once
#include<SDL/SDL.h>
namespace Engine {
	class FPSLimiter {
	public:
		FPSLimiter();

		void init(float maxFps);
		void setMaxFps(float maxFPS);
		float begin();

		float end();

	private:
		void calculateFPS();
		float _startTicks;
		float _maxFPS;
		float _fps;
		float _frameTime;
	};
}