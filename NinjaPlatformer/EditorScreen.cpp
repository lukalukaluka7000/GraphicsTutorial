#include "EditorScreen.h"
#include<Engine/ResourceManager.h>
#include<memory>
#include<Engine/GLTexture.h>
EditorScreen::EditorScreen(Engine::Window* window) : _spriteFont("Fonts/chintzy.ttf", 32) {
    _window = window;
    _screenIndex = SCREEN_INDEX_EDITOR;
}
EditorScreen::~EditorScreen() {

}

void EditorScreen::build() {

}
void EditorScreen::destroy() {
    _debugRenderer.dispose();
}

void EditorScreen::onEntry() {
    _world = std::make_unique<b2World>(b2Vec2(0.0f, -25.0f));
    _debugRenderer.init();

    // light program
    _lightProgram.compileShaders("./Shaders/lightShading.vert", "./Shaders/lightShading.frag");
    _lightProgram.addAttribute("vertexPosition");
    _lightProgram.addAttribute("vertexColor");
    _lightProgram.addAttribute("vertexUV");
    _lightProgram.linkShaders();

    //init camera
    _levelCamera.init(_window->getScreenWidth(), _window->getScreenHeight());
    _levelCamera.setScale(32.0f); //32. pixels per meter
    _uiCamera.init(_window->getScreenWidth(), _window->getScreenHeight());
    _uiCamera.setScale(1.0f);

    initUI();
    
    _spriteBatch.init();

    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();

    _blankTexture = Engine::ResourceManager::getTexture("Assets/blank.png");
}
void EditorScreen::writeToFile(const std::string& fileName) {
    std::ofstream file("./Levels/tempLevel.txt");
    if (!file) std::cerr << "Could not open file!" << std::endl;
    //file.open("Levels/" + fileName);
    file << _boxes.size() << "\n";
    for (auto& b : _boxes)
    {
        glm::vec2 pos = b.getPosition();
        float dim = b.getDimensions()[0];
        Engine::Vertex::ColorRGBA8 clr = b.getColor()[0];
        file << pos.x << " " << pos.y << " " << dim << " " << dim << " " << (int)clr.r << " " << (int)clr.g << " " << (int)clr.b << "\n";
    }
    if (_hasPlayer) {
        file << (int)1 << "\n"; // One player
        glm::vec2 posPl = _player.getPosition();
        glm::vec2 drawDimsPl = _player.getDrawDims();
        Engine::Vertex::ColorRGBA8 clr = _player.getColorVector()[0];
        file << posPl.x << " " << posPl.y << " " << drawDimsPl.x << " " << drawDimsPl.y << " " << (int)clr.r << " " << (int)clr.g << " " << (int)clr.b << "\n";
    }
    else
        file << (int)0 << "\n"; // zero player

    //write lights
    file << _lights.size() << "\n";
    for (auto& l : _lights)
    {
        glm::vec2 pos = l.getPosition();
        float radius = l.getRadius();
        Engine::Vertex::ColorRGBA8 clr = l.getColor();
        file << pos.x << " " << pos.y << " " << radius << " " << radius << " " << (int)clr.r << " " << (int)clr.g << " " << (int)clr.b << "\n";
    }
    file.close();

}
void EditorScreen::initUI() {
    _gui.init("GUI");
    _gui.loadScheme("TaharezLook.scheme");    
    _gui.loadScheme("AlfiskoSkin.scheme");
    _gui.setFont("DejaVuSans-10");
    
    // GroupBox UI
    {
        _groupBoxUI = static_cast<CEGUI::GroupBox*>(_gui.createWidget("TaharezLook/GroupBox",
            glm::vec4(0.0f, 0.0f, 0.28f, 0.70f),
            glm::vec4(0.0f),
            "GroupBoxUI"));
        _groupBoxUI->setAlwaysOnTop(false);
        _groupBoxUI->setVisible(true);
        _groupBoxUI->moveToBack();
        _groupBoxUI->disable();
    }

    // RGB Sliders
    {
        const float X_DIM = 0.020f, Y_DIM = 0.12f;
        const float Y_POS = 0.05f;
        
        _rSlider = static_cast<CEGUI::Slider*>(_gui.createWidget("TaharezLook/Slider",
            glm::vec4(0.05f, Y_POS, X_DIM, Y_DIM),
            glm::vec4(0.0f),
            "RedSlider"));
        _rSlider->setMaxValue(255.0f);
        _rSlider->setCurrentValue(_colorPickerRed);
        _rSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerRedChanged, this));
        _rSlider->setClickStep(1.0f);

        _gSlider = static_cast<CEGUI::Slider*>(_gui.createWidget("TaharezLook/Slider",
            glm::vec4(0.09f, Y_POS, X_DIM, Y_DIM),
            glm::vec4(0.0f),
            "GreenSlider"));
        _gSlider->setMaxValue(255.0f);
        _gSlider->setCurrentValue(_colorPickerGreen);
        _gSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerGreenChanged, this));
        _gSlider->setClickStep(1.0f);


        _bSlider = static_cast<CEGUI::Slider*>(_gui.createWidget("TaharezLook/Slider",
            glm::vec4(0.13f, Y_POS, X_DIM, Y_DIM),
            glm::vec4(0.0f),
            "BlueSlider"));
        _bSlider->setMaxValue(255.0f);
        _bSlider->setCurrentValue(_colorPickerBlue);
        _bSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerBlueChanged, this));
        _bSlider->setClickStep(1.0f);
    }
    
    // Radio Buttons (Rigid and Dynamic)
    {
        const float Y_POS = 0.25f;
        const float DIMS = 0.10f;
        const float DIMS_PIXELS = 20.0f;

        _rigidRadioButton = static_cast<CEGUI::RadioButton*>(_gui.createWidget("TaharezLook/RadioButton",
            glm::vec4(0.05f,        Y_POS,  0.0f,           0.0f),
            glm::vec4(0.0f,         0.0f,   DIMS_PIXELS,    DIMS_PIXELS),
            "RigidRadioButton"));
        _rigidRadioButton->setSelected(true);
        _rigidRadioButton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onRigidMouseClick, this));
        

        _dynamicRadioButton = static_cast<CEGUI::RadioButton*>(_gui.createWidget("TaharezLook/RadioButton",
            glm::vec4(0.05f *2.0f,  Y_POS,  0.0f,           0.0f),
            glm::vec4(0.0f,         0.0f,   DIMS_PIXELS,    DIMS_PIXELS),
            "DynamicRadioButton"));
        _dynamicRadioButton->setSelected(false);
        _dynamicRadioButton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onDynamicMouseClick, this));

    }

    // Radio Buttons (Player, Boxes, Lights)
    {
        const float Y_POS = 0.40f;
        const float DIMS = 0.10f;
        const float DIMS_PIXELS = 20.0f;

        _playerRadioButton = static_cast<CEGUI::RadioButton*>(_gui.createWidget("TaharezLook/RadioButton",
            glm::vec4(0.05f, Y_POS, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS),
            "PlayerRadioButton"));
        _playerRadioButton->setSelected(false);
        _playerRadioButton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onPlayerMouseClick, this));

        _boxRadioButton = static_cast<CEGUI::RadioButton*>(_gui.createWidget("TaharezLook/RadioButton",
            glm::vec4(0.05f, Y_POS, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS),
            "BoxRadioButton"));
        _boxRadioButton->setSelected(false);
        _boxRadioButton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onBoxMouseClick, this));


        _lightRadioButton = static_cast<CEGUI::RadioButton*>(_gui.createWidget("TaharezLook/RadioButton",
            glm::vec4(0.05f * 2.0f, Y_POS, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS),
            "LightRadioButton"));
        _lightRadioButton->setSelected(false);
        _lightRadioButton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onLightMouseClick, this));

    }

    // Push Button for Try it out!
    {
        const float Y_POS = 0.55f;
        const float DIMSX = 0.45f;
        const float DIMSY = 0.18f;
        const float DIMS_PIXELS = 40.0f;

        _tryItOutPushButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button",
            glm::vec4(0.05f, Y_POS, 0.09f, 0.03f),
            glm::vec4(0.0f),
            "TryItOutPushButton"));
        _tryItOutPushButton->setText("Try It Out!");
        //_tryItOutPushButton->setVisible(true);
        _tryItOutPushButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorScreen::onTryItOutMouseClick, this));
    }

    _gui.setMouseCursor("TaharezLook/MouseArrow");
    _gui.showMouseCursor();
    SDL_ShowCursor(0);
}

void EditorScreen::onExit() {
    _gui.destroy();
    _textureProgram.dispose();
    _lightProgram.dispose();
    _currentState = Engine::ScreenState::EXIT_APPLICATION;

}
int EditorScreen::getPreviousScreenIndex() {
    return SCREEN_INDEX_MAINMENU;
}
int EditorScreen::getNextScreenIndex() {
    return SCREEN_INDEX_TRYLEVEL;
}
void EditorScreen::proceedToGameplayScreen() {
    if (_playMode == PlayMode::FINISH) {
        // proceed to gamePlayScreen index
        _currentState = Engine::ScreenState::CHANGE_NEXT;
        _nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
    }
}
void EditorScreen::update() {
    proceedToGameplayScreen();

    _levelCamera.update();
    _uiCamera.update();
    updateRadioButtons();


    checkInput();
    _gui.update();
}
void EditorScreen::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    _textureProgram.use();

    //upload texture uniform
    GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    glm::mat4 projectionMatrix = _uiCamera.getCameraMatrix();
    GLint pUniform = _textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // UI CAMERA
    //Draw color picker quad
    _spriteBatch.begin();
    _currentColor = Engine::Vertex::ColorRGBA8((GLubyte)_colorPickerRed, (GLubyte)_colorPickerGreen, (GLubyte)_colorPickerBlue, (GLubyte)255.0f);
    {
        const float QUAD_SIZE = 75.0f;
        glm::vec4 destRect;
        destRect.x = _bSlider->getXPosition().d_scale * _window->getScreenWidth() + QUAD_SIZE - _window->getScreenWidth() / 2.0f;
        destRect.y = _bSlider->getYPosition().d_scale * _window->getScreenHeight() +            _window->getScreenHeight() / 2.0f - 150.0f;
        destRect.z = QUAD_SIZE;
        destRect.w = QUAD_SIZE;
        _spriteBatch.draw(_blankTexture.id, 0.0f, destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), _currentColor.getVectorWithSameColor(_currentColor));
    }

    // Draw labels for rigid and dynamic, player,box,light
    {
        glm::vec2 pos;
        pos.x = _rigidRadioButton->getXPosition().d_scale * _window->getScreenWidth() - _window->getScreenWidth() / 2.0f +
            _rigidRadioButton->getWidth().d_offset / 2.0f;
        pos.y = _window->getScreenHeight() / 2.0f - _rigidRadioButton->getYPosition().d_scale * _window->getScreenHeight();
        _spriteFont.draw(_spriteBatch, "Rigid", pos, glm::vec2(0.49f), 0.0f, _currentColor, Engine::Justification::MIDDLE);

        pos.x = _dynamicRadioButton->getXPosition().d_scale * _window->getScreenWidth() - _window->getScreenWidth() / 2.0f +
            _dynamicRadioButton->getWidth().d_offset / 2.0f;
        pos.y = _window->getScreenHeight() / 2.0f - _dynamicRadioButton->getYPosition().d_scale * _window->getScreenHeight();
        _spriteFont.draw(_spriteBatch, "Dinamic", pos, glm::vec2(0.41f), 0.0f, _currentColor, Engine::Justification::MIDDLE);

        // Draw appropriate radio buttons for rigid and dynamic
        if (_physicsMode == PhysicsMode::RIGID) {
            pos.x = _boxRadioButton->getXPosition().d_scale * _window->getScreenWidth() - _window->getScreenWidth() / 2.0f +
                _boxRadioButton->getWidth().d_offset / 2.0f - 2.0f;
            pos.y = _window->getScreenHeight() / 2.0f - _boxRadioButton->getYPosition().d_scale * _window->getScreenHeight();
            _spriteFont.draw(_spriteBatch, "Box", pos, glm::vec2(0.49f), 0.0f, _currentColor, Engine::Justification::MIDDLE);

            pos.x = _lightRadioButton->getXPosition().d_scale * _window->getScreenWidth() - _window->getScreenWidth() / 2.0f +
                _lightRadioButton->getWidth().d_offset / 2.0f;
            pos.y = _window->getScreenHeight() / 2.0f - _lightRadioButton->getYPosition().d_scale * _window->getScreenHeight();
            _spriteFont.draw(_spriteBatch, "Light", pos, glm::vec2(0.41f), 0.0f, _currentColor, Engine::Justification::MIDDLE);
        }
        else if (_physicsMode == PhysicsMode::DYNAMIC) {
            pos.x = _playerRadioButton->getXPosition().d_scale * _window->getScreenWidth() - _window->getScreenWidth() / 2.0f +
                _playerRadioButton->getWidth().d_offset / 2.0f;
            pos.y = _window->getScreenHeight() / 2.0f - _playerRadioButton->getYPosition().d_scale * _window->getScreenHeight();
            _spriteFont.draw(_spriteBatch, "Player", pos, glm::vec2(0.49f), 0.0f, _currentColor, Engine::Justification::MIDDLE);
        }
    }
    _spriteBatch.end();
    _spriteBatch.renderBatch();

    glm::vec4 line;
    line = glm::vec4(0.0f, 0.0f, 50.0f, 0.0f);
    _debugRenderer.drawLine(line, Engine::Vertex::ColorRGBA8(255, 0, 0, 200));
    line = glm::vec4(0.0f, 0.0f, 0.0f, 50.0f);
    _debugRenderer.drawLine(line, Engine::Vertex::ColorRGBA8(0, 255, 0, 200));

    _debugRenderer.end();
    _debugRenderer.render(projectionMatrix, 2.0);
    _textureProgram.unuse();


    // LEVEL CAMERA
    _textureProgram.use();
    _spriteBatch.begin();

    projectionMatrix = _levelCamera.getCameraMatrix();
    pUniform = _textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);
    // Draw Player
    if(_hasPlayer)
        _player.draw(_spriteBatch);
    // Draw Boxes
    for (auto& b : _boxes) 
        b.draw(_spriteBatch);
    _spriteBatch.end();
    _spriteBatch.renderBatch();
    _textureProgram.unuse();

    _lightProgram.use();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    pUniform = _lightProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
    _spriteBatch.begin();
    for (auto& l : _lights)
        l.draw(_spriteBatch);
    _spriteBatch.end();
    _spriteBatch.renderBatch();
    _lightProgram.unuse();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    _gui.draw();
}
bool EditorScreen::MouseInUI() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    /*std::cout << "mouse in ui: " << x << " " << y << std::endl;*/
    /*std::cout << _groupBoxUI->getPixelPosition() << std::endl;
    std::cout << _groupBoxUI->getArea() << std::endl;
    std::cout << _groupBoxUI->getPixelPosition() << std::endl;
    std::cout << _groupBoxUI->getMouseCursor() << std::endl;
    std::cout << _groupBoxUI->getHeight() << std::endl;
    std::cout << _groupBoxUI->isHit(CEGUI::Vector2f(-10,0),true) << std::endl;*/
    std::cout << _groupBoxUI->isHit(CEGUI::Vector2f(x, y), true) << std::endl;

    // simple function to check if is hit, since group box is disabled
    // i set optional second parameter to true, that means it can hit disabled groupbox 
    return _groupBoxUI->isHit(CEGUI::Vector2f(x, y), true);
}
void EditorScreen::drawLevel(const SDL_Event & evnt) {
    switch (evnt.button.button) {
    case SDL_BUTTON_LEFT:
        std::cout << "aaaaaa;" << std::endl;
        std::cout << static_cast<std::underlying_type<ObjectMode>::type>(_objectMode) << std::endl;
        std::cout << static_cast<std::underlying_type<PlayMode>::type>(_playMode) << std::endl;
        if (MouseInUI()) 
            break;
        if (_playMode == PlayMode::EDIT) {
            Engine::Vertex::ColorRGBA8 color(_currentColor.r, _currentColor.g, _currentColor.b, _currentColor.a);
            if (_objectMode == ObjectMode::PLAYER) {
                Engine::TileSheet playerSheets;
                playerSheets._texture = Engine::ResourceManager::getTexture("Assets/blue_ninja.png");
                glm::vec2 pos(_levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)));
                _player.init(_world.get(),
                    pos,
                    glm::vec2(2.0f),   // drawdims  Player 
                    glm::vec2(1.0f, 1.8f),
                    color.getVectorWithSameColor(color),
                    playerSheets);
                std::cout << _levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)).x << " " << _levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)).y << std::endl;
                _hasPlayer = true;
                //_player.draw(_spriteBatch);
            }
            else if (_objectMode == ObjectMode::BOX) {
                Box newBox;
                glm::vec2 pos(_levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)));
                Engine::GLTexture texture = Engine::ResourceManager::getTexture("Assets/bricks_top.png");
                newBox.init(_world.get(),
                    pos,
                    glm::vec2(1.0f, 1.0f),
                    color.getVectorWithSameColor(color),
                    texture);
                _boxes.push_back(newBox);
                std::cout << _levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)).x << " " << _levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)).y << std::endl;
            }
            else if (_objectMode == ObjectMode::LIGHT) {
                Light newLight;
                glm::vec2 pos(_levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)));
                newLight.init(  1.5f,
                                glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f),
                                pos,
                                color);
                _lights.push_back(newLight);
                std::cout << _levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)).x << " " << _levelCamera.ScreenToScene(glm::vec2(evnt.button.x, evnt.button.y)).y << std::endl;
            }
        }
        else {
            // inside FinishMode -> send all the neceseary values to gameplay screen
        }
        break;
    default:
        break;
    }
}


void EditorScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
        _gui.OnSDLevent(evnt);
        switch (evnt.type) {
        case SDL_QUIT:
            onExitClicked(CEGUI::EventArgs());
            break;
        case SDL_MOUSEBUTTONDOWN:
            drawLevel(evnt);
            break;
        }

    }
}
void EditorScreen::updateRadioButtons() {
    if (_dynamicRadioButton->isSelected()) {
        _boxRadioButton->hide();
        _lightRadioButton->hide();
        _playerRadioButton->show();
    }
    else if (_rigidRadioButton->isSelected()) {
        _playerRadioButton->hide();
        _boxRadioButton->show();
        _lightRadioButton->show();
    }
}

bool EditorScreen::onExitClicked(const CEGUI::EventArgs& e) {
    _currentState = Engine::ScreenState::EXIT_APPLICATION;
    return true;
}
bool EditorScreen::onColorPickerRedChanged(const CEGUI::EventArgs& e) {
    _colorPickerRed = _rSlider->getCurrentValue();
    //std::cout << _colorPickerRed << std::endl;

    return true;
}
bool EditorScreen::onColorPickerGreenChanged(const CEGUI::EventArgs& e) {
    _colorPickerGreen = _gSlider->getCurrentValue();
    //std::cout << _colorPickerGreen << std::endl;

    return true;
}
bool EditorScreen::onColorPickerBlueChanged(const CEGUI::EventArgs& e) {
    _colorPickerBlue = _bSlider->getCurrentValue();
    //std::cout << _colorPickerBlue << std::endl;
    return true;
}
bool EditorScreen::onRigidMouseClick(const CEGUI::EventArgs& e) {
    _physicsMode = PhysicsMode::RIGID;
    return true;
}
bool EditorScreen::onDynamicMouseClick(const CEGUI::EventArgs& e) {
    _physicsMode = PhysicsMode::DYNAMIC;
    return true;
}
bool EditorScreen::onPlayerMouseClick(const CEGUI::EventArgs& e) {
    _objectMode = ObjectMode::PLAYER;
    return true;
}
bool EditorScreen::onBoxMouseClick(const CEGUI::EventArgs& e) {
    _objectMode = ObjectMode::BOX;
    return true;
}
bool EditorScreen::onLightMouseClick(const CEGUI::EventArgs& e) {
    _objectMode = ObjectMode::LIGHT;
    std::cout << "bbbbb" << std::endl;
    return true;
}
bool EditorScreen::onTryItOutMouseClick(const CEGUI::EventArgs& e) {
    _playMode = PlayMode::FINISH;
    writeToFile("tempLevel.txt");
    return true;
    //todo getback to edit mode
}