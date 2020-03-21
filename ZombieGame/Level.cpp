#include "Level.h"

#include<fstream>
#include<iostream>

#include<Engine/GLTexture.h>
#include<Engine/ResourceManager.h>

#include<glm/glm.hpp>

Level::Level(std::string levelFileName) {
	_levelFileName = levelFileName;
	_numHumans = 0;
}
Level::~Level() {

}
void Level::init() {
	std::ifstream file(_levelFileName);
	
	std::string tmp;
	std::string junk;

	file >> junk >> _numHumans;
	// u tmp mi je sad cila prva linija
	std::getline(file, tmp);

	while (std::getline(file, tmp)) {
		// process string ...
		_levelMap.push_back(tmp);
	}
	_spriteBatch.init();
	_spriteBatch.begin(Engine::GlyphSortType::TEXTURE);

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	
	

	std::vector<Engine::Vertex::ColorRGBA8> colors;
	Engine::Vertex::ColorRGBA8 color;

	color.SetColor(0, 255, 0, 255);
	colors.push_back(color);

	color.SetColor(0, 0, 255, 255);
	colors.push_back(color);

	color.SetColor(255, 255, 255, 255);
	colors.push_back(color);

	color.SetColor(0, 255, 0, 255);
	colors.push_back(color);

	Engine::GLTexture texture;
	for (int i = 0; i < _levelMap.size(); i++) {
		//std::cout << _levelMap[i] << "\n";
		for (int j = 0; j < _levelMap[i].size(); j++) {
			glm::vec4 destRect(j * SPRITE_SIZE, i * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE);
			char currentSprite = _levelMap[i][j];

			switch (currentSprite) {
			case 'B':
			case 'R':
				//red_bricks.png	GLTexture getTexture(std::string texturePath);
				texture = Engine::ResourceManager::getTexture("./Textures/red_bricks.png");
				//draw(GLuint texture, float depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::ColorRGBA8> colors);
				// destRect(x, y, width, height);
				_spriteBatch.draw(texture.id, 0.0f, destRect, uvRect, colors);
				break;
			case 'G':
				//red_bricks.png	GLTexture getTexture(std::string texturePath);
				texture = Engine::ResourceManager::getTexture("./Textures/glass.png");
				_spriteBatch.draw(texture.id, 0.0f, destRect, uvRect, colors);
				break;
			case 'L':
				//red_bricks.png	GLTexture getTexture(std::string texturePath);
				texture = Engine::ResourceManager::getTexture("./Textures/light_bricks.png");
				_spriteBatch.draw(texture.id, 0.0f, destRect, uvRect, colors);
				break;
			case '@':
				_levelMap[i][j] = '.';
				std::cout << "sljam najobicniji" << std::endl;
				std::cout << currentSprite << std::endl;
				std::cout << i << " " << j << std::endl;
				_startPlayerPosition.x = j * SPRITE_SIZE;
				_startPlayerPosition.y = i * SPRITE_SIZE;
				//texture = Engine::ResourceManager::getTexture("./Textures/player.png");
				//_spriteBatch.draw(texture.id, 0.0f, destRect, uvRect, colors);
				break;
			case 'Z':
				_levelMap[i][j] = '.';
				glm::vec2 startZombiePosition;
				startZombiePosition.x = j * SPRITE_SIZE;
				startZombiePosition.y = i * SPRITE_SIZE;
				_startZombiePositions.push_back(startZombiePosition);
				_numZombies++;
				//texture = Engine::ResourceManager::getTexture("./Textures/zombie.png");
				//_spriteBatch.draw(texture.id, 0.0f, destRect, uvRect, colors);
				break;
			case '.':
				break;
			default:
				std::cout << "A nije ti to nista" << std::endl;
				break;
			}

		}

	}

	_spriteBatch.end();

}
void Level::draw() {
	_spriteBatch.renderBatch();
}
const std::vector<std::string>& Level::getLevelData() {
	return _levelMap;
}