#pragma once
#include<string>
#include<include/GL/glew.h>
class GLSLProgram //opengl shading language
{ //language to write shaders
public:
	GLSLProgram();
	~GLSLProgram();

	//functions that compiles hsaders
	//read frag and vert from file and than compile that opengl can use
	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath); 
	//&good habit
	void linkShaders();

	//addd attribute to next avalilable slot
	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string uniformName);
	void use();
	void unuse();

private:
	int _numAttributes;

	//zasto je privatna ???
	void compileShader(const std::string&filePath, GLuint id);

	//hold on to individua shaders

	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
};
//link tohgether frag and vert in single program on gpu

