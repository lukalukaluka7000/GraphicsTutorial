#pragma once

#include"Human.h"

#include<Engine/InputManager.h>
#include<Engine/Camera2D.h>

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Engine::InputManager* inputManager, Engine::Camera2D* camera);

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies, 
						std::vector<Gun*>& guns,
						float deltaTime) override;

	glm::vec2 getPlayerPosition();

	int currentGunIndex;
private:

	Engine::InputManager* _inputManager;
	Engine::Camera2D* _camera;
	
};

