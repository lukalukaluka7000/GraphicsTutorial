#pragma once

#include<Box2D/Box2D.h>
#include<Engine/IGameScreen.h>
#include<vector>

#include<Engine/SpriteBatch.h>
#include<Engine/GLSLProgram.h>
#include<Engine/Camera2D.h>
#include<Engine/GLTexture.h>
#include<Engine/Window.h>

#include"Box.h"
#include"Player.h" 

#include<Engine/DebugRenderer.h>
#include"Light.h"

#include<Engine/GUI.h>

class GameplayScreen : public Engine::IGameScreen
{
public:
	GameplayScreen(Engine::Window* _window);
	~GameplayScreen();

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
	void initUI();

	bool onExitClicked(const CEGUI::EventArgs& e);

	std::unique_ptr<b2World> _world;
	std::vector<Box> _boxes;

	Engine::SpriteBatch _spriteBatch;
	Engine::GLSLProgram _textureProgram;
	Engine::GLSLProgram _lightProgram;
	Engine::Camera2D _camera;
	
	Engine::Window* _window;

	Player _player;
	Engine::Vertex::ColorRGBA8 _playerColor;

	Engine::DebugRenderer _debugRenderer;

	bool _renderDebug = true;

	Light _mouseLight;
	Light _playerLight;

	Engine::GUI _gui;

	bool _AbortMission = false;
};

