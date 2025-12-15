# Cellular Automaton Simulation

A C++ simulation using SFML that visualizes cellular automaton with cities and obstacles.

## Requirements

- SFML library
- C++17 or later
- CMake (optional)

## Building
```bash
g++ -std=c++17 main.cpp Simulation.cpp Button.cpp -lsfml-graphics -lsfml-window -lsfml-system -o simulation
```

## Running
```bash
./simulation
```

## Features

- Grid-based cellular automaton
- Interactive sidebar with buttons
- City markers
- Obstacle placement
- Pause/play functionality
- Adjustable cell visualization

## Controls

Use the sidebar buttons to control the simulation.

## Project Structure

- `Simulation.cpp/h` - Main simulation logic
- `Button.cpp/h` - UI button implementation
- LoadMap.cpp/h - Map loading logic
- `main.cpp` - Entry point
