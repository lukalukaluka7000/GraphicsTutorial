#include "App.h"
#include<Engine/ScreenList.h>

App::App() {

}
App::~App() {

}

void App::onInit() {

}
void App::addScreens() {
	_mainMenuScreen = std::make_unique<MainMenuScreen>(&_window);
	_gameplayScreen = std::make_unique<GameplayScreen>(&_window);
	_editorScreen = std::make_unique<EditorScreen>(&_window);
	_tryLevelScreen = std::make_unique<TryLevelScreen>(&_window);
	
	_screenList->addScreen(_mainMenuScreen.get()); // index 0
	_screenList->addScreen(_gameplayScreen.get());	// index 1
	_screenList->addScreen(_editorScreen.get());	// index 2
	_screenList->addScreen(_tryLevelScreen.get()); // index 3

	_screenList->setScreen(_mainMenuScreen->getScreenIndex());
	//_screenList->setScreen(_tryLevelScreen->getScreenIndex());

}

void App::onExit() {

}