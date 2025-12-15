#ifndef SIMULATION_H
#define SIMULATION_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <algorithm>
#include "LoadMap.h"
#include "config.h"
#include "Button.h"

enum BoundryType {
    PERIODIC
};

enum Rule {
    CONWAY
};

enum Terrain {
    CITY,
    MOUNTAINS,
    DESERT,
    FOREST,
    LAKE
};
class Conditions { 
public:// new
    float population;
    float avgTemperature;
    Terrain terrain;
        Conditions(float p, float temp, Terrain terr): population(p), avgTemperature(temp), terrain(terr){}
        Conditions(): population(0.0f), avgTemperature(0.0f), terrain(CITY) {}
        
};
class State {
    std::string stateName;
    std::vector<sf::Vector2f> points;
    Conditions conditions;
    public:
        State(std::string name, std::vector<sf::Vector2f> borderPoints, Conditions conds): stateName(name), points(borderPoints), conditions(conds) {
            
        };
    const std::string& getName() const { return stateName; }
    const std::vector<sf::Vector2f>& getPoints() const { return points; }
    const Conditions& getConditions() const { return conditions; }
};

class Simulation {
private:
    sf::RenderWindow window;
    sf::Text statsText;
    sf::RectangleShape statsPanel;
    std::vector<std::vector<bool>> grid;
    std::vector<std::vector<int>> obstacles;
    std::vector<Conditions> conditions;
    std::vector<sf::CircleShape> borderElements;
    std::unordered_map<std::string, Terrain> toTerrain;
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
    int panelHeight;  
    float gridDisplayHeight;  
    LoadMap map;
    std::vector<State> states; // new 
    int aliveCells;
public:
    Simulation();
    void setCellAlive(int x, int y, bool alive);
    void toggleCell(int x, int y);
    void clearGrid();
    void clearObstacles();
    void setRandom(int density);
    void setBlock(int startX, int startY);
    void handleInput();
    int countNeighbors(int x, int y);
    void updateGameOfLife();
    void update();
    void draw();
    void run();
    void initStats();
    void updateStats();
    void initButtons();
    void updateWindowDimensions(); 
    void loadStatesFromFile(std::string fName);
     void drawCities();  // new
    void addState(const std::string& name, const std::vector<sf::Vector2f>& points, float population, float avgTemperature, Terrain terrain);
    void addConditions(const State& state);
    // bool loadMapFromFile(const std::string& filename); ???
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
