#include "DebugRenderer.h"
#include<iostream>
#define PI 3.14159265359f

const char* VERT_SRC = R"(#version 130
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;
    
    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;
    
    fragmentPosition = vertexPosition;
    
    fragmentColor = vertexColor;
})";
const char* FRAG_SRC = R"(#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})";
namespace Engine {
	DebugRenderer::DebugRenderer() {

	}
	DebugRenderer::~DebugRenderer() {

	}

	void DebugRenderer::init() {
		//SHADER INIT
		_program.compileShadersFromSource(VERT_SRC, FRAG_SRC);
		_program.addAttribute("vertexPosition");
		_program.addAttribute("vertexColor");
		_program.linkShaders();

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));


		//ovo automatski ocisti buffere pa ne triba unbindat buffere
		glBindVertexArray(0);
	}

	void DebugRenderer::end() {

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphan the buffer with this nullptr so he deletes buffer and not overwrites it and gives us brand new buffer
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(DebugVertex), _vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		//orphan the buffer with this nullptr so he deletes buffer and not overwrites it and gives us brand new buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _indices.size() * sizeof(GLuint), _indices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		_numElements = _indices.size();
		
		_indices.clear();
		_vertices.clear();
	}
	glm::vec2 rotatePoint(const glm::vec2& pos, float angle) {
		glm::vec2 newv;
		newv.x = pos.x * cos(angle) - pos.y * sin(angle);
		newv.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newv;
	}
	void DebugRenderer::drawLine(glm::vec4& destRect, Engine::Vertex::ColorRGBA8 color) {
		_vertices.resize(_vertices.size() + 2);
		int indexVert = _vertices.size() - 2;
		_vertices[indexVert + 0].position = glm::vec2(destRect.x, destRect.y);
		_vertices[indexVert + 1].position = glm::vec2(destRect.z, destRect.w);

		_vertices[indexVert + 0].color = color;
		_vertices[indexVert + 1].color = color;

		_indices.resize(_indices.size() + 2);
		int indexIndices = _indices.size() - 2;

		_indices[indexIndices + 0] = indexVert + 0;
		_indices[indexIndices + 1] = indexVert + 1;
	}
	void DebugRenderer::drawBox(glm::vec4& destRect, Engine::Vertex::ColorRGBA8 color, float angle) {
		/*GLSLProgram _program;
		std::vector<DebugVertex> _vertices;
		std::vector<GLuint> _indices;*/
		//destRect.x .y .dimx .dimy, dobijem color i dobijem angle
		_vertices.resize(_vertices.size() + 4);
		int indexVert = _vertices.size() - 4;

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);
		// Get points centered at origin
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);

		glm::vec2 positionOffset(destRect.x, destRect.y);


		_vertices[indexVert + 0].position = rotatePoint(tl, angle) + halfDims + positionOffset;
		_vertices[indexVert + 1].position = rotatePoint(bl, angle) + halfDims + positionOffset;
		_vertices[indexVert + 2].position = rotatePoint(br, angle) + halfDims + positionOffset;
		_vertices[indexVert + 3].position = rotatePoint(tr, angle) + halfDims + positionOffset;
		/*_vertices[0].position.x = destRect.x;
		_vertices[0].position.y = destRect.y;

		_vertices[1].position.x = destRect.x;
		_vertices[1].position.x = destRect.x;*/

		_vertices[indexVert + 0].color = color;
		_vertices[indexVert + 1].color = color;
		_vertices[indexVert + 2].color = color;
		_vertices[indexVert + 3].color = color;

		_indices.resize(_indices.size() + 8);
		int indexIndices = _indices.size() - 8;

		_indices[indexIndices + 0] = indexVert + 0;
		_indices[indexIndices + 1] = indexVert + 1;

		_indices[indexIndices + 2] = indexVert + 1;
		_indices[indexIndices + 3] = indexVert + 2;

		_indices[indexIndices + 4] = indexVert + 2;
		_indices[indexIndices + 5] = indexVert + 3;

		_indices[indexIndices + 6] = indexVert + 3;
		_indices[indexIndices + 7] = indexVert + 0;
		
	}
	void DebugRenderer::drawCircle(const glm::vec2& center, Engine::Vertex::ColorRGBA8& color, float radius) {
		const int NUM_VERTICES = 100;
		int startniVrh = _vertices.size(); // 204

		_vertices.resize(_vertices.size() + NUM_VERTICES);
		int indexVert = _vertices.size() - NUM_VERTICES;
		int br = 0;
		// ovo je indexed, a ja sad u svom projektu pokusavan obicni i nemogu hahahaha
		for (size_t i = indexVert; i < _vertices.size(); i++, br++) {
			_vertices[i].position.x = radius * cos((float)br / /*(NUM_VERTICES))**/(2.0f * PI)) + center.x;
			_vertices[i].position.y = radius * sin((float)br / /*(NUM_VERTICES))**/(2.0f * PI)) + center.y;
			_vertices[i].color = color;
		}

		
		_indices.resize(_indices.size() + 2 * NUM_VERTICES);
		int startniIndex = _indices.size() - 2 * NUM_VERTICES;


		br = 1;
		int i;
		_indices[startniIndex] = startniVrh; //204 //pogledaj u teku [408] = 204
		
		for (i = startniIndex + 1; i < _indices.size() - 2; i = i + 2, br++) {
			_indices[i]		= startniVrh + br; // [409] = 205
			_indices[i + 1] = startniVrh + br; // [410] = 205
											   // [411] = 206
											   // [412] = 206 ...
											   //do predzadnjega
		}
		_indices[i] = startniVrh; //; // [607] = 304=size(vertices)
		//_indices[i + 1] =  startniVrh;   // [608] = 204=startniVrh
	}

	void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth) {
		_program.use();

		GLint pUniform = _program.getUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		glLineWidth(lineWidth);
		//glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		glBindVertexArray(_vao);
		glDrawElements(GL_LINES, _numElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindVertexArray(0);

		_program.unuse();
	}
	void DebugRenderer::dispose()
	{
		if (_vao) {
			glDeleteVertexArrays(1, &_vao);
		}
		if (_vbo) {
			glDeleteBuffers(1, &_vbo);
		}
		if (_ibo) {
			glDeleteBuffers(1, &_ibo);
		}
		_program.dispose();
	}
}