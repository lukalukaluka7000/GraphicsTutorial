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

	struct Glyph {
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex bottomRight;
		Vertex topRight;
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

		void draw(GLuint texture, float depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::Color> colors);

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
		GlyphSortType _sortType;
		std::vector<Glyph*> _glyphs;
		std::vector<RenderBatch> _renderBatches;
	};

}