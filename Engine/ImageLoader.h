#pragma once
#include<include/GL/glew.h>
#include<string>
#include"GLTexture.h"
static class ImageLoader
{
public:
	static GLTexture loadPNG(std::string filePath);
};

