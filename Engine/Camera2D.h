#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


namespace Engine {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int W, int H);
		void setPosition(const glm::vec2& newPosition) { _position = newPosition; _needsChange = true; }
		glm::vec2 getPosition() { return _position;  }

		void setScale(float newScale) { _scale = newScale; _needsChange = true;}
		float getScale() { return _scale; }

		void update();
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }
		glm::vec2 ScreenToScene(glm::vec2 mousecord);

		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

	private:
		float _screenWidth, _screenHeight;
		float _scale;
		glm::vec2 _position;

		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
		bool _needsChange; // we need to change cameraMatrix IFF position or scale are changed

	};
}
