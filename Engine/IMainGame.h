#pragma once
#include<memory>
#include"Engine.h"
#include"Window.h"
#include"InputManager.h"

namespace Engine {
	class ScreenList;
	class IGameScreen;

	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void run();
		void exitGame();

		//ovo je prva stvar koju triba implementirat tamo di nasljedimo
		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;

		const float getFps() const {
			return _fps;
		}
		
		void onSDLEvent(SDL_Event& evnt);

		InputManager inputManager;

		
		//whatever class inherit from it can call this NICE
	protected:
		virtual void update(); // custom update function
		virtual void draw(); // custom draw function

		

		std::unique_ptr<ScreenList> _screenList = nullptr;

		IGameScreen* _currentScreen = nullptr;

		bool _isRunning = false;
		bool _hasChangedInUpdate = false;

		float _fps = 0;

		Window _window;

		//initialization function
		bool init();
		bool initSystems();

		
	};

}
