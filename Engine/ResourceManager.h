#pragma once
#include"GLTexture.h"
#include"TextureCache.h"
#include<string>

class ResourceManager
{
public:
	static GLTexture getTexture(std::string texturePath);
private:
	static TextureCache _textureCache;
};

