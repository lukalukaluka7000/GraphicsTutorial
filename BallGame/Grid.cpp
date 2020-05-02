#include "Grid.h"

#include <cmath>
#include <cstdlib>

Grid::Grid(int width, int height, int cellSize) :
    m_width(width),
    m_height(height),
    m_cellSize(cellSize) {

    //sa ceil smo uvik sigurni da imamo vise ili jednako
    m_numXCells = (int)ceil((float)m_width / m_cellSize);
    m_numYCells = (int)ceil((float)m_height / m_cellSize);

    // Allocate all the cells


    m_cells.resize(m_numYCells * m_numXCells);


    const int BALLS_TO_RESERVE = 20;
    for (size_t i = 0; i < m_cells.size(); i++) {
    
        m_cells[i].balls.reserve(BALLS_TO_RESERVE);
    }

}

Grid::~Grid() {
}

Cell* Grid::getCell(int x, int y) {
    if (x < 0) x = 0;
    if (x >= m_numXCells) 
        x = m_numXCells - 1;

    if (y < 0) y = 0;

    if (y >= m_numYCells) 
        y = m_numYCells - 1;


    return &m_cells[y * m_numXCells + x];
}



Cell* Grid::getCell(const glm::vec2& pos) {
    int cellX = (int)(pos.x / m_cellSize);
    int cellY = (int)(pos.y / m_cellSize);

    return getCell(cellX, cellY);
}



void Grid::addBall(Ball* ball) {

    //get cell at that location
    Cell* tajCell = getCell(ball->position);

    tajCell->balls.push_back(ball);
    


    ball->ownerCell = tajCell;
    ball->cellVectorIndex = tajCell->balls.size() - 1; //its position in the vector 
}


void Grid::addBall(Ball* ball, Cell* cell) {
    cell->balls.push_back(ball);
    ball->ownerCell = cell;

    //KA DA znamo di je odma jer je to vektor pointera i ne tribas trazit nego odma znas di sam 
    ball->cellVectorIndex = cell->balls.size() - 1;
}



void Grid::removeBallFromCell(Ball* ball) {

    //ovo je samo trick
    std::vector<Ball*>& balls = ball->ownerCell->balls;
    
    
    // Normal vector swap
    balls[ball->cellVectorIndex] = balls.back();
    balls.pop_back();

    // Update vector index
    if ((size_t)ball->cellVectorIndex < balls.size()) {

        balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;

    }
    // Set the index of ball to -1
    ball->cellVectorIndex = -1;
    ball->ownerCell = nullptr;
}
