#include "Camera2D.h"
#include<iostream>

namespace Engine {
	Camera2D::Camera2D() : _position(0, 0),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_scale(1.0f),
		_needsChange(true), //initialization list
		_screenWidth(0),
		_screenHeight(0)
	{
	}
	Camera2D::~Camera2D()
	{

	}
	void Camera2D::init(int W, int H) {
		_orthoMatrix = glm::ortho(0.0f, float(W), 0.0f, float(H));
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
			
			//_cameraMatrix = glm::scale(_cameraMatrix, scaleVec);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * _cameraMatrix;
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

	//aabb test to see if a box is in the camera 
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) {

		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale); //*2

		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y /2.0f + scaledScreenDimensions.y / 2.0f;

		//center of agents
		glm::vec2 centerPosition = position + dimensions / 2.0f;
		glm::vec2 centerCameraPosition = _position;
		glm::vec2 distVec = centerPosition - centerCameraPosition;

		//check x direction
		//depth je koliko je zarilo u drugi tile, sta je pozitivniji i veci to je vise unutra
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);
		if (xDepth > 0 && yDepth > 0) {
			return true;
		}
		return false;
	}
}