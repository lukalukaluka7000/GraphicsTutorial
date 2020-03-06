#include "Camera2D.h"


namespace Engine {
	Camera2D::Camera2D() : _position(0, 0),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_scale(1.0f),
		_needsChange(true), //initialization list
		_screenWidth(500),
		_screenHeight(500)
	{
	}
	Camera2D::~Camera2D()
	{

	}
	void Camera2D::init(int W, int H) {
		_orthoMatrix = glm::ortho(0.0f, _screenWidth, 0.0f, _screenHeight);
		_screenWidth = W;
		_screenHeight = H;
	}
	void Camera2D::update() {
		if (_needsChange == true) { //treba mijenjat cameraMatrix (ortho uvik imamo i ovisi samo o W i H)
			//ovi + w/2 i +h/2 su isto sto i u prvim primjerima cupiceve knjige da focal point bude u centru ekrana ili di vec zelimo
			//moze se lako najestit da bude u centru svakog spritea TODO
			glm::vec3 translationVec(-_position.x + _screenWidth / 2, -_position.y+ _screenHeight / 2, 0.0f);
			glm::vec3 scaleVec(_scale, _scale, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translationVec);
			_cameraMatrix = glm::scale(_cameraMatrix, scaleVec);
			//_cameraMatrix = glm::translate(_cameraMatrix, glm::vec3(_screenWidth / 2, _screenHeight / 2, 0.0f));
			_needsChange = false;
		}
	}
	glm::vec2 Camera2D::ScreenToScene(glm::vec2 mousecord) {
		//invertiranje y-axis
		mousecord.y = _screenHeight - mousecord.y;
		glm::vec2 sceneCoords;
		sceneCoords = mousecord - glm::vec2(_screenWidth / 2, _screenHeight / 2);
		sceneCoords /= _scale;
		sceneCoords += _position;
		return sceneCoords;
	}
}