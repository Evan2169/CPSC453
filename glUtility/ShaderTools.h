#pragma once

#include "glad/glad.h"

namespace GLUtility 
{
	GLuint createProgramFromShaders(const char** vertexShaderCode, const char** fragmentShaderCode);
	GLuint createShader(const char** shaderCode, GLenum shaderType);

	bool checkShaderCompileStatus(GLuint shaderID);
	bool checkProgramLinkStatus(GLuint programID);
}
