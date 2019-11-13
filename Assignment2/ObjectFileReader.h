#pragma once

#include <fstream>
#include "glad/glad.h"
#include <vector>

class ObjectFileReader
{
public:
	ObjectFileReader(const char* filePath);
	~ObjectFileReader() = default;

	std::vector<GLfloat> vertices() const;
	std::vector<GLfloat> textures() const;
	std::vector<GLfloat> normals() const;

	std::vector<GLuint> vertexIndices() const;
	std::vector<GLuint> textureIndices() const;
	std::vector<GLuint> normalIndices() const;

private:
	std::ifstream streamForFilePath(const char* filePath);

private:
	std::vector<GLfloat> vertices_;
	std::vector<GLfloat> textures_;
	std::vector<GLfloat> normals_;

	std::vector<GLuint> vertexIndices_;
	std::vector<GLuint> textureIndices_; 
	std::vector<GLuint> normalIndices_;
};