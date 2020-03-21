#include "Timing.h"

namespace Engine {
	FPSLimiter::FPSLimiter() : _maxFPS(60000.0f){

	}
	void FPSLimiter::init(float maxFPS) {
		setMaxFps(maxFPS);
	}
	void FPSLimiter::setMaxFps(float maxFPS) {
		_maxFPS = maxFPS;
	}
	float FPSLimiter::begin() {
		_startTicks = SDL_GetTicks(); //jer me zanima koliko triba do dna ovaj jedan loop
		return _startTicks;
	}
	float FPSLimiter::end() { //limit the FPS
		calculateFPS(); // ON SPREMI fps u _fps

		//Limit the FPS to _maxFPS
		float frameTicks = SDL_GetTicks() - _startTicks;
		//check if we even NEED to limit FPS
		if ((1000.0f / _maxFPS) > frameTicks) {
			SDL_Delay((1000.0f / _maxFPS) - frameTicks); //Task Manager said Thank you !!! :)
		}
		return _fps;
	}
	void FPSLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];

		static int currentFrame = 0;
		//std::cout << currentFrame << std::endl;
		static float previousTicks = SDL_GetTicks(); //mozemo ovo minjat u ostatku koda ali necemo jer zelimo da je static
		float currentTicks;
		currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - previousTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		previousTicks = currentTicks;


		currentFrame++;

		int count; // koliko cemo ih averageat
		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		float frameTimeAvgs = 0;
		float avg;

		for (int i = 0; i < count; i++) {
			frameTimeAvgs += frameTimes[i];
		}
		avg = frameTimeAvgs / count;

		if (avg > 0) {
			_fps = 1000 / avg; //1000 jer je toliko ms/s (ms u sekundi)
		}
		else
		{
			_fps = 60.0f;
		}
	}
}
