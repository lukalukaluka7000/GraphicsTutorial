#include<fstream>
#include<vector>
#include "GLSLProgram.h"
#include"EngineErrors.h"
#include<iostream>
<<<<<<< HEAD
#include"IOManager.h"
=======
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5


namespace Engine {
	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
		//_programID=0
		//instead initialization lists -> gore
		//_vertexShaderID=0
		//_fragmentShaderID=0
	}


	GLSLProgram::~GLSLProgram()
	{
	}


	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
<<<<<<< HEAD

		std::string vertSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);


		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());

	}
	void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource) {
=======
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		_programID = glCreateProgram();

		//open the files and read the data -> compile the shaders
		//opengl shader compilation
		//GLVERTEXX_SHADER, GL_FRAGMENT_SHADER
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

		if (_vertexShaderID == 0) {
			fatalError("Vertex Shader failed to be created!");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		if (_fragmentShaderID == 0) {
			fatalError("Fragment Shader failed to be created!");
		}

<<<<<<< HEAD
		compileShader(vertexSource, "Vertex File", _vertexShaderID);
		compileShader(fragmentSource, "Fragment File", _fragmentShaderID);
	}
	void GLSLProgram::compileShader(const char* source, const std::string& name, GLuint id) {


		/*//load the code from files
		std::ifstream shaderFile(filePath);
		if (shaderFile.fail()) {
=======
		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);

	}
	void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {


		//load the code from files
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail()) {
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5
			perror(filePath.c_str());
			fatalError("Failed to open" + filePath);
		}
		std::string fileContents = "";
		std::string line;

<<<<<<< HEAD
		while (std::getline(shaderFile, line)) {
			fileContents += line + "\n";
		}
		shaderFile.close();

		const char* contentsPtr = fileContents.c_str(); //array of strings*/



		//glShaderSource(id, 1, &contentsPtt, nullptr); //0
		glShaderSource(id, 1, &source, nullptr);
=======
		while (std::getline(vertexFile, line)) {
			fileContents += line + "\n";
		}
		vertexFile.close();

		const char* contentsPtr = fileContents.c_str(); //array of strings
		glShaderSource(id, 1, &contentsPtr, nullptr); //0
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5

		glCompileShader(id);


		// // // // sve s wiki
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			std::printf("%s\n", &errorLog[0]);
<<<<<<< HEAD
			fatalError("Shader " + name + " failed to compile");
=======
			fatalError("Shader " + filePath + " failed to compile");
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5

		}
		std::cout << "JA SAM aaaaaa" << std::endl;
		// // // // sve s wiki


	}
<<<<<<< HEAD


=======
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5
	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
		//odnosti se na onaj prvi dio tih fieova in vec2
	}
	void GLSLProgram::linkShaders() {

		std::cout << "4.0" << std::endl;

		// Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		// Link our program
		glLinkProgram(_programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
		std::cout << "4.1" << std::endl;
		std::cout << isLinked << std::endl;
		// // // // sve s wiki
		if (isLinked == GL_FALSE)
		{
			std::cout << "4.****" << std::endl;
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::cout << "mamamamama" << std::endl;
			std::cout << "Max length: " << maxLength << std::endl;

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			std::cout << "Max length: " << maxLength << std::endl;
			std::cout << "mamamamama" << std::endl;

			//std::cout << errorLog[0] << std::endl;
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			std::cout << "4.2" << std::endl;

			// We don't need the program anymore.
			glDeleteProgram(_programID);
			// Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			//std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader failed to link");

			return;
		}
		std::cout << "4.3" << std::endl;
		// // // // sve s wiki

		// Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
	}




	void GLSLProgram::use() {
		glUseProgram(_programID);

		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}

	}
	void GLSLProgram::unuse() {
		glUseProgram(0);

		glUseProgram(_programID);
		for (int i = 0; i <= _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	GLint GLSLProgram::getUniformLocation(const std::string uniformName) {
		//dijeljena memorija u GPU koja je svima vidljiva
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + "not found in shader");
		}
		return location;

	}
<<<<<<< HEAD
	void GLSLProgram::dispose() {
		if(_programID) glDeleteProgram(_programID);

	}
=======
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5

}