#pragma once
#include<iostream>
#include<vector>

namespace Engine {
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer); //vector of chars
<<<<<<< HEAD
		static bool readFileToBuffer(std::string filePath, std::string& buffer); //vector of chars

=======
>>>>>>> b601b96a92a10ac7168d7c02b7ff61d86f7179f5
	};
}

