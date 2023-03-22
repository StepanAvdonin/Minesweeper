#include <SFML/Graphics.hpp>
#include <vector>
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
        sf::RectangleShape cell_shape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        cell_shape.setFillColor(sf::Color(255, 255, 255, 200));
        for (size_t i = 0; i < COLUMNS; i++)
        {
            for (size_t j = 0; j < ROWS; j++)
            {
                cell_shape.setPosition(CELL_SIZE * i, CELL_SIZE * j);
                window.draw(cell_shape);
            }
        }
    }
private:
    std::vector<Cell> cells;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLUMNS*CELL_SIZE, ROWS * CELL_SIZE), "Minesweeper");
    Field field;
   
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        

        field.draw(window);

        window.display();
        window.clear();
    }

    return 0;
}