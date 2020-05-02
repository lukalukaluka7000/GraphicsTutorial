#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace Engine {

	class IMainGame;
	enum class ScreenState {
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IGameScreen
	{
	public:
		

		friend class ScreenList;
		IGameScreen() {

		}
		virtual ~IGameScreen() {

		}
	
		//OVO SU VIRTUAL FUNKCIJE PREMA TOME ONO STA NASLJEDI OVAJ INTERFACE MORA TO SMA IMPLEMENTIRAT
		//called at begin at the end of app
		virtual void build() = 0;
		virtual void destroy() = 0;

		//called when a screen enters and exits focus
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		//called in main game loop
		virtual void update() = 0;
		virtual void draw() = 0;

		int getScreenIndex()const {
			return _screenIndex;
		}

		//return the index of next and previous screen
		virtual int getNextScreenIndex() = 0;
		virtual int getPreviousScreenIndex() = 0;

		void setParentGame(IMainGame* game) { _game = game; }

		

		void setRunning() {
			_currentState = ScreenState::RUNNING;
		}
		ScreenState getState()  { return _currentState; }

		IMainGame* getGame() { return _game; }
	protected:
		int _screenIndex = SCREEN_INDEX_NO_SCREEN;
		ScreenState _currentState = ScreenState::NONE;

		IMainGame* _game = nullptr;
	};

}
