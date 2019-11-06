#include "TextureCache.h"



TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(std::string texturePath) {

	//std::map<std::string, GLTexture> _textureMap;

	if (_textureMap.find(texturePath) == _textureMap.end()) {
		// not found, uzmi novu teksturu
		GLTexture texture;
		texture = ImageLoader::loadPNG(texturePath);
		
		std::cout << "Cached texture loaded, prvi put je vidim" << std::endl;
		_textureMap.insert(make_pair(texturePath, texture));

		return texture;
	}
	else {
		// found

		std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath); //vrati iterator na eleemnt ili map::end
		std::cout << "Texture loaded from map" << std::endl;
		return mit->second;
	}

}