#include <SFML/Graphics.hpp>
#include <vector>
#include <Windows.h>
#define CELL_SIZE 30
#define COLUMNS 30
#define ROWS 30


class Cell
{
public:
    Cell(int x, int y) 
    {
        this->x = x;
        this->y = y;
        is_open = false;
        is_flagged = false;
    }

     int GetType()                     // designations of types:
    {                                  // 0 .. 8 - mines_around
         if (is_open)                  // 9 - flag
         {                             // 10 - closed cell
             return mines_around;      
         }                             
         else if(is_flagged)
         {
             return 9;
         }
         else
         {
             return 10;
         }
    }

private:
    bool is_flagged;
    bool is_mine;
    bool is_open;
    bool mouse_alloc;

    int mines_around;

    int x;
    int y;
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
                cells.push_back(Cell(j, i));
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
                Cell temp = cells.at(CELL_SIZE * i + j);
                type = temp.GetType();
                cell_sprt.setTextureRect(sf::IntRect(type*30, 0, 30, 30));
                cell_sprt.setPosition((CELL_SIZE + 1) * i, (CELL_SIZE+1) * j);
                window.draw(cell_sprt);
            }
        }
    }
private:
    std::vector<Cell> cells;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLUMNS*CELL_SIZE- 1, ROWS * CELL_SIZE-1), "Minesweeper");
    Field field;
   
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vector2i position;
        position = sf::Mouse::getPosition(window);
        position / CELL_SIZE;
        field.draw(window);
        Sleep(1000);

        window.display();
        window.clear();
    }

    return 0;
}