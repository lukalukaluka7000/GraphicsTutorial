#include "GameplayScreen.h"
#include<iostream>
#include<SDL/SDL.h>
#include<Engine/IMainGame.h>
#include"Box.h"

#include<Engine/ResourceManager.h>
#include<random>

#include"ScreenIndices.h"

GameplayScreen::GameplayScreen(Engine::Window* window) {
    _window = window;
    _screenIndex = SCREEN_INDEX_GAMEPLAY;
}
GameplayScreen::~GameplayScreen() {

}

void GameplayScreen::build() {

}
void GameplayScreen::destroy() {

 }

void GameplayScreen::onEntry() {

    //pandan za glm::vec2 b2vec2
    b2Vec2 gravity(0.0f, -24.81f);

    //create world
    _world = std::make_unique<b2World>(gravity);

    _debugRenderer.init();

    //create rigid body or object
    //fixture bind different shape to rigid body
    //GROUND
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -21.0f);

    b2Body* groundBody = _world->CreateBody(&groundBodyDef); 
    //make the ground fixture
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f); //density static object

    std::mt19937 randGenerator;
    std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
    std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);
    std::uniform_real_distribution<float> size(0.5f, 2.5f);
    std::uniform_int_distribution<int> color(0, 255);

    //texture
    Engine::GLTexture texture = Engine::ResourceManager::getTexture("Assets/bricks_top.png");
    Engine::TileSheet playerSheets;
    playerSheets._texture = Engine::ResourceManager::getTexture("Assets/blue_ninja.png");

    const int NUM_BOXES = 10;

    for (int i = 0; i < NUM_BOXES; i++) {
        Box newBox;

        Engine::Vertex::ColorRGBA8 randColor;
        randColor.r = color(randGenerator);
        randColor.g = color(randGenerator);
        randColor.b = color(randGenerator);
        randColor.a = 255;

        newBox.init(    _world.get(), 
                        glm::vec2(xPos(randGenerator), yPos(randGenerator)), 
                        glm::vec2(size(randGenerator), size(randGenerator)),
                        randColor.getVectorWithSameColor(randColor),
                        texture);
        _boxes.push_back(newBox);
    }

    _playerColor.SetColor(0,0,255,255);
    _player.init(   _world.get(), 
                    glm::vec2(0.0f, 8.0f), 
                    glm::vec2(2.0f),   // drawdims  Player 
                    glm::vec2(1.0f, 1.8f),  //collision dimensions - dimensions of capsule
                    _playerColor.getVectorWithSameColor(_playerColor),
                    playerSheets);

    _mouseLight.init(  3.5f,
                  glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f),
                  glm::vec2(0.0f,0.0f),//_camera.ScreenToScene(_game->inputManager.getMouseCoords()),
                  Engine::Vertex::ColorRGBA8(255, 255, 255, 255)
                );
    _playerLight.init(9.0f,
                    glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f),
                    glm::vec2(0.0f, 0.0f),
                    Engine::Vertex::ColorRGBA8(112, 0, 187, 255)
        );

    //init spritebatch
    _spriteBatch.init();

    // world texture program
    _textureProgram.compileShaders("./Shaders/textureShading.vert", "./Shaders/textureShading.frag");
    std::cout << "1" << std::endl;
    _textureProgram.addAttribute("vertexPosition");
    std::cout << "2" << std::endl;
    _textureProgram.addAttribute("vertexColor");
    std::cout << "3" << std::endl;
    _textureProgram.addAttribute("vertexUV");
    std::cout << "4" << std::endl;
    _textureProgram.linkShaders();
    std::cout << "5" << std::endl;

    // light program
    _lightProgram.compileShaders("./Shaders/lightShading.vert", "./Shaders/lightShading.frag");
    _lightProgram.addAttribute("vertexPosition");
    _lightProgram.addAttribute("vertexColor");
    _lightProgram.addAttribute("vertexUV");
    _lightProgram.linkShaders();

    //init camera
    _camera.init(_window->getScreenWidth(), _window->getScreenHeight());
    _camera.setScale(32.0f); //32. pixels per meter

    ////temp ui -> using opengl context !!!! -> zato san i ima problema sa verzijama opengla 4.5 a ovaj renderer3 ne moze pa moran obicni
    //CEGUI::OpenGLRenderer& myRenderer = CEGUI::OpenGLRenderer::bootstrapSystem();
    ////CEGUI::OpenGL3Renderer& myRenderer = CEGUI::OpenGL3Renderer::bootstrapSystem();

    initUI();
 }
void GameplayScreen::initUI() {
     _gui.init("GUI");
     _gui.loadScheme("TaharezLook.scheme");
     _gui.loadScheme("AlfiskoSkin.scheme");
     _gui.setFont("DejaVuSans-10");
     _gui.setMouseCursor("TaharezLook/MouseArrow");

     CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.5f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "testButton"));
     testButton->setText("Exit Game!");

     testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

     CEGUI::Combobox* testComboBox = static_cast<CEGUI::Combobox*>(_gui.createWidget("TaharezLook/Combobox", glm::vec4(0.3f, 0.3f, 0.15f, 0.05f), glm::vec4(0.0f), "testComboBox"));

     _gui.showMouseCursor();
     SDL_ShowCursor(0);
 }
bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
     _currentState = Engine::ScreenState::EXIT_APPLICATION;
     return true;
 }
void GameplayScreen::onExit() {
    _player.destroy(_world.get());
    _debugRenderer.dispose();
    _boxes.clear();
    _world.reset();
    _AbortMission = true;
    _currentState = Engine::ScreenState::EXIT_APPLICATION;
 }

void GameplayScreen::update() {
    _camera.update();
    _game->inputManager.update();


    checkInput(); // check for jeli se stiska x, ako je e onda ne ulazi
    //neznam jeli ovo velika mana ovog obrasca ili ja neznam kako ga koristiti
    //pitat strucnjaka nekoga // ovo definitinvo nebi triba radit u gameplayscreen.cpp

    if (!_AbortMission) {
        _player.update(_game->inputManager);

        _mouseLight.update(_camera.ScreenToScene(_game->inputManager.getMouseCoords()));
        glm::vec2 updatedPosition(_player.getCapsule().getBody()->GetPosition().x, _player.getCapsule().getBody()->GetPosition().y);
        _playerLight.update(updatedPosition);

        _world->Step(1.0f / 60.0f, 6, 2);
    }
 }
void GameplayScreen::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (!_AbortMission) {
        _gui.draw();
        // texture program use
        _textureProgram.use();

        //upload texture uniform
        GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
        glUniform1i(textureUniform, 0);
        glActiveTexture(GL_TEXTURE0);

        glm::mat4 projectionMatrix = _camera.getCameraMatrix();
        GLint pUniform = _textureProgram.getUniformLocation("P");
        glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

        _spriteBatch.begin();

        for (auto& b : _boxes) {
            b.draw(_spriteBatch);
        }
        _player.draw(_spriteBatch);

        _spriteBatch.end();
        _spriteBatch.renderBatch();
        _textureProgram.unuse();



        //light program use
        _lightProgram.use();

        //projectionMatrix = _camera.getCameraMatrix();
        pUniform = _lightProgram.getUniformLocation("P");
        glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

        //additive blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        _spriteBatch.begin();

        _mouseLight.draw(_spriteBatch);
        _playerLight.draw(_spriteBatch);

        _spriteBatch.end();
        _spriteBatch.renderBatch();

        _lightProgram.unuse();



        // Debug rendering
        if (_game->inputManager.isKeyPressed(SDLK_t)) {
            _renderDebug = _renderDebug ? false : true;
            std::cout << _renderDebug << std::endl;
        }
        if (_renderDebug) {
            glm::vec4 destRect;
            //render boxes
            for (auto& b : _boxes) {
                destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
                destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
                destRect.z = b.getDimensions().x;
                destRect.w = b.getDimensions().y;


                _debugRenderer.drawBox(destRect, Engine::Vertex::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());

                /* _debugRenderer.drawCircle(glm::vec2(b.getBody()->GetPosition().x, b.getBody()->GetPosition().y),
                                         Engine::Vertex::ColorRGBA8(255, 255, 255, 255),
                                         b.getDimensions().x / 2.0f);*/

            }

            ////render player
            //auto b = _player.getCapsule();//getBox();
            //destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
            //destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
            //destRect.z = b.getDimensions().x;
            //destRect.w = b.getDimensions().y;
            //_debugRenderer.drawBox(destRect, Engine::Vertex::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());

            _player.drawDebug(_debugRenderer, Engine::Vertex::ColorRGBA8(255, 255, 255, 255));

            _debugRenderer.end();
            _debugRenderer.render(projectionMatrix, 2.0f);


        }

    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
 }

int GameplayScreen::getNextScreenIndex() {
    return SCREEN_INDEX_NO_SCREEN;
}
int GameplayScreen::getPreviousScreenIndex() {
    return SCREEN_INDEX_MAINMENU;

}
void GameplayScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
        _game->onSDLEvent(evnt);
        _gui.OnSDLevent(evnt);
        switch (evnt.type) {
        case SDL_QUIT:
            onExitClicked(CEGUI::EventArgs());
            break;
        }
    }
}