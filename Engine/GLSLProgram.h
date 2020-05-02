#pragma once
#include<string>
#include<GL/glew.h>
namespace Engine {
	class GLSLProgram //opengl shading language
	{ //language to write shaders
	public:
		GLSLProgram();
		~GLSLProgram();

		//functions that compiles hsaders
		//read frag and vert from file and than compile that opengl can use
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
<<<<<<< HEAD

		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);
=======
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5
		//&good habit
		void linkShaders();

		//addd attribute to next avalilable slot
		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string uniformName);
		void use();
		void unuse();

<<<<<<< HEAD
		void dispose();
	private:
		int _numAttributes;


		//zasto je privatna ???
		//void compileShader(const std::string& filePath, GLuint id);
		void compileShader(const char* source, const std::string& name, GLuint id);
=======
	private:
		int _numAttributes;

		//zasto je privatna ???
		void compileShader(const std::string& filePath, GLuint id);
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5

		//hold on to individua shaders

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
	//link tohgether frag and vert in single program on gpu
}
