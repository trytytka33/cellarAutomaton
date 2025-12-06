#ifndef SIMULATION_H
#define SIMULATION_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <algorithm>

#include"loadMap.h"

enum BoundryType {
    PERIODIC
};

enum Rule {
    CONWAY
};

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    bool isHovered;
    bool isPressed;
    std::function<void()> onClick;

    Button(sf::Vector2f pos, sf::Vector2f size, const std::string& label, 
           sf::Font& font, std::function<void()> callback);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update();
    void draw(sf::RenderWindow& window);
    void updateTextPosition();  // DODANE
    void setPosition(sf::Vector2f pos);  // DODANE

    LoadMap map;
};

class Simulation {
private:
    sf::RenderWindow window;
    std::vector<std::vector<bool>> grid;
    std::vector<std::vector<bool>> obstacles;
    bool isRunning;
    sf::Clock clock;
    float updateInterval;
    sf::Font font;
    BoundryType boundryType;
    Rule rule;
    int nrCykluSymulacji;
    int sidebarWidth;
    int statsWidth;
    float cellSize;
    unsigned int windowWidth;
    unsigned int windowHeight;
    float gridOffsetX;
    std::vector<Button> buttons;
    std::string currentMapName;
    int panelHeight;  // DODANE
    float gridDisplayHeight;  // DODANE
    
public:
    Simulation();
    void setCellAlive(int x, int y, bool alive);
    void toggleCell(int x, int y);
    void clearGrid();
    void clearObstacles();
    void setRandom(int density);
    void setBlock(int startX, int startY);  // DODANE
    void handleInput();
    int countNeighbors(int x, int y);
    void updateGameOfLife();
    void update();
    void draw();
    void run();
    void initButtons();
    void updateWindowDimensions();  // DODANE
    bool loadMapFromFile(const std::string& filename);
    
    void onStartStop();
    void onClear();
    void onMapIsrael();
    void onMapPoland();
    void onMapUSA();
    void onMapItaly();
    void onLoadCustomMap();
    void onRandom15();
    void onRandom30();
    void onRandom50();
    void onSpeedUp();
    void onSlowDown();
};

#endif
