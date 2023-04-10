#include "Cell.h"

Cell::Cell(sf::Vector2i x_y)
{
    this->x_y = x_y;
    isOpen = false;
    isFlagged = false;
    isMine = false;
    minesAround = 0;
}

int Cell::getType() // for Draw         // designations of types:
{                     // 0 .. 8 - mines_around
    if (isOpen)                   // 9 - flag
    {
        if (isMine)                // Test showing of mines
            return 12;
        else                       // 10 - closed cell
            return minesAround;       // 11 - mine
    }                              // 12 - active mine
    else if (isFlagged)
    {
        return 9;
    }
    else
    {
        return 10;
    }
}

sf::Vector2i Cell::getCoordinates()
{
    return x_y;
}

void Cell::open()
{
    if (!isFlagged)
    {
        isOpen = true;
        if (isMine)
        {
            win = false;
            finish = true;
        }
    }
}

void Cell::setFlag()
{
    if (!isOpen)
    {
        if (!isFlagged)
            isFlagged = true;
        else if (isFlagged)
            isFlagged = false;
    }
}

void Cell::setMine()
{
    isMine = true;
}

void Cell::addMinesAround()
{
    minesAround++;
}

int Cell::getMinesAround()
{
    return minesAround;
}

bool Cell::getMine()
{
    return isMine;
}

bool Cell::getOpen()
{
    return isOpen;
}

void Cell::reset()
{
    isFlagged = false;
    isMine = false;
    isOpen = false;
    minesAround = 0;
}