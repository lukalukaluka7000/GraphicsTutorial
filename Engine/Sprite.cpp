#include "Sprite.h"
#include "Vertex.h"
#include<cstddef>


Sprite::Sprite()
{
	//starts of by setting your vbo to null	
	_vboID = 0;
}


Sprite::~Sprite()
{
	//brisi ga ako postoji
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	//---CREATE VBO---
	//ONLY GENERATE BUFFER IF IT IS NOT GENERATED 
	//IF _VBOID == 0 WE KNOW DA NIJE GENERIRAN
	if (_vboID == 0) { //has not been generated yet
		//allocate it to a buffer

		//prvi paramatar: koliko spremnika treba stvorit, drugi:&identifikator spremnika
		glGenBuffers(1, &_vboID);//nece bit isti _vboID ovo sa & ce se updateat
		//SADA JER JE &_vboID ce se promijenit NICE
	}
	//quad is two triangles

	//make array of floats VBO jebeneno pogledat teku 8
	
	//float vertexData[12];
	Vertex vertexData[6];
	//FIRST TRIANGLE
	//vertexData[0].position.x = ;
	//vertexData[0].position.y = y + height;

	vertexData[0].setPosition(x + width, y + height);
	vertexData[0].setUV(1.0f, 1.0f);

	//vertexData[1].position.x = x;
	//vertexData[1].position.y = y + height;
	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0.0f, 1.0f);

	//vertexData[2].position.x = x;
	//vertexData[2].position.y = y;
	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0.0f, 0.0f);
	//SECOND TRIANGLE

	//vertexData[3].position.x = x;
	//vertexData[3].position.y = y;
	vertexData[3].setPosition(x,y);
	vertexData[3].setUV(0.0f,0.0f);

	//vertexData[4].position.x = x + width;
	//vertexData[4].position.y = y;
	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1.0f, 0.0f);


	//vertexData[5].position.x = x + width;
	//vertexData[5].position.y = y + height;
	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1.0f, 1.0f);


	

	for (int i = 0; i < 6; i++) {
		vertexData[i].color.r = 255;
		vertexData[i].color.g = 0;
		vertexData[i].color.b = 0;
		vertexData[i].color.a = 255;
	}

	vertexData[1].color.r = 0;
	vertexData[1].color.g = 0;
	vertexData[1].color.b = 255;
	vertexData[1].color.a = 255;

	vertexData[4].color.r = 0;
	vertexData[4].color.g = 255;
	vertexData[4].color.b = 0;
	vertexData[4].color.a = 255;


	_texture = ResourceManager::getTexture(texturePath);

	//BIND THE BUFFER SO WE CAN UPLOAD
	//we want ths buffer to be active
	glBindBuffer(GL_ARRAY_BUFFER, _vboID); 

	//UPLOAD THE DATA
	// ,amount of data(bytes), pointer to beggining of data, GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	
	//unbind the buffer with 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//render the sprite
void Sprite::draw() {

	glBindTexture(GL_TEXTURE_2D, _texture.id);

	//this is current buffer that is active
	//only one can be active at a time
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	
	//sending just one info(POS) - zero first element POS
	glEnableVertexAttribArray(0); 
	
	//position attribute pointer
	//tell opengl where are vertx data is in VBO - POINTER
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)offsetof(Vertex, position));
	//pointing opengl to start of data mozdazeimo crtat krace, a ne cijeli VBO
	
	//this is color attribute pointer
	//glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//this is uv attribute pointer
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// NE GL_QUADS
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//DrawArrays takes data from ARRAY_BUFFER.
	//Access violations during glDrawArrays or glDrawElements are most
	//often due to incorrectly enabled vertex attribute arrays
	//solution thread optimization disabled in nvidia settings

	//
	glDisableVertexAttribArray(0);

	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}