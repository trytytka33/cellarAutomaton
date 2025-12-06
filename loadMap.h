#pragma once
#include <SFML/Graphics.hpp>

class LoadMap {
private:
	sf::Texture map;
	sf::Sprite background;

public:
	LoadMap(int width, int height);
	LoadMap();
	sf::Sprite getBackground() {
		return background;
	}
};