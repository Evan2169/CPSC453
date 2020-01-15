#include "GLFWBoilerplate.h"

#include <cstdlib>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

GLFWwindow* GLFWBoilerplate::initGLFW()
{
	if (!glfwInit()) 
  {
    std::cerr << "GLFW failed to initialize." << std::endl;
    exit(-1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(
    WindowDimensions::INITIAL_WINDOW_WIDTH, 
   	WindowDimensions::INITIAL_WINDOW_HEIGHT, 
    "AssignmentOnePartTwo_EvanHampton", NULL, NULL);
  if (!window) 
  {
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1); // vsync    
  
  return window;
}
	
void GLFWBoilerplate::terminateGLFW(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}