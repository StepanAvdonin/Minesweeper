#pragma once
#include <SFML/Graphics.hpp>

extern bool finish;
extern bool win;

class Cell
{
public:
    Cell(sf::Vector2i x_y);

    int getType();

    sf::Vector2i getCoordinates();

    void open();

    void setFlag();

    void setMine();

    void addMinesAround();

    int getMinesAround();

    bool getMine();

    bool getOpen();

    void reset();

private:
    bool isFlagged;
    bool isMine;
    bool isOpen;

    int minesAround;

    sf::Vector2i x_y;
};