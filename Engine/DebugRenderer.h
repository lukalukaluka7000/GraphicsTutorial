#pragma once
#include<glm/glm.hpp>
#include"Vertex.h"
#include"SpriteBatch.h"
#include"GLSLProgram.h"
namespace Engine {
	class DebugRenderer
	{
	public:
		DebugRenderer();
		~DebugRenderer();

		void init();

		void end();

		void drawLine(glm::vec4& destRect, Engine::Vertex::ColorRGBA8 color);
		void drawBox(glm::vec4& destRect, Engine::Vertex::ColorRGBA8 color, float angle);
		void drawCircle(const glm::vec2& center, Engine::Vertex::ColorRGBA8& color, float radius);

		void render(const glm::mat4& projectionMatrix, float lineWidth);

		void dispose();

		struct DebugVertex {
			glm::vec2 position;
			Engine::Vertex::ColorRGBA8 color;
		};

	private:
		GLSLProgram _program;
		std::vector<DebugVertex> _vertices;
		std::vector<GLuint> _indices;
		GLuint _vbo = 0, _vao = 0, _ibo = 0;
		int _numElements = 0;
	};
}

