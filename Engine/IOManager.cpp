#include "IOManager.h"
#include<fstream>

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
	std::ifstream file(filePath, std::ios::binary); //binary format, raw bytes

	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	//how big a file is (buffer in parameters)
	//seek to end of file, determine how many bytes long
	//seek to end
	file.seekg(0, std::ios::end);

	//get the file size
	int fileSize = file.tellg(); // return how many bytes before current location
	file.seekg(0, std::ios::beg);

	//reduce the file size by any heade bytes that might be present
	fileSize -= file.tellg();

	//read in file in buffer

	//first resize to the actual file size
	buffer.resize(fileSize);
	//file >> nesto nesto, ali bolje jejk
	file.read((char*)&(buffer[0]), fileSize);
	file.close();
	return true;

}