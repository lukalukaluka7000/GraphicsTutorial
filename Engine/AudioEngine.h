#pragma once
#include<SDL/SDL_mixer.h>
#include<string>
#include<iostream>
#include<map>

namespace Engine {
	//2 ways of playing sound
	//a) mix chunk sound effect
	class SoundEffect {
	public:
		friend class AudioEngine;

		void play(int loops = 0);
	private:
		//what  sound effect is wrapping is mixed chunk
		Mix_Chunk* _chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;
		void play(int loops = -1);

		static void pause();
		static void stop();
		static void resume();
	private:
		Mix_Music* _music = nullptr;
	};


	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);
	
	private:
		bool _isInitialised = false;

		//cache for music and effects
		std::map<std::string, Mix_Chunk*> _effectMap;
		std::map<std::string, Mix_Music*> _musicMap;
	};
}
