#include "MainMenuScreen.h"

#include "GameplayScreen.h"
#include<iostream>
#include<SDL/SDL.h>
#include<Engine/IMainGame.h>
#include"Box.h"

#include<Engine/ResourceManager.h>
#include<random>
#include"ScreenIndices.h"


MainMenuScreen::MainMenuScreen(Engine::Window* window) {
    _window = window;
    _screenIndex = SCREEN_INDEX_MAINMENU;
}
MainMenuScreen::~MainMenuScreen() {

}

void MainMenuScreen::build() {

}
void MainMenuScreen::destroy() {

}

void MainMenuScreen::onEntry() {

    

    _mouseLight.init(3.5f,
        glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f),
        glm::vec2(0.0f, 0.0f),//_camera.ScreenToScene(_game->inputManager.getMouseCoords()),
        Engine::Vertex::ColorRGBA8(255, 255, 255, 255)
    );

    // light program
    _lightProgram.compileShaders("./Shaders/lightShading.vert", "./Shaders/lightShading.frag");
    _lightProgram.addAttribute("vertexPosition");
    _lightProgram.addAttribute("vertexColor");
    _lightProgram.addAttribute("vertexUV");
    _lightProgram.linkShaders();

    //init camera
    _camera.init(_window->getScreenWidth(), _window->getScreenHeight());
    _camera.setScale(32.0f); //32. pixels per meter


    initUI();
}
void MainMenuScreen::initUI() {
    _gui.init("GUI");
    _gui.loadScheme("TaharezLook.scheme");
    _gui.loadScheme("AlfiskoSkin.scheme");
    _gui.setFont("DejaVuSans-10");
    _gui.setMouseCursor("TaharezLook/MouseArrow");

    CEGUI::PushButton* playGameButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.45f, 0.45f, 0.1f, 0.05f), glm::vec4(0.0f), "playGameButton"));
    playGameButton->setText("Play Game!");
    playGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onPlayClicked, this));

    CEGUI::PushButton* editorButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.45f, 0.55f, 0.1f, 0.05f), glm::vec4(0.0f), "editorButton"));
    editorButton->setText("Level Editor!");
    editorButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onEditorClicked, this));

    CEGUI::PushButton* exitGameButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.45f, 0.65f, 0.1f, 0.05f), glm::vec4(0.0f), "exitGameButton"));
    exitGameButton->setText("Exit Game!");
    exitGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onExitClicked, this));

    

    _gui.showMouseCursor();
    SDL_ShowCursor(0);
}
bool MainMenuScreen::onExitClicked(const CEGUI::EventArgs& e) {
    _currentState = Engine::ScreenState::EXIT_APPLICATION;
    return true;
}
bool MainMenuScreen::onPlayClicked(const CEGUI::EventArgs& e) {
    _nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
    _currentState = Engine::ScreenState::CHANGE_NEXT;
    return true;
}
bool MainMenuScreen::onEditorClicked(const CEGUI::EventArgs& e) {
    _nextScreenIndex = SCREEN_INDEX_EDITOR;
    _currentState = Engine::ScreenState::CHANGE_NEXT;
    return true;
}

void MainMenuScreen::onExit() {
    _gui.destroy();
    _currentState = Engine::ScreenState::EXIT_APPLICATION;
}

void MainMenuScreen::update() {
    _camera.update();

    checkInput();

}
void MainMenuScreen::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    _gui.draw();
 

}

int MainMenuScreen::getNextScreenIndex() {
    return _nextScreenIndex;
    //return SCREEN_INDEX_GAMEPLAY;
}
int MainMenuScreen::getPreviousScreenIndex() {
    return SCREEN_INDEX_NO_SCREEN;

}
void MainMenuScreen::checkInput() {
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