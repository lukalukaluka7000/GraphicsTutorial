#pragma once
#include"GLTexture.h"
#include<glm/glm.hpp>
//class for spritesheet
//grid of sprites of the same size
namespace Engine {
	class TileSheet {
	public:
		void init(const Engine::GLTexture& texture, const glm::ivec2& tileDims/*x and y of tilesheet in tiles */) {
			_texture = texture;
			_dimsOfSpriteSheet = tileDims;
			_widthOfOneTile = 1.0f / _dimsOfSpriteSheet.x; // 
			_heightOfOneTile = 1.0f / _dimsOfSpriteSheet.y;
		}

		glm::vec4 getUV(int indexOfTile) {
			int xTile = indexOfTile % _dimsOfSpriteSheet.x; // 0
			int yTile = indexOfTile / _dimsOfSpriteSheet.x; // 1

			// 10 x 2
			glm::vec4 uv;
			uv.x = xTile / (float)_dimsOfSpriteSheet.x;
			uv.y = yTile / (float)_dimsOfSpriteSheet.y;
			uv.z = _widthOfOneTile;
			uv.w = _heightOfOneTile;

			return uv;
		}

		Engine::GLTexture _texture;
		glm::ivec2 _dimsOfSpriteSheet;

		float _widthOfOneTile = 0.0f;
		float _heightOfOneTile = 0.0f;

	};
}