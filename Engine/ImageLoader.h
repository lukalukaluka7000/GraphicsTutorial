#pragma once
#include<GL/glew.h>
#include<string>
#include"GLTexture.h"

namespace Engine {
	static class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}
