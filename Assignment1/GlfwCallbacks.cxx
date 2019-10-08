#include "GlfwCallbacks.h"

#include <algorithm>
#include "GLFW/glfw3.h"
#include <iostream>

namespace
{
	static int minimumWindowDimension = Callbacks::INITIAL_WINDOW_DIMENSIONS;
	static int maximumWindowDimension = Callbacks::INITIAL_WINDOW_DIMENSIONS;
	static int windowWidth = Callbacks::INITIAL_WINDOW_DIMENSIONS;
	static int windowHeight = Callbacks::INITIAL_WINDOW_DIMENSIONS;

	static int mengerRecursionDepth = Callbacks::INITIAL_MENGER_RECURSION_DEPTH;

	static int xRotation = Callbacks::INITIAL_X_ROTATION_DEGREES;
	static int yRotation = Callbacks::INITIAL_Y_ROTATION_DEGREES;
	static int zRotation = Callbacks::INITIAL_Z_ROTATION_DEGREES;

	static float zoomRatio = Callbacks::INITIAL_ZOOM_RATIO;

	static bool perspectiveEnabled = Callbacks::INITIAL_PERSPECTIVE_ENABLED;
}

void Callbacks::handleWindowSizeChanging(GLFWwindow* window, int width, int height)
{
	minimumWindowDimension = width < height ? width : height;
	maximumWindowDimension = width > height ? width : height;
	windowWidth = width;
	windowHeight = height;
}

int Callbacks::currentMinimumWindowDimension()
{
	return minimumWindowDimension;
}

int Callbacks::currentMaximumWindowDimension()
{
	return maximumWindowDimension;
}

int Callbacks::currentWindowWidth()
{
	return windowWidth;
}

int Callbacks::currentWindowHeight()
{
	return windowHeight;
}

void Callbacks::setCurrentMengerRecursionDepthBasedOnScrollWheel(GLFWwindow* window, double xOffset, double yOffset)
{
	mengerRecursionDepth += yOffset;
	mengerRecursionDepth = std::clamp(mengerRecursionDepth, 1, 4);
}

int Callbacks::currentMengerRecursionDepth()
{
	return mengerRecursionDepth;
}

bool Callbacks::currentPerspectiveEnabled()
{
	return perspectiveEnabled;
}

void Callbacks::handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS && key == GLFW_KEY_P)
	{
		perspectiveEnabled = !perspectiveEnabled;
	}
	else if(action == GLFW_REPEAT || action == GLFW_RELEASE)
	{
		switch(key)
		{
		case(GLFW_KEY_Q):
			xRotation--;
			break;
		case(GLFW_KEY_W):
			xRotation++;
			break;
		case(GLFW_KEY_A):
			yRotation--;
			break;
		case(GLFW_KEY_S):
			yRotation++;
			break;
		case(GLFW_KEY_Z):
			zRotation--;
			break;
		case(GLFW_KEY_X):
			zRotation++;
			break;
		case(GLFW_KEY_D):
			zoomRatio*=0.99;
			break;
		case(GLFW_KEY_F):
			zoomRatio*=1.01;
			break;
		default:
			break;
		}
	}
}

int Callbacks::currentXRotationDegrees()
{
	return xRotation;
}

int Callbacks::currentYRotationDegrees()
{
	return yRotation;
}

int Callbacks::currentZRotationDegrees()
{
	return zRotation;
}

float Callbacks::currentZoomRatio()
{
	return zoomRatio;
}
