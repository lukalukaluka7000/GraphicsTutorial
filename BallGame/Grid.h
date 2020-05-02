#pragma once

#include "Ball.h"
#include <vector>

struct Cell {
    std::vector<Ball*> balls;
};

class Grid {
    friend class BallController;
public:
    //dobijes width i height window i cellsize i onda izracunas cell
    Grid(int width, int height, int cellSize);
    ~Grid();

    /// Adds a ball and determines which cell it belongs to
    void addBall(Ball* ball);


    /// Adds a ball to the specified cell
    void addBall(Ball* ball, Cell* cell);

    /// Gets cell of x and y
    Cell* getCell(int x, int y);

    /// Gets cell based on window coordinates
    Cell* getCell(const glm::vec2& pos);

    void removeBallFromCell(Ball* ball);

private:
    // cache friendliness of array, vector stores an array 
    std::vector<Cell> m_cells;

    int m_cellSize;
    int m_width;
    int m_height;

    // broj cellova hor i vert
    int m_numXCells;
    int m_numYCells;
};
