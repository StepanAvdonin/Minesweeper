#include <SFML/Graphics.hpp>
#include <vector>
#include <Windows.h>
#include <random>
#define CELL_SIZE 30      
#define COLUMNS 30
#define ROWS 30
#define MINES 50

class Cell
{
public:
    Cell(sf::Vector2i x_y) 
    {
        this->x_y = x_y;
        isOpen = false;
        isFlagged = false;
        isMine = false;
        minesAround = 0;
    }  

     int getType() // for Draw         // designations of types:
    {                     // 0 .. 8 - mines_around
         if (isOpen)                   // 9 - flag
         {    
             if (isMine)                // Test showing of mines
                 return 12;
             else                       // 10 - closed cell
             return minesAround;       // 11 - mine
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

     void addMinesAround()
     {
         minesAround++;
     }

     int getMinesAround()
     {
             return minesAround;
     }

     bool getMine()
     {
         return isMine;
     }

     bool getOpen()
     {
         return isOpen;
     }

private:
    bool isFlagged;
    bool isMine;
    bool isOpen;

    int minesAround;

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
                Cell temp = cells.at(ROWS* i + j);
                type = temp.getType();
                cell_sprt.setTextureRect(sf::IntRect(type*30, 0, 30, 30));
                cell_sprt.setPosition((CELL_SIZE + 1) * i, (CELL_SIZE+1) * j);
                window.draw(cell_sprt);
            }
        }
    }

    void openCell(sf::Vector2i temp)
    {
        getCell(temp.x, temp.y)->open();
    }

    Cell* getCell(int x, int y)
    {
        return &cells.at(x * COLUMNS + y);
    }
         
    void flagCell(sf::Vector2i temp)
    {
        getCell(temp.x, temp.y)->setFlag();
    }

    void setMines(sf::Vector2i firstClick)
    {
        std::vector<sf::Vector2i> mines;
        sf::Vector2i temp;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist1(0, COLUMNS-1);
        std::uniform_int_distribution<> dist2(0, ROWS-1);
    
        temp.x = dist1(gen);       
        temp.y = dist2(gen);

        while (MINES > mines.size())      // getting unic coordinates of mines
        {
            bool stop = false;

            if ((temp == firstClick))
            {
                temp.x = dist1(gen);
                temp.y = dist2(gen);
                continue;
            }

            for (auto iter : mines)
            {

                if ((iter == temp))
                {
                    temp.x = dist1(gen);
                    temp.y = dist2(gen);
                    stop = true;
                    break;
                } 
            }
            if (stop)
                continue;

            mines.push_back(temp);
            getCell(temp.x, temp.y)->setMine();
        } 

        for (auto &iter : cells)   // add mines around
        {
            int a = iter.getCoordinates().x;
            int b = iter.getCoordinates().y;
            if(getCell(a,b)->getMine())
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (a+i < 0 || b+j < 0 || a+i == COLUMNS || b+j == ROWS || i == 0 && j == 0 )
                    {
                        continue;
                    }
                    getCell(a+i, b+j)->addMinesAround();
                }
            }
        }
    }

    void autoOpen()
    {
        for (auto& iter : cells)
        {
            int a = iter.getCoordinates().x;
            int b = iter.getCoordinates().y;
            if (getCell(a, b)->getOpen() && (getCell(a, b)->getMinesAround() == 0))
                for (int i = -1; i < 2; i++)
                {
                    for (int j = -1; j < 2; j++)
                    {
                        if (a + i < 0 || b + j < 0 || a + i == COLUMNS || b + j == ROWS || i == 0 && j == 0)
                        {
                            continue;
                        }
                        getCell(a + i, b + j)->open();
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
                    sf::Vector2i position(event.mouseButton.x / (CELL_SIZE + 1),event.mouseButton.y / (CELL_SIZE + 1));
                    field.openCell(position);
                    if (!started)
                    {
                        field.setMines(position);
                        started = true;
                        break;
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i position(event.mouseButton.x / (CELL_SIZE + 1), event.mouseButton.y / (CELL_SIZE + 1));
                    field.flagCell(position);
                }
            }
            
        }
        field.autoOpen();
        field.draw(window);
        //Sleep(100);

        window.display();
        window.clear();
    }

    return 0;
}