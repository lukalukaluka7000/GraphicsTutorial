#pragma once //both files include GLTexutre, pragma fixa to
#include<GL/glew.h>

namespace Engine {
	class GLTexture
	{
	public:
		struct {
			GLuint id;
			int width;
			int height;
		};

	};
}
