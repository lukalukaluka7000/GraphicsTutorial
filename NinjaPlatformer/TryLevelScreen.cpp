#include "TryLevelScreen.h"
#include<iostream>
#include<SDL/SDL.h>
#include<Engine/IMainGame.h>
#include"Box.h"

#include<Engine/ResourceManager.h>
#include<random>
#include"ScreenIndices.h"

#include"EditorScreen.h"

TryLevelScreen::TryLevelScreen(Engine::Window* window) {
	_window = window;
	_screenIndex = SCREEN_INDEX_TRYLEVEL;
}
TryLevelScreen::~TryLevelScreen() {

}

void TryLevelScreen::build() {

}
void TryLevelScreen::destroy(){

}
void TryLevelScreen::readFromFileAndInit(const std::string& fileName) {
    Engine::GLTexture texture = Engine::ResourceManager::getTexture("Assets/bricks_top.png");
    Engine::TileSheet playerSheets;
    playerSheets._texture = Engine::ResourceManager::getTexture("Assets/blue_ninja.png");

    std::string line;
    std::ifstream file("./Levels/tempLevel.txt");
    if (!file) std::cerr << "Could not open file for read!" << std::endl;
  
    if (file.is_open()) {
        while (std::getline(file, line)) 
        {
            _levelData.push_back(line);
        }
        file.close();
    }
    for (auto& l : _levelData) {
        std::cout << l << std::endl;
    }
    
    std::vector<std::string> boxes;
    std::vector<std::string> player;
    std::vector<std::string> lights;


    int NUM_BOX_PL_LIG[3];
    int num,j=0;
    float xPos, yPos, xDim, yDim;
    int r, g, b;

    //first of all find better way to this get the number of elements than loop ... with getline

    // get the numbers of elements from file, store them to NUM_BOX_PL_LIG
    for (int i = 0; i < _levelData.size(); i++) {
        int k = sscanf_s(_levelData[i].c_str(), "%d %f %f %f %d %d %d", &num, &yPos, &xDim, &yDim, &r, &g, &b);
        //if sscanf returns 1 it means we read the number of elem
        if (k == 1) {
            NUM_BOX_PL_LIG[j] = num;
            j++;
        }
    }
    
    // process levelData
    int currentObject = 0; // objects: boxes, player, lights
    for (int i = 0; i < _levelData.size(); i++) {
        if (currentObject == 0) {
            for (int j = 0; j < NUM_BOX_PL_LIG[0]; j++) {
                if (sscanf_s(_levelData[i + j].c_str(), "%f %f %f %f %d %d %d", &xPos, &yPos, &xDim, &yDim, &r, &g, &b) == 7) {
                    Box newBox;
                    Engine::Vertex::ColorRGBA8 clr;
                    clr.r = r;
                    clr.g = g;
                    clr.b = b;
                    clr.a = 255;
                    newBox.init(_world.get(),
                        glm::vec2(xPos, yPos),
                        glm::vec2(xDim, yDim),
                        clr.getVectorWithSameColor(clr),
                        texture);
                    _boxes.push_back(newBox);
                }
            }
            i = i + NUM_BOX_PL_LIG[0];
            currentObject++;
        }
        else if (currentObject == 1) {
            sscanf_s(_levelData[i].c_str(), "%f %f %f %f %d %d %d", &xPos, &yPos, &xDim, &yDim, &r, &g, &b);
            _playerColor.SetColor(r, g, b, 255);
            _player.init(_world.get(),
                glm::vec2(0.0f, 8.0f),
                glm::vec2(2.0f),   // drawdims  Player 
                glm::vec2(1.0f, 1.8f),  //collision dimensions - dimensions of capsule
                _playerColor.getVectorWithSameColor(_playerColor),
                playerSheets);
            i = i + NUM_BOX_PL_LIG[1];
            currentObject++;
        }
        else if (currentObject == 2) {
            for (int j = 0; j < NUM_BOX_PL_LIG[2]; j++) {
                if (sscanf_s(_levelData[i + j].c_str(), "%f %f %f %f %d %d %d", &xPos, &yPos, &xDim, &yDim, &r, &g, &b) == 7) {
                    Light newLight;
                    Engine::Vertex::ColorRGBA8 clr;
                    clr.r = r;
                    clr.g = g;
                    clr.b = b;
                    clr.a = 255;
                    newLight.init(xDim * 2,
                        glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f),
                        glm::vec2(xPos, yPos),
                        clr);
                    _lights.push_back(newLight);
                }
            }
            i = i + NUM_BOX_PL_LIG[2];
            currentObject++;
        }
        
    }
}
void TryLevelScreen::onEntry() {
    //pandan za glm::vec2 b2vec2
    b2Vec2 gravity(0.0f, -24.81f);
    //create world
    _world = std::make_unique<b2World>(gravity);

    //GROUND
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -21.0f);
    b2Body* groundBody = _world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f); //density static object

    readFromFileAndInit("Levels/tempLevel.txt");



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
    _camera.setScale(32.0f);
	
}
void TryLevelScreen::onExit() {
	_player.destroy(_world.get());
	_boxes.clear();
	_world.reset();
    _AbortMission = true;
	_currentState = Engine::ScreenState::EXIT_APPLICATION;
}

void TryLevelScreen::update() {
	_camera.update();
	_game->inputManager.update();

	checkInput();

    if (!_AbortMission) {
        _player.update(_game->inputManager);

        glm::vec2 updatedPosition(_player.getCapsule().getBody()->GetPosition().x, _player.getCapsule().getBody()->GetPosition().y);

        _world->Step(1.0f / 60.0f, 6, 2);
    }
}
void TryLevelScreen::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);


    if (!_AbortMission) {
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
    }
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int TryLevelScreen::getNextScreenIndex() {
	return SCREEN_INDEX_NO_SCREEN;
}
int TryLevelScreen::getPreviousScreenIndex() {
	return SCREEN_INDEX_EDITOR;
}
void TryLevelScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		_game->onSDLEvent(evnt);
		switch (evnt.type) {
		case SDL_QUIT:
			//onExitClicked(CEGUI::EventArgs());
			break;
		}
	}
}
