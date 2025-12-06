#include "simulation.h"
#include "config.h"
#include <iostream>
#include <cmath>

Button::Button(sf::Vector2f pos, sf::Vector2f size, const std::string& label, 
               sf::Font& font, std::function<void()> callback)
    : text(font),
      normalColor(70, 130, 180),
      hoverColor(100, 149, 237),
      pressedColor(65, 105, 225),
      isHovered(false),
      isPressed(false),
      onClick(callback)
{
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(normalColor);
    
    text.setString(label);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::White);
    
    updateTextPosition();
}

void Button::updateTextPosition()
{
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f shapePos = shape.getPosition();
    sf::Vector2f shapeSize = shape.getSize();
    
    text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                    textBounds.position.y + textBounds.size.y / 2.0f});
    text.setPosition({shapePos.x + shapeSize.x / 2.0f, shapePos.y + shapeSize.y / 2.0f});
}

void Button::setPosition(sf::Vector2f pos)
{
    shape.setPosition(pos);
    updateTextPosition();
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            
            if (shape.getGlobalBounds().contains(mousePosF))
            {
                isPressed = true;
            }
        }
    }
    
    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left)
        {
            if (isPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                
                if (shape.getGlobalBounds().contains(mousePosF))
                {
                    if (onClick)
                    {
                        onClick();
                    }
                }
                isPressed = false;
            }
        }
    }
    
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mousePosF(static_cast<float>(mouseMoved->position.x), 
                               static_cast<float>(mouseMoved->position.y));
        isHovered = shape.getGlobalBounds().contains(mousePosF);
    }
}

void Button::update()
{
    if (isPressed)
    {
        shape.setFillColor(pressedColor);
    }
    else if (isHovered)
    {
        shape.setFillColor(hoverColor);
    }
    else
    {
        shape.setFillColor(normalColor);
    }
}

void Button::draw(sf::RenderWindow& window)
{
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f size = shape.getSize();
    float radius = 8.0f;
    
    sf::RectangleShape mainRect({size.x - 2 * radius, size.y});
    mainRect.setPosition({pos.x + radius, pos.y});
    mainRect.setFillColor(shape.getFillColor());
    window.draw(mainRect);
    
    sf::RectangleShape vertRect({size.x, size.y - 2 * radius});
    vertRect.setPosition({pos.x, pos.y + radius});
    vertRect.setFillColor(shape.getFillColor());
    window.draw(vertRect);
    
    sf::CircleShape corner(radius);
    corner.setFillColor(shape.getFillColor());
    
    corner.setPosition({pos.x + size.x - 2 * radius, pos.y});
    window.draw(corner);
    
    corner.setPosition({pos.x, pos.y});
    window.draw(corner);
    
    corner.setPosition({pos.x + size.x - 2 * radius, pos.y + size.y - 2 * radius});
    window.draw(corner);
    
    corner.setPosition({pos.x, pos.y + size.y - 2 * radius});
    window.draw(corner);
    
    window.draw(text);
}

const int SPACE_WIDTH = 100;
const int SPACE_HEIGHT = 100;
const int CELL_SIZE = 6;
const int SIDEBAR_WIDTH = 250;
const int WINDOW_WIDTH = SPACE_WIDTH * CELL_SIZE + SIDEBAR_WIDTH + 50;
const int WINDOW_HEIGHT = SPACE_HEIGHT * CELL_SIZE + 40;

Simulation::Simulation() : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Game of Life - Virus Spread", sf::Style::Default),
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
          map(WINDOW_WIDTH, WINDOW_HEIGHT)
     {
          window.setFramerateLimit(60);
        srand(static_cast<unsigned>(time(nullptr)));
        gridDisplayHeight = windowHeight;
        cellSize = CELL_SIZE;
        gridOffsetX = SIDEBAR_WIDTH + 20;

        if (!font.openFromFile("Roboto-VariableFont_wdth,wght.ttf")) {
            if (!font.openFromFile("Roboto-VariableFont_wdth,wght.ttf")) {
                if (!font.openFromFile("arial.ttf")) {
                    std::cerr << "Nie można załadować czcionki!" << std::endl;
                }
            }
        }
        
        initButtons();
}

void Simulation::initButtons() {
    float buttonWidth = 200;
    float buttonHeight = 42;
    float spacing = 12;
    float startX = 25;
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
    if (x >= 0 && x < SPACE_WIDTH && y >= 0 && y < SPACE_HEIGHT)
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
            if (gridY >= 0 && gridY < SPACE_HEIGHT && gridX >= 0 && gridX < SPACE_WIDTH)
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
        if (gridX >= 0 && gridX < SPACE_WIDTH && gridY >= 0 && gridY < SPACE_HEIGHT)
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
                if (grid[nowy_y][nowy_x])
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

    window.draw(map.getBackground());
  
    sf::RectangleShape sidebar({static_cast<float>(sidebarWidth), static_cast<float>(windowHeight)});
    sidebar.setPosition({0, 0});
    sidebar.setFillColor(sf::Color(230, 230, 235));
    window.draw(sidebar);

    for (auto& button : buttons) {
        button.draw(window);
    }

    sf::RectangleShape gridBackground({SPACE_WIDTH * cellSize, SPACE_HEIGHT * cellSize});
    gridBackground.setPosition({gridOffsetX, 20});
    gridBackground.setFillColor(sf::Color(255, 255, 255));
    window.draw(gridBackground);

    for (int y = 0; y < SPACE_HEIGHT; y++)
    {
        for (int x = 0; x < SPACE_WIDTH; x++)
        {
            if (grid[y][x])
            {
                int hash = (x * 73 + y * 31) % 5;
                float centerX = gridOffsetX + x * cellSize + cellSize / 2.0f;
                float centerY = 20 + y * cellSize + cellSize / 2.0f;
                
                if (hash == 0)
                {
                    sf::CircleShape cell(cellSize / 2.3f);
                    cell.setPosition({centerX - cellSize / 2.3f, centerY - cellSize / 2.3f});
                    cell.setFillColor(sf::Color(220, 53, 69));
                    window.draw(cell);
                }
                else if (hash == 1)
                {
                    sf::CircleShape cell(cellSize / 2.5f, 6);
                    cell.setPosition({centerX - cellSize / 2.5f, centerY - cellSize / 2.5f});
                    cell.setFillColor(sf::Color(231, 76, 60));
                    cell.setRotation(sf::degrees(30));
                    window.draw(cell);
                }
                else if (hash == 2)
                {
                    sf::ConvexShape cell;
                    cell.setPointCount(5);
                    float size = cellSize / 2.2f;
                    cell.setPoint(0, sf::Vector2f(centerX, centerY - size));
                    cell.setPoint(1, sf::Vector2f(centerX + size * 0.95f, centerY - size * 0.31f));
                    cell.setPoint(2, sf::Vector2f(centerX + size * 0.59f, centerY + size * 0.81f));
                    cell.setPoint(3, sf::Vector2f(centerX - size * 0.59f, centerY + size * 0.81f));
                    cell.setPoint(4, sf::Vector2f(centerX - size * 0.95f, centerY - size * 0.31f));
                    cell.setFillColor(sf::Color(192, 57, 43));
                    window.draw(cell);
                }
                else if (hash == 3)
                {
                    sf::CircleShape cell(cellSize / 2.4f, 8);
                    cell.setPosition({centerX - cellSize / 2.4f, centerY - cellSize / 2.4f});
                    cell.setFillColor(sf::Color(239, 68, 68));
                    window.draw(cell);
                }
                else
                {
                    sf::CircleShape cell(cellSize / 2.6f, 4);
                    cell.setPosition({centerX - cellSize / 2.6f, centerY - cellSize / 2.6f});
                    cell.setFillColor(sf::Color(185, 50, 58));
                    cell.setRotation(sf::degrees(45));
                    window.draw(cell);
                }
            }
        }
    }

    for (int y = 0; y <= SPACE_HEIGHT; y++)
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
    }

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
