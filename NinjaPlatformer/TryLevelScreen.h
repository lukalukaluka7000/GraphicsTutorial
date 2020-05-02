#pragma once
#include<Engine/IGameScreen.h>
#include"ScreenIndices.h"
#include"Light.h"
#include<Engine/SpriteBatch.h>
#include<Engine/GLSLProgram.h>
#include<Engine/Camera2D.h>
#include<Engine/GLTexture.h>
#include<Engine/Window.h>

#include"Box.h"
#include"Player.h" 
class TryLevelScreen : public Engine::IGameScreen
{
public:
	TryLevelScreen(Engine::Window* _window);
	~TryLevelScreen();

	virtual void build() override;
	virtual void destroy()override;

	//called when a screen enters and exits focus
	virtual void onEntry()override;
	virtual void onExit()override;

	//called in main game loop
	virtual void update()override;
	virtual void draw()override;

	virtual int getNextScreenIndex()  override;
	virtual int getPreviousScreenIndex() override;

private:
	void checkInput();

	std::unique_ptr<b2World> _world;
	std::vector<Box> _boxes;
	std::vector<Light> _lights;

	Engine::SpriteBatch _spriteBatch;
	Engine::GLSLProgram _textureProgram;
	Engine::GLSLProgram _lightProgram;
	Engine::Camera2D _camera;

	Engine::Window* _window;

	Player _player;
	Engine::Vertex::ColorRGBA8 _playerColor;

	bool _AbortMission = false;
	
	void readFromFileAndInit(const std::string& fileName);
	std::vector<std::string> _levelData;
};

