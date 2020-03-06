#include "ResourceManager.h"
#include"TextureCache.h"

namespace Engine {
	//decladre it and say that it exist
	TextureCache ResourceManager::_textureCache;

	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return _textureCache.getTexture(texturePath);
	}
	//GLTexture ResourceManager::getTexture(std::string texturePath) {
	//	return TextureCache::getTexture(texturePath);
	//}
}