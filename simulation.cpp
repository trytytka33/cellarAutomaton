#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

const int SPACE_WIDTH = 100;
const int SPACE_HEIGHT = 100;
const int CELL_SIZE = 6;
const int WINDOW_WIDTH = SPACE_WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = SPACE_HEIGHT * CELL_SIZE + 100;

enum BoundryType {
    PERIODIC,
};

enum Rule{
    CONWAY,
};

class virusSimulation {
private:
    sf::RenderWindow window;
    std::vector<std::vector<bool>> grid;
    bool isRunning;
    sf::Clock clock;
    float updateInterval;
    sf::Font font;
    BoundryType boundryType;
    Rule rule;
    int nrCykluSymulacji;
    int panelHeight;
    
    float cellSize;
    int windowWidth;
    int windowHeight;
    int gridDisplayHeight;
    
public:
    virusSimulation() 
        : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Virus Spread - Cellar Automaton", sf::Style::Default),
          grid(SPACE_HEIGHT, std::vector<bool>(SPACE_WIDTH, false)),
          isRunning(false),
          updateInterval(0.1f),
          boundryType(PERIODIC),
          rule(CONWAY),
          nrCykluSymulacji(0),
          panelHeight(100),
          windowWidth(WINDOW_WIDTH),
          windowHeight(WINDOW_HEIGHT) {
        
        window.setFramerateLimit(60);
        srand(static_cast<unsigned>(time(nullptr)));
        
        updateWindowDimensions();
        
    }
    
    void updateWindowDimensions() {
        gridDisplayHeight = windowHeight - panelHeight;
        
        float cellWidthSize = static_cast<float>(windowWidth) / SPACE_WIDTH;
        float cellHeightSize = static_cast<float>(gridDisplayHeight) / SPACE_HEIGHT;
        cellSize = std::min(cellWidthSize, cellHeightSize);
    }
    
    void toggleCell(int x, int y) {
        if (x >= 0 && x < SPACE_WIDTH && y >= 0 && y < SPACE_HEIGHT) {
            grid[y][x] = !grid[y][x];
        }
    }
    
    void clearGrid() {
        for (int y = 0; y < SPACE_HEIGHT; y++) {
            for (int x = 0; x < SPACE_WIDTH; x++) {
                grid[y][x] = false;
            }
        }
        nrCykluSymulacji = 0;
    }
   
    void setBlock(int startX, int startY) {
        clearGrid();
        setCellAlive(startX, startY, true);
        setCellAlive(startX + 1, startY, true);
        setCellAlive(startX, startY + 1, true);
        setCellAlive(startX + 1, startY + 1, true);
    }
    
    void setRandom(int density = 15) {
        clearGrid();
        for (int y = 0; y < SPACE_HEIGHT; y++) {
            for (int x = 0; x < SPACE_WIDTH; x++) {
                grid[y][x] = (rand() % 100 < density);
            }
        }
    }
    
    void handleInput() {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                windowWidth = resized->size.x;
                windowHeight = resized->size.y;
                gridDisplayHeight = windowHeight - panelHeight;

                float cellWidthSize = static_cast<float>(windowWidth) / SPACE_WIDTH;
                float cellHeightSize = static_cast<float>(gridDisplayHeight) / SPACE_HEIGHT;
                cellSize = std::min(cellWidthSize, cellHeightSize);
            }
            
            if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Space:
                        isRunning = !isRunning;
                        break;
                    case sf::Keyboard::Key::C:
                        clearGrid();
                        break;
                    case sf::Keyboard::Key::Num2:
                        setBlock(SPACE_WIDTH/2, SPACE_HEIGHT/2);
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
            
            if (event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int gridX = static_cast<int>(mousePos.x / cellSize);
                int gridY = static_cast<int>(mousePos.y / cellSize);
                if (gridY < SPACE_HEIGHT && gridX < SPACE_WIDTH) {
                    toggleCell(gridX, gridY);
                }
            }
        }
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int gridX = static_cast<int>(mousePos.x / cellSize);
            int gridY = static_cast<int>(mousePos.y / cellSize);
            if (gridX >= 0 && gridX < SPACE_WIDTH && gridY >= 0 && gridY < SPACE_HEIGHT && mousePos.y < gridDisplayHeight) {
                grid[gridY][gridX] = true;
            }
        }
    }
    
    int countNeighbors(int x, int y) {
        int count = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                
                int nowy_x = x + dx;
                int nowy_y = y + dy;
                
                if (boundryType == PERIODIC) {
                    nowy_x = ((nowy_x % SPACE_WIDTH) + SPACE_WIDTH) % SPACE_WIDTH;
                    nowy_y = ((nowy_y % SPACE_HEIGHT) + SPACE_HEIGHT) % SPACE_HEIGHT;
                } else {
                    if (nowy_x < 0 || nowy_x >= SPACE_WIDTH || nowy_y < 0 || nowy_y >= SPACE_HEIGHT) {
                        continue;
                    }
                }
                if (nowy_x >= 0 && nowy_x < SPACE_WIDTH && nowy_y >= 0 && nowy_y < SPACE_HEIGHT) {
                    if (grid[nowy_y][nowy_x]) count++;
                }
            }
        }
        return count;
    }
    
    void updateGameOfLife() {
        std::vector<std::vector<bool>> newGrid = grid;
        
        for (int y = 0; y < SPACE_HEIGHT; y++) {
            for (int x = 0; x < SPACE_WIDTH; x++) {
                int neighbors = countNeighbors(x, y);
                bool currentState = grid[y][x];
                
                if (rule== CONWAY) {
                    if (currentState) {
                        newGrid[y][x] = (neighbors == 2 || neighbors == 3);
                    } else {
                        newGrid[y][x] = (neighbors == 3);
                    }
                }
            }
        }
        
        grid = newGrid;
        nrCykluSymulacji++;
    }
    
    void update() {
        if (isRunning && clock.getElapsedTime().asSeconds() >= updateInterval) {
            updateGameOfLife();
            clock.restart();
        }
    }
    
    void draw() {
        window.clear(sf::Color(15, 15, 15));
        
        for (int y = 0; y < SPACE_HEIGHT; y++) {
            for (int x = 0; x < SPACE_WIDTH; x++) {
                if (grid[y][x]) {
                    sf::RectangleShape cell({cellSize - 1.0f, cellSize - 1.0f});
                    cell.setPosition({x * cellSize, y * cellSize});
                    cell.setFillColor(sf::Color::White);
                    window.draw(cell);
                }
            }
        }
        
        sf::RectangleShape panel({static_cast<float>(windowWidth), static_cast<float>(panelHeight)});
        panel.setPosition({0, static_cast<float>(gridDisplayHeight)});
        panel.setFillColor(sf::Color(30, 30, 30));
        window.draw(panel);
        
        window.display();
    }
    
    void run() {
        while (window.isOpen()) {
            handleInput();
            update();
            draw();
        }
    }
    
    void setCellAlive(int x, int y, bool alive) {
        if (x >= 0 && x < SPACE_WIDTH && y >= 0 && y < SPACE_HEIGHT) {
            grid[y][x] = alive;
        }
    }
};

int main() {
    virusSimulation simulation;
    simulation.run();
    return 0;
}
