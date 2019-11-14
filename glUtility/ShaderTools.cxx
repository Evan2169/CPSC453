#include "ShaderTools.h"

#include <cstdlib>
#include <iostream>
#include <vector>

GLuint GLUtility::createProgramFromShaders(const char** vertexShaderCode, const char** fragmentShaderCode)
{
  GLuint vertexShaderId = createShader(vertexShaderCode, GL_VERTEX_SHADER);
  GLuint fragmentShaderId = createShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);
  checkProgramLinkStatus(programId);

  glDetachShader(programId, vertexShaderId);
  glDetachShader(programId, fragmentShaderId);
  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);

  return programId;
}

GLuint GLUtility::createProgramFromShaders(
  const char** vertexShaderCode, 
  const char** geometryShaderCode, 
  const char** fragmentShaderCode)
{
  GLuint vertexShaderId = createShader(vertexShaderCode, GL_VERTEX_SHADER);
  GLuint geometryShaderId = createShader(geometryShaderCode, GL_GEOMETRY_SHADER);
  GLuint fragmentShaderId = createShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, geometryShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);
  checkProgramLinkStatus(programId);

  glDetachShader(programId, fragmentShaderId);
  glDetachShader(programId, geometryShaderId);
  glDetachShader(programId, vertexShaderId);
  glDeleteShader(fragmentShaderId);
  glDeleteShader(geometryShaderId);
  glDeleteShader(vertexShaderId);

  return programId;
}

GLuint GLUtility::createShader(const char** shaderCode, GLenum shaderType)
{
  GLuint shaderId = glCreateShader(shaderType);
  glShaderSource(shaderId, 1, shaderCode, NULL);
  glCompileShader(shaderId);

  if(!checkShaderCompileStatus(shaderId))
  {
    exit(-1);
  }

  return shaderId;
}

bool GLUtility::checkShaderCompileStatus(GLuint shaderID) 
{
  GLint result;
  int infoLogLength;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
  if (!result) 
  {
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMsg(infoLogLength);
    glGetShaderInfoLog(shaderID, infoLogLength, nullptr, errorMsg.data());
    std::cerr << errorMsg.data() << std::endl;
    return false;
  }

  return true;
}

bool GLUtility::checkProgramLinkStatus(GLuint programID) 
{
  GLint result;
  int infoLogLength;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (!result) 
  {
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMsg(infoLogLength);
    glGetProgramInfoLog(programID, infoLogLength, nullptr, errorMsg.data());
    std::cerr << errorMsg.data() << std::endl;
    return false;
  }

  return true;
}