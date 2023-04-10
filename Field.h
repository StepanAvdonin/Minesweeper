#pragma once
#include "Cell.h"
#include <random>
#define COLUMNS 10  
#define ROWS 10
#define CELL_SIZE 30 
#define MINES 5
#define HAT 60

class Field
{
public:
    Field();

    void draw(sf::RenderWindow& window);

    void openAll();

    void openCell(sf::Vector2i temp);

    Cell* getCell(int x, int y);

    void flagCell(sf::Vector2i temp);

    void setMines(sf::Vector2i firstClick);

    void autoOpen();

    void stopGame();

    void reset();

private:
    std::vector<Cell> cells;
};