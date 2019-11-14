#pragma once

#include <cstddef>
#include "glad/glad.h"
#include <iostream>
#include "stb_image.h"

#include "glUtility/WireframeMesh.h"

namespace GLUtility
{
	GLuint createVertexArrayObject();
	GLuint createTexture();

	template<typename CoordinatePrimitive, typename IndexPrimitive>
	void setupVertexArrayObjectAndLoadMeshData(
		GLuint vertexArrayObjectId, 
		const GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>& mesh);

	template<typename CoordinatePrimitive, typename IndexPrimitive>
	void setupVertexArrayObjectAndLoadMeshDataWithTextureSupport(
		GLuint vertexArrayObjectId, 
		const GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>& mesh,
		const char* textureFilePath);

}

template<typename CoordinatePrimitive, typename IndexPrimitive>
void GLUtility::setupVertexArrayObjectAndLoadMeshDataWithTextureSupport(
	GLuint vertexArrayObjectId, 
	GLuint textureId, 
	const GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>& mesh,
	const char* textureFilePath)
{
	GLuint bufferIds[2];
	glGenBuffers(2, bufferIds);
	
	glBindVertexArray(vertexArrayObjectId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIds[0]);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(IndexPrimitive) * mesh.numberOfVertexIndices(),
		mesh.vertexIndices(),
		GL_STATIC_DRAW);

	size_t bufferOffsetOfVertices = 0;
	size_t bufferOffsetOfTextures = sizeof(CoordinatePrimitive) * mesh.numberOfVertices();
	size_t bufferOffsetOfNormals = bufferOffsetOfTextures + sizeof(CoordinatePrimitive) * mesh.numberOfTextures();

	glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)bufferOffsetOfVertices);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)bufferOffsetOfTextures);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)bufferOffsetOfNormals);

	size_t sizeOfVertexBuffer = sizeof(CoordinatePrimitive) * mesh.numberOfVertices()
											+ sizeof(CoordinatePrimitive) * mesh.numberOfTextures();
											+ sizeof(CoordinatePrimitive) * mesh.numberOfNormals();
  	glBufferData(GL_ARRAY_BUFFER, sizeOfVertexBuffer, NULL, GL_STATIC_DRAW);
 	glBufferSubData(GL_ARRAY_BUFFER, bufferOffsetOfVertices, 
 		sizeof(CoordinatePrimitive) * mesh.numberOfVertices(), mesh.vertices());
 	glBufferSubData(GL_ARRAY_BUFFER, bufferOffsetOfTextures, 
 		sizeof(CoordinatePrimitive) * mesh.numberOfTextures(), mesh.textures());
 	glBufferSubData(GL_ARRAY_BUFFER, bufferOffsetOfNormals, 
 		sizeof(CoordinatePrimitive) * mesh.numberOfNormals(), mesh.normals());

  	stbi_set_flip_vertically_on_load(true);
 	int width, height, channels;
 	auto* image = stbi_load(textureFilePath, &width, &height, &channels, 0);
 	if(!image)
 	{
 		std::cerr << "Could not open texture file.\n" << std::endl;
 	}

 	glActiveTexture(GL_TEXTURE0);	
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	auto format = GL_RGBA;
	if (channels == 3)
	{
	 	format = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
}