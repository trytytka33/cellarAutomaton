#include "Simulation.h"
#include "config.h"
#include <iostream>
#include <cmath>

const int SPACE_WIDTH = 250;
const int SPACE_HEIGHT = 200;
const int CELL_SIZE = 5;
const int SIDEBAR_WIDTH = 140;
const int WINDOW_WIDTH = SPACE_WIDTH * CELL_SIZE; // + SIDEBAR_WIDTH + 50;
const int WINDOW_HEIGHT = SPACE_HEIGHT * CELL_SIZE; // + 40;

Simulation::Simulation() : window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Game of Life - Virus Spread", sf::Style::Default),
        grid(SPACE_HEIGHT, std::vector<bool>(SPACE_WIDTH, false)),
        isRunning(false),
        updateInterval(0.1f),
        boundryType(PERIODIC),
        rule(CONWAY),
        nrCykluSymulacji(0),
        panelHeight(0),
        sidebarWidth(SIDEBAR_WIDTH),
        windowWidth(WINDOW_WIDTH),
        windowHeight(WINDOW_HEIGHT),
        map(1280, 792),
        obstacles(SPACE_HEIGHT, std::vector<int>(SPACE_WIDTH, 0)),
        conditions(SPACE_HEIGHT * SPACE_WIDTH)
     {
          window.setFramerateLimit(60);
        srand(static_cast<unsigned>(time(nullptr)));
        // ???
        gridDisplayHeight = windowHeight;
        cellSize = CELL_SIZE;
        gridOffsetX = SIDEBAR_WIDTH + 20;
        // ???
        if (!font.openFromFile(fontPathArial)) {
            if (!font.openFromFile(fontPathHelvetica)) {
                if (!font.openFromFile("arial.ttf")) {
                    std::cerr << "Nie można załadować czcionki!" << std::endl;
                }
            }
        }
        // inicjalizacja granic
        obstacles = map.initObstacles(SPACE_WIDTH, SPACE_HEIGHT);

        for (int y = 0; y < SPACE_HEIGHT; ++y) {
            for (int x = 0; x < SPACE_WIDTH; ++x) {
                if (obstacles[y][x] == 1) {
                    float centerX = gridOffsetX + x * cellSize + cellSize / 2.0f;
                    float centerY = 20 + y * cellSize + cellSize / 2.0f;
                    sf::CircleShape cell(cellSize / 2.3f);
                    cell.setPosition({ centerX - cellSize / 2.3f, centerY - cellSize / 2.3f });
                    cell.setFillColor(sf::Color(0, 0, 0));
                    borderElements.push_back(cell);
                }
            }
        }

        initButtons();
}

void Simulation::initButtons() {
    float buttonWidth = 100;
    float buttonHeight = 42;
    float spacing = 12;
    float startX = 20;
    float startY = 30;

    if (!buttons.empty()) {
        buttons[0].setPosition(sf::Vector2f(startX, startY));
        buttons[1].setPosition(sf::Vector2f(startX, startY + (buttonHeight + spacing)));
        buttons[2].setPosition(sf::Vector2f(startX, startY + (buttonHeight + spacing) * 2));
        buttons[3].setPosition(sf::Vector2f(startX, startY + (buttonHeight + spacing) * 3));
        buttons[4].setPosition(sf::Vector2f(startX, startY + (buttonHeight + spacing) * 4));
        buttons[5].setPosition(sf::Vector2f(startX, startY + (buttonHeight + spacing) * 5));
        buttons[6].setPosition(sf::Vector2f(startX, startY + (buttonHeight + spacing) * 6));
        return;
    }

    buttons.clear();

    buttons.emplace_back(
        sf::Vector2f(startX, startY),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Start/Stop",
        font,
        [this]() { isRunning = !isRunning; }
    );

    buttons.emplace_back(
        sf::Vector2f(startX, startY + (buttonHeight + spacing)),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Wyczysc",
        font,
        [this]() { clearGrid(); }
    );

    buttons.emplace_back(
        sf::Vector2f(startX, startY + (buttonHeight + spacing) * 2),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Block 2x2",
        font,
        [this]() { setBlock(SPACE_WIDTH / 2, SPACE_HEIGHT / 2); }
    );

    buttons.emplace_back(
        sf::Vector2f(startX, startY + (buttonHeight + spacing) * 3),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Random 15%",
        font,
        [this]() { setRandom(15); }
    );

    buttons.emplace_back(
        sf::Vector2f(startX, startY + (buttonHeight + spacing) * 4),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Random 30%",
        font,
        [this]() { setRandom(30); }
    );

    buttons.emplace_back(
        sf::Vector2f(startX, startY + (buttonHeight + spacing) * 5),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Szybciej",
        font,
        [this]() { updateInterval = std::max(0.01f, updateInterval - 0.02f); }
    );

    buttons.emplace_back(
        sf::Vector2f(startX, startY + (buttonHeight + spacing) * 6),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Wolniej",
        font,
        [this]() { updateInterval = std::min(1.0f, updateInterval + 0.02f); }
    );
}

void Simulation::updateWindowDimensions()
{
    gridDisplayHeight = windowHeight;
    float cellWidthSize = static_cast<float>(windowWidth - sidebarWidth - 50) / SPACE_WIDTH;
    float cellHeightSize = static_cast<float>(gridDisplayHeight - 40) / SPACE_HEIGHT;
    cellSize = std::min(cellWidthSize, cellHeightSize);
    gridOffsetX = sidebarWidth + 20;
}

void Simulation::toggleCell(int x, int y)
{
    if (x >= 0 && x < SPACE_WIDTH && y >= 0 && y < SPACE_HEIGHT && !obstacles[y][x])
    {
        grid[y][x] = !grid[y][x];
    }
}

void Simulation::clearGrid()
{
    for (int y = 0; y < SPACE_HEIGHT; y++)
    {
        for (int x = 0; x < SPACE_WIDTH; x++)
        {
            grid[y][x] = false;
        }
    }
    nrCykluSymulacji = 0;
}

void Simulation::setBlock(int startX, int startY)
{
    clearGrid();
    setCellAlive(startX, startY, true);
    setCellAlive(startX + 1, startY, true);
    setCellAlive(startX, startY + 1, true);
    setCellAlive(startX + 1, startY + 1, true);
}

void Simulation::setRandom(int density)
{
    clearGrid();
    for (int y = 0; y < SPACE_HEIGHT; y++)
    {
        for (int x = 0; x < SPACE_WIDTH; x++)
        {
            grid[y][x] = (rand() % 100 < density);
        }
    }
}

void Simulation::handleInput()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        for (auto& button : buttons) {
            button.handleEvent(*event, window);
        }

        if (const auto *resized = event->getIf<sf::Event::Resized>())
        {
            windowWidth = resized->size.x;
            windowHeight = resized->size.y;
            
            sf::View view(sf::FloatRect({0, 0}, {static_cast<float>(windowWidth), static_cast<float>(windowHeight)}));
            window.setView(view);
            
            updateWindowDimensions();
            initButtons();
        }

        if (auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            switch (keyPressed->code)
            {
            case sf::Keyboard::Key::Space:
                isRunning = !isRunning;
                break;
            case sf::Keyboard::Key::C:
                clearGrid();
                break;
            case sf::Keyboard::Key::Num2:
                setBlock(SPACE_WIDTH / 2, SPACE_HEIGHT / 2);
                break;
            case sf::Keyboard::Key::Num5:
                setRandom(15);
                break;
            case sf::Keyboard::Key::Num6:
                setRandom(30);
                break;
            case sf::Keyboard::Key::Num7:
                setRandom(50);
                break;
            case sf::Keyboard::Key::Up:
                updateInterval = std::max(0.01f, updateInterval - 0.02f);
                break;
            case sf::Keyboard::Key::Down:
                updateInterval = std::min(1.0f, updateInterval + 0.02f);
                break;
            default:
                break;
            }
        }

        if (event->getIf<sf::Event::MouseButtonPressed>())
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int gridX = static_cast<int>((mousePos.x - gridOffsetX) / cellSize);
            int gridY = static_cast<int>((mousePos.y - 20) / cellSize);
            if (gridY >= 0 && gridY < SPACE_HEIGHT && gridX >= 0 && gridX < SPACE_WIDTH && !obstacles[gridY][gridX])
            {
                toggleCell(gridX, gridY);
            }
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int gridX = static_cast<int>((mousePos.x - gridOffsetX) / cellSize);
        int gridY = static_cast<int>((mousePos.y - 20) / cellSize);
        if (gridX >= 0 && gridX < SPACE_WIDTH && gridY >= 0 && gridY < SPACE_HEIGHT && obstacles[gridY][gridX] == 0)
        {
            grid[gridY][gridX] = true;
        }
    }
}

int Simulation::countNeighbors(int x, int y)
{
    int count = 0;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;

            int nowy_x = x + dx;
            int nowy_y = y + dy;

            if (boundryType == PERIODIC)
            {
                nowy_x = ((nowy_x % SPACE_WIDTH) + SPACE_WIDTH) % SPACE_WIDTH;
                nowy_y = ((nowy_y % SPACE_HEIGHT) + SPACE_HEIGHT) % SPACE_HEIGHT;
            }
            else
            {
                if (nowy_x < 0 || nowy_x >= SPACE_WIDTH || nowy_y < 0 || nowy_y >= SPACE_HEIGHT)
                {
                    continue;
                }
            }
            if (nowy_x >= 0 && nowy_x < SPACE_WIDTH && nowy_y >= 0 && nowy_y < SPACE_HEIGHT)
            {
                if (grid[nowy_y][nowy_x] && obstacles[nowy_y][nowy_x] == 0)
                    count++;
            }
        }
    }
    return count;
}

void Simulation::updateGameOfLife()
{
    std::vector<std::vector<bool>> newGrid = grid;

    for (int y = 0; y < SPACE_HEIGHT; y++)
    {
        for (int x = 0; x < SPACE_WIDTH; x++)
        {
            int neighbors = countNeighbors(x, y);
            bool currentState = grid[y][x];

            if (rule == CONWAY)
            {
                if (currentState)
                {
                    newGrid[y][x] = (neighbors == 2 || neighbors == 3);
                }
                else
                {
                    newGrid[y][x] = (neighbors == 3);
                }
            }
        }
    }

    grid = newGrid;
    nrCykluSymulacji++;
}

void Simulation::update()
{
    if (isRunning && clock.getElapsedTime().asSeconds() >= updateInterval)
    {
        updateGameOfLife();
        clock.restart();
    }

    for (auto& button : buttons) {
        button.update();
    }
}

void Simulation::draw()
{
    window.clear(sf::Color(245, 245, 250));

    //window.draw(map.getBackground());
  
    sf::RectangleShape sidebar({static_cast<float>(sidebarWidth), static_cast<float>(windowHeight)});
    sidebar.setPosition({0, 0});
    sidebar.setFillColor(sf::Color(230, 230, 235));
    window.draw(sidebar);

    for (auto& button : buttons) {
        button.draw(window);
    }

    //sf::RectangleShape gridBackground({SPACE_WIDTH * cellSize, SPACE_HEIGHT * cellSize});
    //gridBackground.setPosition({gridOffsetX, 20});
    //gridBackground.setFillColor(sf::Color(255, 255, 255));
    //window.draw(gridBackground);

    for (int y = 0; y < SPACE_HEIGHT; y++)
    {
        for (int x = 0; x < SPACE_WIDTH; x++)
        {
            if (grid[y][x] && obstacles[y][x] == 0)
            {
                int hash = (x * 73 + y * 31) % 5;
                float centerX = gridOffsetX + x * cellSize + cellSize / 2.0f;
                float centerY = 20 + y * cellSize + cellSize / 2.0f;
                
                if (hash == 0)
                {
                    sf::CircleShape cell(cellSize / 2.3f);
                    cell.setPosition({centerX - cellSize / 2.3f, centerY - cellSize / 2.3f});
                    cell.setFillColor(sf::Color(247, 101, 101));
                    window.draw(cell);
                } 
                // ten sie zle wyswietla dla (x,0)
                else if (hash == 1)
                 {
                     sf::CircleShape cell(cellSize / 2.5f, 6);
                     cell.setPosition({centerX - cellSize / 2.5f, centerY - cellSize / 2.5f});
                     cell.setFillColor(sf::Color(107, 17, 17));
                     cell.setRotation(sf::degrees(90));
                     window.draw(cell);
                 } else if (hash == 2)
                {
                    sf::ConvexShape cell;
                    cell.setPointCount(5);
                    float size = cellSize / 2.0f;
         
                    cell.setPoint(0, sf::Vector2f(centerX, centerY - size));
                    cell.setPoint(1, sf::Vector2f(centerX + size * 0.95f, centerY - size * 0.31f));
                    cell.setPoint(2, sf::Vector2f(centerX + size * 0.59f, centerY + size * 0.81f));
                    cell.setPoint(3, sf::Vector2f(centerX - size * 0.59f, centerY + size * 0.81f));
                    cell.setPoint(4, sf::Vector2f(centerX - size * 0.95f, centerY - size * 0.31f));
                    cell.setFillColor(sf::Color(139, 0, 0));
                    window.draw(cell);
                } else if (hash == 3)
                {
                    sf::CircleShape cell(cellSize / 2.6f, 3);
                    cell.setPosition({centerX - cellSize / 2.4f, centerY - cellSize / 2.4f});
                    cell.setFillColor(sf::Color(224, 11, 11));
                    window.draw(cell);
                } 
                else
                {
                    sf::CircleShape cell(cellSize / 2.6f, 4);
                    cell.setPosition({centerX - cellSize / 2.6f, centerY - cellSize / 2.6f});
                    cell.setFillColor(sf::Color(250, 132, 132));
                    window.draw(cell);
                }
            }
        }
    }

    //rysowanie granic
    for (int i = 0; i < borderElements.size(); ++i) {
        window.draw(borderElements[i]);
    }

    /*for (int y = 0; y <= SPACE_HEIGHT; y++)
    {
        sf::RectangleShape line({SPACE_WIDTH * cellSize, 1});
        line.setPosition({gridOffsetX, 20 + y * cellSize});
        line.setFillColor(sf::Color(220, 220, 225));
        window.draw(line);
    }
    
    for (int x = 0; x <= SPACE_WIDTH; x++)
    {
        sf::RectangleShape line({1, SPACE_HEIGHT * cellSize});
        line.setPosition({gridOffsetX + x * cellSize, 20});
        line.setFillColor(sf::Color(220, 220, 225));
        window.draw(line);
    }*/

    window.display();
}




void Simulation::run()
{
    while (window.isOpen())
    {
        handleInput();
        update();
        draw();
    }
}

void Simulation::setCellAlive(int x, int y, bool alive)
{
    if (x >= 0 && x < SPACE_WIDTH && y >= 0 && y < SPACE_HEIGHT)
    {
        grid[y][x] = alive;
    }
}
