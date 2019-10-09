#include <array>
#include <cmath>
#include <cstdlib>
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <vector>

#include "assignmentSpecific/GlfwCallbacks.h"
#include "assignmentSpecific/MengerSponge.h"
#include "assignmentSpecific/Shaders.h"
#include "glUtility/DrawableCube.h"
#include "glUtility/ShaderTools.h"
#include "glUtility/VertexArrayObjectConfiguration.h"
#include "math/Matrices.h"

GLFWwindow* initGLFW();
void calculateAndLoadMVP(GLuint programId);
std::vector<GLUtility::Vertex<float, float>> mengerSpongeVertices();

int main() 
{
  GLFWwindow* window = initGLFW();

  //OpenGL setup
  GLuint programId = GLUtility::createProgramFromShaders(&Shaders::VERTEX_SHADER, &Shaders::FRAGMENT_SHADER);
  GLuint vertexArrayObjectId;
  glGenVertexArrays(1, &vertexArrayObjectId);
  GLuint vertexBufferId;
  glGenBuffers(1, &vertexBufferId);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  GLUtility::setupVertexArrayObject<float, float>(vertexArrayObjectId, vertexBufferId);

  //Render loop
  while (!glfwWindowShouldClose(window)) 
  {
    glViewport(0, 0, Callbacks::currentWindowWidth(), Callbacks::currentWindowHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    calculateAndLoadMVP(programId);

    auto vertices = mengerSpongeVertices();
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLUtility::Vertex<float, float>) * vertices.size(),
                                    vertices.data(), GL_STATIC_DRAW);
    glUseProgram(programId);
    glBindVertexArray(vertexArrayObjectId);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  //Cleanup
  glDeleteVertexArrays(1, &vertexArrayObjectId);
  glDeleteBuffers(1, &vertexBufferId);
  glDeleteProgram(programId);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void calculateAndLoadMVP(GLuint programId)
{
  const auto transformationMatrix 
    //Rotations
    = Matrices::rotateAboutX3D<float>(Callbacks::currentXRotationDegrees())
    * Matrices::rotateAboutY3D<float>(Callbacks::currentYRotationDegrees())
    * Matrices::rotateAboutZ3D<float>(Callbacks::currentZRotationDegrees())
    //Scaling
    * Matrices::uniformScaleMatrix3D<float>(Callbacks::currentZoomRatio());

  const float aspectRatio = (static_cast<float>(Callbacks::currentWindowWidth())
                                  / static_cast<float>(Callbacks::currentWindowHeight()));

  const auto camera = Matrices::lookAtMatrix<float>(
    MathTypes::Vector<3, float>(0, 0, 61), 
    MathTypes::Vector<3, float>(0, 0, 0),
    MathTypes::Vector<3, float>(0, 1, 0));
   
  if(Callbacks::currentPerspectiveEnabled())
  {
    const float oneHalfViewWidth = 60.0 * 0.5 * aspectRatio;
    const auto mvp = MatrixUtility::convertToColumnMajorArray<float>(
      Matrices::perspectiveProjectionMatrix3D<float>(-oneHalfViewWidth, oneHalfViewWidth, -30, 30, 21, 90) 
      * camera 
      * transformationMatrix).data();
    
    glUniformMatrix4fv(glGetUniformLocation(programId, "MVP"), 1, false, mvp);
  }
  else
  {
    const float oneHalfViewWidth = 100.0 * 0.5 * aspectRatio;
    const auto mvp = MatrixUtility::convertToColumnMajorArray<float>(
      Matrices::orthographicProjectionMatrix3D<float>(-oneHalfViewWidth, oneHalfViewWidth, -50, 50, -100, 100) 
      * camera 
      * transformationMatrix).data();

    glUniformMatrix4fv(glGetUniformLocation(programId, "MVP"), 1, false, mvp);
  }
}

std::vector<GLUtility::Vertex<float, float>> mengerSpongeVertices()
{
  MengerSponge spongeGenerator;
  MathTypes::Vector<3, float> bottomLeftCornerOfMengerSpongeInModelSpace(-25, -25, 25);
  auto mengerSpongeSubCubes = spongeGenerator.cubesForMengerSponge(Callbacks::currentMengerRecursionDepth(), 
                                                bottomLeftCornerOfMengerSpongeInModelSpace, 50);
  
    std::vector<GLUtility::Vertex<float, float>> vertices;
    for(auto subCube : mengerSpongeSubCubes)
    { 
      for(auto vertex : subCube.vertices())
      {
        vertices.push_back(vertex);
      }
    }
    vertices.shrink_to_fit();

    return vertices;
}

GLFWwindow* initGLFW()
{
  GLFWwindow* window = NULL;

  //Housekeeping
  {
    if (!glfwInit()) 
    {
      std::cout << "GLFW failed to initialize." << std::endl;
      exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
  
    window = glfwCreateWindow(Callbacks::INITIAL_WINDOW_DIMENSIONS, Callbacks::INITIAL_WINDOW_DIMENSIONS, 
                                "AssignmentOnePartTwo_EvanHampton", NULL, NULL);
    if (!window) 
    {
      glfwTerminate();
      exit(-1);
    }
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // vsync    
  }

  //Set up callbacks
  {
    glfwSetWindowSizeCallback(window, Callbacks::handleWindowSizeChanging);
    glfwSetScrollCallback(window, Callbacks::setCurrentMengerRecursionDepthBasedOnScrollWheel);
    glfwSetKeyCallback(window, Callbacks::handleKeyPressed);
  }

  return window;
}