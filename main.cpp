#include "Cell.h"
#include "Field.h"
#include "windows.h"
#define CELL_SIZE 30      
#define COLUMNS 10
#define ROWS 10
#define MINES 5
#define HAT 60

bool finish = false;
bool win = false;

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLUMNS*(CELL_SIZE+1), 
                                            ROWS * (CELL_SIZE+1) + HAT), "Minesweeper");
    Field field;

    int started = false;
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
                    if (event.mouseButton.y > HAT)
                    {
                        sf::Vector2i position(event.mouseButton.x / (CELL_SIZE + 1),
                                            (event.mouseButton.y - HAT) / (CELL_SIZE + 1));
                        field.openCell(position);
                        if (!started)
                        {
                            field.setMines(position);
                            started = true;
                            break;
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i position(event.mouseButton.x / (CELL_SIZE + 1), 
                                            (event.mouseButton.y - HAT) / (CELL_SIZE + 1));
                    field.flagCell(position);
                }
            }
            
        }

        field.autoOpen();
        field.stopGame();

        if (finish)
        {
            if (!win)
            {
                field.openAll();
            }
            field.draw(window);
            window.display();
            Sleep(1000);
            bool continueGame = true;
            if (continueGame == false)
            {
                window.close();
            }
            else
            {
                field.reset();
                finish = false;
                started = false;
            }
        } 
        field.draw(window);
        window.display();
        //Sleep(100);
        window.clear();
    }
    return 0;
}