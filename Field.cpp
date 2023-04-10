#include "Field.h"

Field::Field()
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

void Field::draw(sf::RenderWindow& window)
{
    sf::Texture hatTexture;
    hatTexture.loadFromFile("Hat.png");

    sf::Sprite hatSprt;
    hatSprt.setTexture(hatTexture);

    sf::Sprite hatReset;
    hatReset.setTexture(hatTexture);
    hatReset.setTextureRect(sf::IntRect(0, 50, 50, 50));

    sf::RectangleShape hatBack;
    sf::Vector2f hatSize = { (float)COLUMNS * (CELL_SIZE + 1), HAT };
    hatBack.setSize(hatSize);
    hatBack.setFillColor(sf::Color(255, 255, 255, 190));
    hatBack.setOutlineColor(sf::Color(255, 255, 255, 150));
    hatBack.setOutlineThickness(-5);

    window.draw(hatBack);

    hatReset.setPosition(COLUMNS * (CELL_SIZE + 1) / 2 - 25, 5);
    window.draw(hatReset);

    int number = 136;

    for (int i = 2; i > -1; i--)
    {

        int counter = number % (10);
        hatSprt.setTextureRect(sf::IntRect(counter * 26, 0, 26, 50));
        hatSprt.setPosition(5 + 26 * i, 5);
        window.draw(hatSprt);
        number /= 10;
    }

    sf::Texture cellTexture;
    cellTexture.loadFromFile("Cells.png");

    sf::Sprite cellSprt;
    cellSprt.setTexture(cellTexture);

    int type = 0;
    for (size_t i = 0; i < COLUMNS; i++)
    {
        for (size_t j = 0; j < ROWS; j++)
        {
            Cell temp = cells.at(ROWS * i + j);
            type = temp.getType();
            cellSprt.setTextureRect(sf::IntRect(type * 30, 0, 30, 30));
            cellSprt.setPosition((CELL_SIZE + 1) * i, (CELL_SIZE + 1) * j + HAT);
            window.draw(cellSprt);
        }
    }
}

void Field::openAll()
{
    for (auto& iter : cells)
    {
        iter.open();
    }
}

void Field::openCell(sf::Vector2i temp)
{
    getCell(temp.x, temp.y)->open();
}

Cell* Field::getCell(int x, int y)
{
    return &cells.at(x * COLUMNS + y);
}

void Field::flagCell(sf::Vector2i temp)
{
    getCell(temp.x, temp.y)->setFlag();
}

void Field::setMines(sf::Vector2i firstClick)
{
    std::vector<sf::Vector2i> mines;
    sf::Vector2i temp;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist1(0, COLUMNS - 1);
    std::uniform_int_distribution<> dist2(0, ROWS - 1);

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

    for (auto& iter : cells)   // add mines around
    {
        int a = iter.getCoordinates().x;
        int b = iter.getCoordinates().y;
        if (getCell(a, b)->getMine())
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (a + i < 0 || b + j < 0 || a + i == COLUMNS || b + j == ROWS || i == 0 && j == 0)
                    {
                        continue;
                    }
                    getCell(a + i, b + j)->addMinesAround();
                }
            }
    }
}

void Field::autoOpen()
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

void Field::stopGame()
{
    int notMines = COLUMNS * ROWS - MINES;
    for (auto iter : cells)
    {
        if (iter.getOpen())
            notMines--;
    }
    if (notMines == 0)
    {
        win = true;
        finish = true;
    }
}

void Field::reset()
{
    for (auto& iter : cells)
    {
        iter.reset();
    }
}