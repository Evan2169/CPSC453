#include "ObjectFileReader.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

ObjectFileReader::ObjectFileReader(const char* filePath)
{
	std::ifstream fileInputStream = streamForFilePath(filePath);
	std::string lineOfFile;
	while (std::getline(fileInputStream, lineOfFile)) 
	{
		std::stringstream lineStream(lineOfFile);
		std::string lineTypeIdentifier;
		lineStream >> lineTypeIdentifier;
		
		if(lineTypeIdentifier == "v") 
		{
			GLfloat x, y, z;
			lineStream >> x >> y >> z;

		   vertices_.push_back(x);
		   vertices_.push_back(y);
		   vertices_.push_back(z);
		}
		else if (lineTypeIdentifier == "vn")
		{
			GLfloat x, y, z;
			lineStream >> x >> y >> z;

		   normals_.push_back(x);
		   normals_.push_back(y);
		   normals_.push_back(z);	
		}
		else if(lineTypeIdentifier == "vt")
		{
			GLfloat x, y;
			lineStream >> x >> y;

		   textures_.push_back(x);
		   textures_.push_back(y);
		}
		else if (lineTypeIdentifier == "f")
		{
			std::string vertexData;
			while(lineStream >> vertexData)
			{
				bool shouldContinueParsingVertexDataString = true;
				for(int i = 0; i < 3 && shouldContinueParsingVertexDataString; i++)
				{
					switch(i)
					{
						case(0):
						{
							auto positionOfFirstSlash = vertexData.find("/", 0);
							shouldContinueParsingVertexDataString = positionOfFirstSlash != std::string::npos;
							std::string vertexIndex = vertexData.substr(0, positionOfFirstSlash);
							vertexData = vertexData.substr(positionOfFirstSlash+1);
			   			vertexIndices_.push_back(std::stoi(vertexIndex) - 1);
			   			break;
						}
			   		case(1):
			   		{
							auto positionOfSecondSlash = vertexData.find("/", 0);
							shouldContinueParsingVertexDataString = positionOfSecondSlash != std::string::npos;
							std::string textureIndex = vertexData.substr(0, positionOfSecondSlash);
							vertexData = vertexData.substr(positionOfSecondSlash+1);
			   			textureIndices_.push_back(std::stoi(textureIndex) - 1);
			   			break;
			   		}
			   		case(2):
			   		{
			   			normalIndices_.push_back(std::stoi(vertexData) - 1);
			   		}
					}
				}
			}
		}
	}
}

std::vector<GLfloat> ObjectFileReader::vertices() const
{
	return vertices_;
}

std::vector<GLfloat> ObjectFileReader::textures() const
{
	return textures_;
}

std::vector<GLfloat> ObjectFileReader::normals() const
{
	return normals_;
}

std::vector<GLuint> ObjectFileReader::vertexIndices() const
{
	return vertexIndices_;
}

std::vector<GLuint> ObjectFileReader::textureIndices() const
{
	return textureIndices_;
}

std::vector<GLuint> ObjectFileReader::normalIndices() const
{
	return normalIndices_;	
}

std::ifstream ObjectFileReader::streamForFilePath(const char* filePath)
{
	std::ifstream fileInputStream(filePath);
  	if (!fileInputStream) 
  	{
  	  std::cerr << "Could not open object file.\n" << std::endl;
  	  exit(-1);
  	}
  	return fileInputStream;
}
