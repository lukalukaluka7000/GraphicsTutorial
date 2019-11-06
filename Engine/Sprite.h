#pragma once

#include<include/GL/glew.h>

#include<string>
#include"GLTexture.h"
#include "ResourceManager.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, std::string texturePath);

	//render the sprite
	void draw();

private:
	//private to store that variables
	int _x;
	int _y;
	int _width;
	int _height;

	GLTexture _texture;

	//unsigned int _vboID; not guaranted to be 32 bits
	//open gl provides us with GLuint guaranted to be 32 bits
	GLuint _vboID;
};

