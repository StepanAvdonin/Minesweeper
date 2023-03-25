#include <SFML/Graphics.hpp>
#include <vector>
#include <Windows.h>
#include <iostream>
#define CELL_SIZE 30      
#define COLUMNS 10
#define ROWS 10
#define MINES 10

class Cell
{
public:
    Cell(sf::Vector2i x_y) 
    {
        this->x_y = x_y;
        isOpen = false;
        isFlagged = false;
        isMine = false;
        mines_around = 1; // test
    }

     int getType() // for Draw         // designations of types:
    {                                  // 0 .. 8 - mines_around
         if (isOpen)                   // 9 - flag
         {    
             if (isMine)                // Test showing of mines
                 return 12;
             else                       // 10 - closed cell
             return mines_around;       // 11 - mine
         }                              // 12 - active mine
         else if(isFlagged)
         {
             return 9;
         }
         else
         {
             return 10;
         }
    }

     sf::Vector2i getCoordinates()
     {
         return x_y;
     }

     void open()
     {
         if(!isFlagged)
         isOpen = true;
     }

     void setFlag()
     {
         if (!isOpen)
         {
             if (!isFlagged)
                 isFlagged = true;
             else if(isFlagged)
                 isFlagged = false;
         }
     }

     void setMine()
     {
         isMine = true;
     }

private:
    bool isFlagged;
    bool isMine;
    bool isOpen;

    int mines_around;

    sf::Vector2i x_y;
};

class Field
{
public:
    Field()
    {
        for (size_t i = 0; i < COLUMNS; i++)
        {
            for (size_t j = 0; j < ROWS; j++)
            {
                sf::Vector2i temp(i, j);
                cells.push_back(Cell(temp));
            }
        }
    }

    void draw(sf::RenderWindow& window)
    {
        sf::Texture texture;
        texture.loadFromFile("Cells.png");

        sf::Sprite cell_sprt;
        cell_sprt.setTexture(texture);

        int type = 0; 
        for (size_t i = 0; i < COLUMNS; i++)
        {
            for (size_t j = 0; j < ROWS; j++)
            {
                Cell temp = cells.at(/*CELL_SIZE*/ ROWS* i + j);
                type = temp.getType();
                cell_sprt.setTextureRect(sf::IntRect(type*30, 0, 30, 30));
                cell_sprt.setPosition((CELL_SIZE + 1) * i, (CELL_SIZE+1) * j);
                window.draw(cell_sprt);
            }
        }
    }

    void openCell(sf::Vector2i temp)
    {

        for (auto &iter : cells)
        {
            if (iter.getCoordinates() == temp)
            {
                iter.open();
                break;
            }
        }
    }

    void flagCell(sf::Vector2i temp)
    {

        for (auto& iter : cells)
        {
            if (iter.getCoordinates() == temp)
            {
                iter.setFlag();
                break;
            }
        }
    }

    void setMines()
    {
        std::vector<sf::Vector2i> mines;
        sf::Vector2i temp;
    
        temp.x = rand() % COLUMNS;       // Need to change random function
        temp.y = rand() % ROWS;

        while (MINES > mines.size())      // getting unic coordinates of mines
        {
            bool stop = false;
            for (auto iter : mines)
            {
                    if (iter == temp)
                    {
                        temp.x = rand() % COLUMNS;
                        temp.y = rand() % ROWS;
                        stop = true;
                        break;
                    } 
            }
            if (stop)
                continue;
            mines.push_back(temp);
        }

        for (auto iter : mines)         // record coordinates
        {
            for (auto& i : cells)
            {
                if (i.getCoordinates() == iter)
                {
                    i.setMine();
                    break;
                }
            }
        }      
    }

private:
    std::vector<Cell> cells;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLUMNS*(CELL_SIZE+1), ROWS * (CELL_SIZE+1)), "Minesweeper");
    Field field;
    field.setMines();
    
    while (window.isOpen())
    {
        for (sf::Event event; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i position(event.mouseButton.x / (CELL_SIZE + 1),event.mouseButton.y / (CELL_SIZE + 1));
                    field.openCell(position);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i position(event.mouseButton.x / (CELL_SIZE + 1), event.mouseButton.y / (CELL_SIZE + 1));
                    field.flagCell(position);
                }
            }
        }

        field.draw(window);
        //Sleep(1000);

        window.display();
        window.clear();
    }

    return 0;
}