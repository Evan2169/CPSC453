#include "VertexArrayObjectConfiguration.h"

GLuint GLUtility::createVertexArrayObject()
{
	GLuint id;
	glGenVertexArrays(1, &id);
	return id;
}

GLuint GLUtility::createTexture()
{
  GLuint id;
  glGenTextures(1, &id);
  return id;
}