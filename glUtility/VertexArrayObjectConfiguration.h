#pragma once

#include <cstddef>
#include "glad/glad.h"

#include "glUtility/Vertex.h"

namespace GLUtility
{
	template<typename CoordinatePrimitive, typename ColourPrimitive>
	void setupVertexArrayObject(GLuint vertexArrayObjectId, GLuint vertexBufferId);
}

template<typename CoordinatePrimitive, typename ColourPrimitive>
void GLUtility::setupVertexArrayObject(GLuint vertexArrayObjectId, GLuint vertexBufferId)
{
	glBindVertexArray(vertexArrayObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	using VertexType = GLUtility::Vertex<CoordinatePrimitive, ColourPrimitive>;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), 
		(void *)offsetof(VertexType, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
		(void *)offsetof(VertexType, colour));
}