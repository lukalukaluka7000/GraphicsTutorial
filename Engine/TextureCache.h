#pragma once
#include<string>
#include<map>
#include<iostream>
#include"GLTexture.h"
#include "ImageLoader.h"


namespace Engine {
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();
		GLTexture getTexture(std::string texturePath);
	private:
		std::map<std::string, GLTexture> _textureMap;
	};

}