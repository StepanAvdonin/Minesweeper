#include <SFML/Graphics.hpp>
#include <vector>
#include <Windows.h>
#define CELL_SIZE 30      
#define COLUMNS 25  
#define ROWS 25


class Cell
{
public:
    Cell(sf::Vector2i x_y) 
    {
        this->x_y = x_y;
        is_open = false;
        is_flagged = false;
        mines_around = 1; // test
    }

     int GetType() // for Draw         // designations of types:
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

     sf::Vector2i GetCoordinates()
     {
         return x_y;
     }

     void Open()
     {
         if(!is_flagged)
         is_open = true;
     }

     void SetFlag()
     {
         if (!is_open)
         {
             if (!is_flagged)
                 is_flagged = true;
             else if(is_flagged)
                 is_flagged = false;
         }
     }

private:
    bool is_flagged;
    bool is_mine;
    bool is_open;
    bool mouse_alloc;

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

    void Draw(sf::RenderWindow& window)
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
                type = temp.GetType();
                cell_sprt.setTextureRect(sf::IntRect(type*30, 0, 30, 30));
                cell_sprt.setPosition((CELL_SIZE + 1) * i, (CELL_SIZE+1) * j);
                window.draw(cell_sprt);
            }
        }
    }

    void OpenCell(sf::Vector2i temp)
    {

        for (auto &iter : cells)
        {
            if (iter.GetCoordinates() == temp)
            {
                iter.Open();
                break;
            }
        }
    }

    void FlagCell(sf::Vector2i temp)
    {

        for (auto& iter : cells)
        {
            if (iter.GetCoordinates() == temp)
            {
                iter.SetFlag();
                break;
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
                    field.OpenCell(position);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i position(event.mouseButton.x / (CELL_SIZE + 1), event.mouseButton.y / (CELL_SIZE + 1));
                    field.FlagCell(position);
                }
            }
        }

        field.Draw(window);
        //Sleep(1000);

        window.display();
        window.clear();
    }

    return 0;
}