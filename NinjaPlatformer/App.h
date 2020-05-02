#pragma once
#include<Engine/IMainGame.h>
#include"GameplayScreen.h"
#include"MainMenuScreen.h"
#include"EditorScreen.h"
#include"TryLevelScreen.h"
class App : public Engine::IMainGame
{
public:
	App();
	~App();

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;

private:
	std::unique_ptr<GameplayScreen> _gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> _mainMenuScreen = nullptr;
	std::unique_ptr<EditorScreen> _editorScreen = nullptr;
	std::unique_ptr<TryLevelScreen> _tryLevelScreen = nullptr;
};

