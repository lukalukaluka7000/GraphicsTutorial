#include "IMainGame.h"
#include "Timing.h"

#include"ScreenList.h"
#include"IGameScreen.h"

#include<SDL/SDL.h>
namespace Engine {

	IMainGame::IMainGame() {
		_screenList = std::make_unique<ScreenList>(this);
	}
	IMainGame::~IMainGame() {

	}

	void IMainGame::run() {
		//all the init stuff
		if (!init()) return;

		FPSLimiter limiter;
		limiter.setMaxFps(60.0f);

		_isRunning = true;
		while (_isRunning) {
			limiter.begin();
			inputManager.update();
			
			if (_isRunning) {

				
				update();


				draw();

				_fps = limiter.end();
				_window.swapBuffer();
			}
		}
	}
	void IMainGame::exitGame() {
		_currentScreen->onExit();
		if (_screenList) {
			_screenList->destroy();
			_screenList.reset();
		}
		_isRunning = false;
	}

	void IMainGame::onSDLEvent(SDL_Event& evnt) {
		switch (evnt.type) {
		case SDL_QUIT:
			exitGame();
			break;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.keyPressed(evnt.button.button);
			//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.keyRelease(evnt.button.button);
			break;
		case SDL_KEYDOWN:
			inputManager.keyPressed(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.keyRelease(evnt.key.keysym.sym);
			break;
		}
	}

	bool IMainGame::init() {
		Engine::init();
		

		if (!initSystems()) return false;

		//custom shit custom behavious for example stavit drugu rezoluciju i sicno
		onInit();
		addScreens();

		_currentScreen = _screenList->getCurrent();
		_currentScreen->onEntry();
		_currentScreen->setRunning();

		return true;
	}

	bool IMainGame::initSystems() {
		_window.initWindow("Default", 1024, 768, 0);

		return true;
	}

	void IMainGame::update() {
		if (_currentScreen->getGame()->_isRunning == true) {
			//check the state
			switch (_currentScreen->getState()) {
				case ScreenState::RUNNING:
					_currentScreen->update();
					break;

				case ScreenState::CHANGE_NEXT:
					_currentScreen->onExit();
					_currentScreen = _screenList->moveNext();
					if (_currentScreen) {
						_currentScreen->setRunning();
						_currentScreen->onEntry();
					}
					break;

				case ScreenState::CHANGE_PREVIOUS:
					_currentScreen->onExit();
					_currentScreen = _screenList->movePrev();
					if (_currentScreen) {
						_currentScreen->setRunning();
						_currentScreen->onEntry();
					}
					break;
				case ScreenState::EXIT_APPLICATION:
					exitGame();
					break;
				default:
					break;
			}
			
		}
		else {
			exitGame();
		}
	}
	void IMainGame::draw() {
		glViewport(0, 0, _window.getScreenWidth(), _window.getScreenHeight());

		if (_currentScreen && _currentScreen->getState() == ScreenState::RUNNING) {
			_currentScreen->draw();
		}
	}
}