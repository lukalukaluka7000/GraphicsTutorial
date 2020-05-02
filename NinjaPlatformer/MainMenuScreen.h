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
#include"ScreenIndices.h"

class MainMenuScreen : public Engine::IGameScreen
{
	
	
	public:
		MainMenuScreen(Engine::Window* _window);
		~MainMenuScreen();

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
		bool onPlayClicked(const CEGUI::EventArgs& e);
		bool onEditorClicked(const CEGUI::EventArgs& e);

		Engine::GLSLProgram _lightProgram;
		Engine::Camera2D _camera;
		Engine::Window* _window;
		Light _mouseLight;

		Engine::GUI _gui;
		int _nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
};

