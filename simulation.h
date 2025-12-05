#ifndef SIMULATION.H
#define SIMULATION.H
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
enum BoundryType {
    PERIODIC,
};

enum Rule{
    CONWAY,
};
class Simulation {
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
    Simulation();
    void updateWindowDimensions();
    void setCellAlive(int x, int y, bool alive);
    void toggleCell(int x, int y);
    void clearGrid();
    void setBlock(int startX, int startY);
    void setRandom(int density = 15);
    void handleInput();
    int countNeighbors(int x, int y);
    void updateGameOfLife();
    void update();
    void draw();
    void run();
};
#endif