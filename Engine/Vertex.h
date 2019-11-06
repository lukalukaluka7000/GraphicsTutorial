#pragma once
#include<include/GL/glew.h>
struct UV {
	float u;
	float v;
};
struct Vertex {
	//float position[2];
	struct Position {
		float x;
		float y;
	} position;


	//GLubyte color[4];
	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	} color;

	//UV Texture coorinates
	UV uv; //[0,1]
	
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}
	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}
};