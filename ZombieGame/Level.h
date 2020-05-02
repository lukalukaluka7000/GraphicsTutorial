#pragma once
#include<string>
#include<vector>
#include<Engine/SpriteBatch.h>
#include<glm/glm.hpp>

const float SPRITE_SIZE = 64.0f;

class Level
{
public:
	Level(std::string levelFileName);
	~Level();

	void init();
	void draw();

	glm::vec2 getStartPlayerPosition() const { return _startPlayerPosition; }
	//const std::vector<glm::vec2>& getStartZombiePosition() { return _zombiePosition; }
	std::vector<glm::vec2> getStartZombiePositions() const { return _startZombiePositions; }
	

	const std::vector<std::string>& getLevelData();

	int getNumberHumans() { return _numHumans; }
	int getNumberZombies() { return _numZombies; }

	int getWidth() { return _levelMap[0].size(); }
	int getHeight() { return _levelMap.size(); }
private:
	std::string _levelFileName;
	int _numHumans;
	int _numZombies;

	std::vector<std::string> _levelMap;

	Engine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPosition;
	std::vector<glm::vec2> _startZombiePositions;
};

