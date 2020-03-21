#include "AudioEngine.h"
#include "EngineErrors.h"
#include<map>

namespace Engine {
	
	void SoundEffect::play(int loops /* = 0 */) {
		if (Mix_PlayChannel(-1, _chunk, loops) == -1) {
			if (Mix_PlayChannel(0, _chunk, loops) == -1) {
				fatalError("Mix_PlayChannel error : " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops /* = -1 */) {
		if (Mix_PlayMusic(_music, loops) == -1) {
			fatalError("Mix_PlayMusic error : " + std::string(Mix_GetError()));
		}
	}

	void Music::pause() {
		Mix_PauseMusic();
	}
	void Music::stop() {
		Mix_HaltMusic();
	}
	void Music::resume() {
		Mix_ResumeMusic();
	}
	
	AudioEngine::AudioEngine() {
		//Emptry
	}
	AudioEngine::~AudioEngine() {
		destroy();
	}

	void AudioEngine::init() {
		if (_isInitialised == true) {
			fatalError("Tried to Initialise Audio Engine twice!\n");

		}
		//can be a bitwise combination of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) { //sets up basic init stuff, flags bitwise operations
			fatalError("Mix_Init error : " + std::string(Mix_GetError()));
		}

		//frequency format number of bytes at one time
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			fatalError("Mix_Init error : " + std::string(Mix_GetError()));
		}
		_isInitialised = true;
	}

	void AudioEngine::destroy() {
		if (_isInitialised == true) {
			_isInitialised = false;
			
			/*for (auto it = _effectMap.begin(); it != _effectMap.end(); it++) {
				Mix_FreeChunk(it->second);
			}*/
			for (auto& it : _effectMap) {
				Mix_FreeChunk(it.second);
			}
			for (auto& it : _musicMap) {
				Mix_FreeMusic(it.second);
			}

			_effectMap.clear();
			_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}
	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
		//try to find audio in cache
		auto it = _effectMap.find(filePath);

		// this we return
		SoundEffect soundEffect;

		if (it == _effectMap.end()) {
			//failed to find it
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr) {
				//Mix_LoadWAW error : Mix_LoadWAV_RW with NULL src
				std::cout << filePath << std::endl;
				fatalError("Mix_LoadWAW error : " + std::string(Mix_GetError()));
			}

			//friend keyword allows us to touch others private's
			soundEffect._chunk = chunk;
			_effectMap[filePath] = chunk;

		}
		else {
			//found it, just pick it up and return taht SoundEffect
			soundEffect._chunk = it->second;
		}
		return soundEffect;
	}
	Music AudioEngine::loadMusic(const std::string& filePath) {
		//try to find audio in cache
		auto it = _musicMap.find(filePath);

		Music musicObject;
		

		if (it == _musicMap.end()) {
			//failed to find it
			Mix_Music* musicChunk = Mix_LoadMUS(filePath.c_str());
			if (musicChunk == nullptr) {
				fatalError("Mix_LoadMUS error : " + std::string(Mix_GetError()));
			}
			
			musicObject._music = musicChunk;
			_musicMap[filePath] = musicChunk;

		}
		else {
			//found it, just pick it up and return taht SoundEffect
			musicObject._music = it->second;
		}
		return musicObject;
	}

}