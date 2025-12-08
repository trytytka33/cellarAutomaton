#include "LoadMap.h"

LoadMap::LoadMap(int width, int height) : background(map) {
	try {
		map.loadFromFile("map.png");
	}
	catch (sf::Exception e) {
		e.what();
	}

	background.setTexture(map, true);
	background.setScale({0.5f, 0.5f});
	background.setPosition({(float)width/8, (float)height/8});
}
