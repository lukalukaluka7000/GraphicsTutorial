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
#include<Engine/SpriteFont.h>
enum class PhysicsMode {
	RIGID,
	DYNAMIC
};
enum class ObjectMode {
	NONE,
	PLAYER,
	BOX,
	LIGHT
};
enum class PlayMode {
	FINISH,
	EDIT
};
class EditorScreen : public Engine::IGameScreen
{
public:
	friend class LevelManager;

	EditorScreen(Engine::Window* _window);
	~EditorScreen();

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
protected:
	
private:
	void checkInput();
	void initUI();

	void drawLevel(const SDL_Event& evnt);

	CEGUI::Slider* _rSlider = nullptr;
	CEGUI::Slider* _gSlider = nullptr;
	CEGUI::Slider* _bSlider = nullptr;

	Engine::GLSLProgram _lightProgram;
	Engine::Camera2D _levelCamera;
	Engine::Camera2D _uiCamera;
	Engine::Window* _window;
	

	Engine::GUI _gui;
	/*int _nextScreenIndex = SCREEN_INDEX_GAMEPLAY;*/ //tribat ce nam samo za nazad ic 
	void updateRadioButtons();
	void proceedToGameplayScreen();

	Engine::SpriteBatch _spriteBatch;
	Engine::GLSLProgram _textureProgram;
	Engine::SpriteFont _spriteFont;
	Engine::DebugRenderer _debugRenderer;
	Engine::GLTexture _blankTexture;

	Player _player; bool _hasPlayer = false;
	std::unique_ptr<b2World> _world;
	
	std::vector<Box> _boxes;
	std::vector<Light> _lights;

	bool MouseInUI();
	CEGUI::GroupBox* _groupBoxUI = nullptr;

	float _colorPickerRed = 255.0f;
	float _colorPickerBlue = 255.0f;
	float _colorPickerGreen = 255.0f;
	Engine::Vertex::ColorRGBA8 _currentColor;

	PhysicsMode _physicsMode = PhysicsMode::RIGID;
	ObjectMode _objectMode = ObjectMode::NONE;
	PlayMode _playMode = PlayMode::EDIT;

	CEGUI::RadioButton* _rigidRadioButton = nullptr;
	CEGUI::RadioButton* _dynamicRadioButton = nullptr;

	CEGUI::RadioButton* _playerRadioButton = nullptr;
	CEGUI::RadioButton* _boxRadioButton = nullptr;
	CEGUI::RadioButton* _lightRadioButton = nullptr;

	CEGUI::PushButton* _tryItOutPushButton = nullptr;

	bool onExitClicked(const CEGUI::EventArgs& e);
	bool onColorPickerRedChanged(const CEGUI::EventArgs& e);
	bool onColorPickerGreenChanged(const CEGUI::EventArgs& e);
	bool onColorPickerBlueChanged(const CEGUI::EventArgs& e);
	bool onRigidMouseClick(const CEGUI::EventArgs& e);
	bool onDynamicMouseClick(const CEGUI::EventArgs& e);
	bool onPlayerMouseClick(const CEGUI::EventArgs& e);
	bool onBoxMouseClick(const CEGUI::EventArgs& e);
	bool onLightMouseClick(const CEGUI::EventArgs& e);
	bool onTryItOutMouseClick(const CEGUI::EventArgs& e);

	int _nextScreenIndex = SCREEN_INDEX_TRYLEVEL;

	void writeToFile(const std::string& fileName);
};

