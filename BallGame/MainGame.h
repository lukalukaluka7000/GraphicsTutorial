#pragma once

#include <Engine/Camera2D.h>
#include <Engine/SpriteBatch.h>
#include <Engine/InputManager.h>
#include <Engine/Window.h>
#include <Engine/GLSLProgram.h>
#include <Engine/Timing.h>
#include <Engine/SpriteFont.h>
#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
#include "Grid.h"

// TODO:
// Visualize momentum with color
// Visualize velocity with color
// Visualize position with color

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 32;

class MainGame {
public:
    ~MainGame();
    void run();


private:
    void init();
    void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    // Ball nije Ball* jer su uvik iste velicine, nema brisanja
    // ako ce se non stop mijenjat deleteted -> pointers new delete
    // ovako JE cache friendliness, ako nije pointer ovi su contiguosly u memoriji
    // update their movement extremely fast
    std::vector<Ball> m_balls;

    //treba nam konstruktor i napisali bi m_grid = nullptr;
    //ali imamo unique_ptr that will let us not have to worry about deleteing memory
    // smart pointer # include<memory>
    //std::unique_ptr<Grid> m_grid;  ======= Grid* m_grid;
    //now allocate it in initBalls() => // Initialize the grid
    //m_grid = std::make_unique<Grid>(m_screenWidth, m_screenHeight, CELL_SIZE);
    std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision


    //vector of ball renderers so we can switch from one to another
    int m_currentRenderer = 0;
    std::vector<std::unique_ptr<BallRenderer>> m_ballRenderers;

    BallController m_ballController; ///< Controls balls

    Engine::Window m_window; ///< The main window
    Engine::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<Engine::SpriteFont> m_spriteFont; ///< For font rendering
    Engine::Camera2D m_camera; ///< Renders the scene
    Engine::InputManager m_inputManager; ///< Handles input
    Engine::GLSLProgram m_textureProgram; ///< Shader for textures]

    
    Engine::FPSLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

