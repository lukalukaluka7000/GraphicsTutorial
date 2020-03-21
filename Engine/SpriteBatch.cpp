#define M_PI           3.14159265358979323846f

#include "SpriteBatch.h"
#include<Engine/Vertex.h>
#include<algorithm>
#include<iostream>


#include<ZombieGame/Agent.h>
namespace Engine {
	Glyph::Glyph(GLuint Texture, float Depth, glm::vec4 destRect, glm::vec4 uvRect, std::vector<Engine::Vertex::ColorRGBA8> colors) {
		texture = Texture;
		depth = Depth;

		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);
		//newGlyph->bottomLeft.setColor(color.r, color.g, color.b, color.a);
		bottomLeft.color = colors[2];

		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
		topLeft.color = colors[1];

		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		topRight.color = colors[0];

		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		bottomRight.color = colors[3];
	}

	Glyph::Glyph(	GLuint Texture, 
					float Depth, 
					glm::vec4 destRect, 
					glm::vec4 uvRect, 
					std::vector<Engine::Vertex::ColorRGBA8> colors, 
					float angle) {
		texture = Texture;
		depth = Depth;

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		// Get points centered at origin
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);
		

		// Rotate the points
		tl = rotatePoint(tl, angle) + halfDims;
		bl = rotatePoint(bl, angle) + halfDims;
		br = rotatePoint(br, angle) + halfDims;
		tr = rotatePoint(tr, angle) + halfDims;
		if (colors.back().b == 6) {
			std::cout << destRect.x + bl.x << " " << destRect.y + bl.y << std::endl;
			std::cout << destRect.x + tl.x << " " << destRect.y + destRect.w + tl.y << std::endl;
			std::cout << destRect.x + destRect.z + tr.x << " " << destRect.y + destRect.w + tr.y << std::endl;
			std::cout << destRect.x + destRect.z + br.x << " " << destRect.y + br.y << std::endl;
			std::cout << std::endl;
		}

		bottomLeft.setPosition(destRect.x+bl.x, destRect.y+bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);
		//newGlyph->bottomLeft.setColor(color.r, color.g, color.b, color.a);
		bottomLeft.color = colors[2];

		topLeft.setPosition(destRect.x+tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
		topLeft.color = colors[1];

		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		topRight.color = colors[0];

		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		bottomRight.color = colors[3];
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle) {
		glm::vec2 newVec;
		newVec.x = pos.x * cos(angle) - pos.y * sin(angle );
		newVec.y = pos.x * sin(angle) + pos.y * cos(angle );
		/*newVec.x = pos.x * cos(angle) + pos.y * sin(angle);
		newVec.y = -pos.x * sin(angle) + pos.y * cos(angle);*/

		return newVec;
	}

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) { }

	SpriteBatch::~SpriteBatch() { }

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType) {
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}
	void SpriteBatch::end() {
		_glyphPointers.resize(_glyphs.size());
		//point allthe poitners to glyphs
		for (int i = 0; i < _glyphs.size(); i++) {
			_glyphPointers[i] = &_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(	GLuint texture, 
							float depth, 
							glm::vec4 destRect, 
							glm::vec4 uvRect, 
							std::vector<Engine::Vertex::ColorRGBA8> colors) {

		_glyphs.emplace_back(texture, depth, destRect, uvRect, colors);
	}
	void SpriteBatch::draw(	GLuint texture, 
							float depth, 
							glm::vec4 destRect, 
							glm::vec4 uvRect, 
							std::vector<Engine::Vertex::ColorRGBA8> colors, 
							float angle) {
		_glyphs.emplace_back(texture, depth, destRect, uvRect, colors, angle);
	}
	void SpriteBatch::draw(	GLuint texture, 
							float depth, 
							glm::vec4 destRect, 
							glm::vec4 uvRect, 
							std::vector<Engine::Vertex::ColorRGBA8> colors, 
							const glm::vec2& dir) {

		const glm::vec2 right(1.0f, 0.0f);

		float angle = acos(glm::dot(right, dir));

		//angle = angle * (180.0f / M_PI);
		if (dir.y < 0.0f) angle = -angle;

		//std::cout << angle << std::endl;

		_glyphs.emplace_back(texture, depth, destRect, uvRect, colors, angle);
	}
	void SpriteBatch::renderBatch() {
		glBindVertexArray(_vao);
		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	//preko pointera
	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(6 * _glyphPointers.size());

		if (_glyphPointers.empty()) {
			return;
		}

		
		//RenderBatch myBatch(0, 6, _glyphs[0]->texture);
		_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);
		int offset = 0;
		int cv = 0; // current vertex
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;

		offset += 6;
		for (int cg = 1; cg < _glyphs.size(); cg++) {
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			else {
				_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			/*vertices[cv++] = _glyphs[cg]->topRight;
			vertices[cv++] = _glyphs[cg]->topLeft;
			vertices[cv++] = _glyphs[cg]->bottomLeft;
			vertices[cv++] = _glyphs[cg]->bottomLeft;
			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->topRight;*/
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphan the buffer with this nullptr so he deletes buffer and not overwrites it and gives us brand new buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0 , vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void SpriteBatch::createVertexArray() {
		if (_vao == 0)
			glGenVertexArrays(1, &_vao);
		
		glBindVertexArray(_vao);

		if (_vbo == 0)
			glGenBuffers(1, &_vbo);

		//this is current buffer that is active
		//only one can be active at a time
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//sending just one info(POS) - zero first element POS
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//position attribute pointer
		//tell opengl where are vertx data is in VBO - POINTER
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//pointing opengl to start of data mozdazeimo crtat krace, a ne cijeli VBO

		//this is color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		//this is uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}
	void SpriteBatch::sortGlyphs() {
		switch (_sortType) {
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
			break;
		 }
	}
	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}
}