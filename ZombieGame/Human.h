#pragma once

#include"Agent.h"
#include<glm/glm.hpp>

class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans, 
						std::vector<Zombie*>& zombies, 
						std::vector<Gun*>& guns,
						float deltaTime) override;

	
private:
	//glm::vec2 _direction;
	int _frames;
	int _rotateEveryFrame;
};

