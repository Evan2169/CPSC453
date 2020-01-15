#include "VertexArrayObjectConfiguration.h"

#include "glUtility/Vertex.h"

GLuint GLUtility::createVertexArrayObject()
{
	GLuint id;
	glGenVertexArrays(1, &id);
	return id;
}

GLuint GLUtility::createBufferObject()
{
	GLuint bufferId;
  	glGenBuffers(1, &bufferId);
  	return bufferId;
}


void GLUtility::setupVertexArrayObject(GLuint vertexArrayObjectId, GLuint vertexBufferId)
{
	glBindVertexArray(vertexArrayObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	using VertexType = GLUtility::Vertex<float, float>;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), 
		(void *)offsetof(VertexType, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
		(void *)offsetof(VertexType, colour));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
		(void *)offsetof(VertexType, normal));
}