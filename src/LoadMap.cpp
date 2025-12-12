#include "LoadMap.h"
#include <iostream>

LoadMap::LoadMap(int width, int height) : background(map) {
	try {
		mapImage.loadFromFile("map.png");
		map.loadFromImage(mapImage);

	}
	catch (sf::Exception e) {
		e.what();
	}

	background.setTexture(map, true);
	background.setScale({0.5f, 0.5f});
	background.setPosition({(float)width/8, (float)height/8});
}

// funkcja do inicjalizacji granicy macierzy
std::vector<std::vector<bool>> LoadMap::initObstacles(int width, int height) {
	std::vector<std::vector<bool>> tempGrid(height, std::vector<bool>(width, false));
	sf::Vector2u imgSize = mapImage.getSize();
	// Opcja 1 - Branie srodkowego piksela
	/*
	for (int y = 0; y < height;y++) {
		for (int x = 0; x < width; x++) {
			// skala
			unsigned int mapX = (x*imgSize.x) / width;
			unsigned int mapY = (y * imgSize.y) / height;
			sf::Color pixel = mapImage.getPixel({mapX, mapY});
			if (pixel.r < 128) {
				tempGrid[y][x] = true;
			} else {
				tempGrid[y][x] = false;
			}
		}
	}
	*/
	// Opcja 2 MultiSampling
	float stepX = (float)imgSize.x / width;
    float stepY = (float)imgSize.y / height;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			bool isObstacle = false;
			// zakres dla konkretnej komorki ktora bedzie sprawdzana
			int startX = (int)(x * stepX);
            int endX   = (int)((x + 1) * stepX);
            int startY = (int)(y * stepY);
            int endY   = (int)((y + 1) * stepY);
			if (endX > imgSize.x) endX = imgSize.x;
            if (endY > imgSize.y) endY = imgSize.y;
			// iterujemy po kazdym pikselu z wiekszego zakresu ktory bedzie skalowany
			// po to aby okreslic dokladnie czy ma byc piksel pelny czy nie
			for (int py = startY; py < endY; py++) {
                for (int px = startX; px < endX; px++) {
					sf::Color pixel = mapImage.getPixel({(unsigned int)px, (unsigned int)py});
					// if (pixel.a == 0) { // Opcja 1, tylko przezroczyste sa granica
					if (pixel.r <30 && pixel.a != 0){ // Opcja 2, tylko granice bez przezroczystych
                        isObstacle = true;
                        break;
                    }

				}
				if (isObstacle) break;
			}
			tempGrid[y][x] = isObstacle;
		}
	}
	return tempGrid;
}
