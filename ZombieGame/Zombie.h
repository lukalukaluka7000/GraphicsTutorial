#pragma once

#include"Agent.h"
#include<iostream>

class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans, 
						std::vector<Zombie*>& zombies, 
						std::vector<Gun*>& guns,
						float deltaTime) override;
private:
	int getNearestHumanIndex(std::vector<Human*>& humans);

	//glm::vec2 _direction;
	int _frames;
	int _rotateEveryFrame;
};

