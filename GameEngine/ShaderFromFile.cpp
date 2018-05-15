#include "ShaderFromFile.h"
#include <fstream>


ShaderFromFile::ShaderFromFile()
{

}


ShaderFromFile::~ShaderFromFile()
{
}

void ShaderFromFile::Add(const std::string& path, GLenum shaderType)
{
	std::ifstream file;
	file.open(path.c_str());

	std::string output;
	std::string line;
	if (file.is_open()) {
		while (file.good()) {
			std::getline(file, line);
			output.append(line + "\n");
		}

		AttachShader(output, shaderType);
		std::cout << "File: " << path << " loaded" << std::endl;
	}
	else {
		std::cout << "File: " << path << " error" << std::endl;
	}
}
