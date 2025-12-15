#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class LoadMap {
private:
	sf::Texture map;
	sf::Sprite background;
	sf::Image mapImage;

public:
	LoadMap(int width, int height);
	LoadMap();
	sf::Sprite getBackground() {
		return background;
	}
	// funkcja do inicjalizacji macierzy granicy
	std::vector<std::vector<int>> initObstacles(int width, int height); // 0 - teren, 1 - granice, 2 - poza granicami, 3 - granice stanow
};