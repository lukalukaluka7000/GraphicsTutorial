#pragma once
#include<vector>
#include<GL/glew.h>
#include<glm/glm.hpp>
#include"Vertex.h" // ili <Engine/Vertex.h>


namespace Engine {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph {
	public:
		Glyph() { };
		Glyph(GLuint Texture, float Depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::ColorRGBA8> colors);
		Glyph(GLuint Texture, float Depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::ColorRGBA8> colors, float angle);

		GLuint texture = 0;
		float depth = 0.0f;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex bottomRight;
		Vertex topRight;
	private:
		glm::vec2 rotatePoint(glm::vec2, float angle);
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture)  : offset(Offset), numVertices(NumVertices), texture(Texture)
		{}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();

		void draw(GLuint texture, float depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::ColorRGBA8> colors);
		void draw(GLuint texture, float depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::ColorRGBA8> colors,float angle);
		void draw(GLuint texture, float depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::ColorRGBA8> colors, const glm::vec2& dir);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray(); //all the crap put in here jer sva ona stanja zauzimaju neko vrijeme izvodenja a s ovim to sve ukomponiramo
		void sortGlyphs();
		GLuint _vbo;
		GLuint _vao;

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);
		GlyphSortType _sortType = GlyphSortType::NONE;


		std::vector<Glyph*> _glyphPointers; // this is for sorting
		std::vector<Glyph> _glyphs;			//actual glyphs

		std::vector<RenderBatch> _renderBatches;
	};

}