#pragma once
#include <SFML/Graphics.hpp>

class LoadMap {
private:
	sf::Texture map;
	sf::Sprite background;

public:
	LoadMap() {}
	LoadMap(int width, int height);

	sf::Sprite getBackground() {
		return background;
	}
};