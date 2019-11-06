#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
class Camera2D
{
public:
	Camera2D();
	~Camera2D();
private:
	glm::vec2 position;
	glm::mat4 orthoMatrix;
};

