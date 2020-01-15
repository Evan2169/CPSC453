#pragma once

#include "glad/glad.h"

namespace GLUtility
{
	GLuint createVertexArrayObject();
	GLuint createBufferObject();

	void setupVertexArrayObject(GLuint vertexArrayObjectId, GLuint vertexBufferId);
}