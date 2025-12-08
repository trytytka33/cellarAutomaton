#include "LoadMap.h"

LoadMap::LoadMap(int width, int height) : background(map) {
	try {
		map.loadFromFile("map.png", false, sf::IntRect({ 0, 0 }, { width, height }));
	}
	catch (sf::Exception e) {
		e.what();
	}

	background.setTexture(map, true);

}
