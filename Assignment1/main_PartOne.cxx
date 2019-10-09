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
#include "glUtility/DrawableCube.h"
#include "math/Matrices.h"

GLFWwindow* init();

int main() 
{
  GLFWwindow* window = init();


  while (!glfwWindowShouldClose(window)) 
  {
    glViewport(0, 0, Callbacks::currentWindowWidth(), Callbacks::currentWindowHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);

    auto camera = Matrices::lookAtMatrix<float>(
      MathTypes::Vector<3, float>(0, 0, 61), 
      MathTypes::Vector<3, float>(0, 0, 0),
      MathTypes::Vector<3, float>(0, 1, 0));
    auto ortho = Matrices::orthographicProjectionMatrix3D<float>(-50, 50, -50, 50, 50, -50);

    if(Callbacks::currentPerspectiveEnabled())
    {
      glLoadMatrixf(MatrixUtility::convertToColumnMajorArray<float>(
        Matrices::perspectiveProjectionMatrix3D<float>(-30, 30, -30, 30, 11, 100) * camera).data());
    }
    else
    {
      glLoadMatrixf(MatrixUtility::convertToColumnMajorArray<float>(ortho * camera).data());
    }

    auto transformationMatrix 
    //Rotations
    = Matrices::rotateAboutX3D<float>(Callbacks::currentXRotationDegrees())
    * Matrices::rotateAboutY3D<float>(Callbacks::currentYRotationDegrees())
    * Matrices::rotateAboutZ3D<float>(Callbacks::currentZRotationDegrees())
    //Scaling
    * Matrices::uniformScaleMatrix3D<float>(Callbacks::currentZoomRatio());
    //Load matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(MatrixUtility::convertToColumnMajorArray<float>(transformationMatrix).data());    

    MengerSponge spongeGenerator;
    MathTypes::Vector<3, float> bottomLeftCornerOfMengerSpongeInOrthographicViewVolume(-25, -25, 25);
    auto mengerSpongeSubCubes = spongeGenerator.cubesForMengerSponge(Callbacks::currentMengerRecursionDepth(), 
                                        bottomLeftCornerOfMengerSpongeInOrthographicViewVolume, 50);
    for(auto subCube : mengerSpongeSubCubes)
    {                                     
      subCube.render();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

GLFWwindow* init()
{
  GLFWwindow* window = NULL;

  //Housekeeping
  {
    if (!glfwInit()) 
    {
      std::cout << "GLFW failed to initialize." << std::endl;
      exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  
    window = glfwCreateWindow(Callbacks::INITIAL_WINDOW_DIMENSIONS, Callbacks::INITIAL_WINDOW_DIMENSIONS, 
                                "AssignmentOne_EvanHampton", NULL, NULL);
    if (!window) 
    {
      glfwTerminate();
      exit(-1);
    }
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // vsync
    glEnable(GL_DEPTH_TEST);  
  }

  //Set up callbacks
  {
    glfwSetWindowSizeCallback(window, Callbacks::handleWindowSizeChanging);
    glfwSetScrollCallback(window, Callbacks::setCurrentMengerRecursionDepthBasedOnScrollWheel);
    glfwSetKeyCallback(window, Callbacks::handleKeyPressed);
  }

  return window;
}