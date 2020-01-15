#include "GlfwCallbacks.h"

#include <iostream>

#include <algorithm>
#include <cmath>
#include <iterator>

#include "assignmentSpecific/Curve.h"
#include "assignmentSpecific/Pot.h"
#include "glUtility/Vertex.h"
#include "glfwUtility/GLFWBoilerplate.h"
#include "math/Matrices.h"
#include "math/Matrix.h"
#include "math/Vector.h"

namespace
{
	static bool currentlyDragging = false;
	static MathTypes::Vector<3, float>* coordinateBeingDragged = NULL;
	static MathTypes::Vector<3, float> currentCursorPositionInWorldSpace(0, 0, 0);
	static float currentXPixelPosition = 0;
	static float currentYPixelPosition = 0;

	static int currentScrollWheelOffset = 0;

	bool currentCursorPositionIsWithinControlCurveViewport();
	bool currentCursorPositionIsWithinPotteryViewport();
}

void GlfwCallbacks::handleMouseButtonInput(
	GLFWwindow* window, 
	int button, 
	int action, 
	int modifierKeys,
	Curves::Curve& curve,
	Curves::Pot& pot)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		auto* intersectedCoordinate = curve.intersectedCoordinate(currentCursorPositionInWorldSpace);
		if(action == GLFW_PRESS && intersectedCoordinate != NULL)
		{
			coordinateBeingDragged = intersectedCoordinate;
		}
		else if(action == GLFW_RELEASE)
		{
			coordinateBeingDragged = NULL;
		}
	}
}

void GlfwCallbacks::adjustControlPointsForMouseMovement(
	GLFWwindow* window, 
	double updatedXPosition, 
	double updatedYPosition,
	const MathTypes::Matrix<4, 4, float>& inverseTransformationMatrix,
	Curves::Curve& curve,
	GLFWcursor* cursorWhenHoveringOverPoint,
	Curves::Pot& pot)
{
	auto screenCoordinateInWorldSpace = 
			inverseTransformationMatrix 
		 * static_cast<MathTypes::Matrix<4, 1, float>>(
		 		MathTypes::Vector<3, float>(updatedXPosition - (WindowDimensions::INITIAL_WINDOW_WIDTH/2), updatedYPosition, 0).homogenized());
		
		currentCursorPositionInWorldSpace = MathTypes::Vector<3, float>(
			screenCoordinateInWorldSpace[0][0],
			-screenCoordinateInWorldSpace[1][0],
			0);

	if(coordinateBeingDragged != NULL && currentCursorPositionIsWithinControlCurveViewport())
	{
		*coordinateBeingDragged = currentCursorPositionInWorldSpace;
		pot.setControlCurve(curve);
	}

	if(curve.intersectedCoordinate(currentCursorPositionInWorldSpace) != NULL)
	{
	 	glfwSetCursor(window, cursorWhenHoveringOverPoint);
	}
	else
	{
		glfwSetCursor(window, NULL);
	}
	currentXPixelPosition = updatedXPosition;
	currentYPixelPosition = updatedYPosition;
}

void GlfwCallbacks::modifyControlPointsForScrollWheelInput(
	GLFWwindow* window, 
	double xOffset, 
	double yOffset,
	Curves::Curve& curve,
	Curves::Pot& pot,
	MathTypes::Matrix<4, 4, float>& scaleMatrix)
{
	int newScrollWheelOffset = currentScrollWheelOffset + static_cast<int>(yOffset);

	if(newScrollWheelOffset < currentScrollWheelOffset)
	{
		if(currentCursorPositionIsWithinControlCurveViewport())
		{
			curve.deletePoint();
			pot.setControlCurve(curve);
		}
		else if(currentCursorPositionIsWithinPotteryViewport())
		{
			scaleMatrix = Matrices::uniformScaleMatrix3D<float>(0.90) * scaleMatrix;
		}
	}
	else if(newScrollWheelOffset > currentScrollWheelOffset)
	{
		if(currentCursorPositionIsWithinControlCurveViewport())
		{
			curve.insertPoint();
			pot.setControlCurve(curve);
		}
		else if(currentCursorPositionIsWithinPotteryViewport())
		{
			scaleMatrix = Matrices::uniformScaleMatrix3D<float>(1.1) * scaleMatrix;
		}
	}
}

void GlfwCallbacks::handleKeyboardInput(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods,
	MathTypes::Matrix<4, 4, float>& rotationMatrix,
	MathTypes::Matrix<4, 4, float>& translationMatrix)
{
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
			case(GLFW_KEY_Q):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutX3D<float>(-5);
				break;
			}
			case(GLFW_KEY_W):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutX3D<float>(5);
				break;	
			}
			case(GLFW_KEY_A):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutY3D<float>(-5);
				break;
			}
			case(GLFW_KEY_S):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutY3D<float>(5);
				break;
			}
			case(GLFW_KEY_Z):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutZ3D<float>(-5);
				break;
			}
			case(GLFW_KEY_X):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutZ3D<float>(5);
				break;	
			}
		}
	}
	else if (action == GLFW_REPEAT)
	{
		switch(key)
		{
			case(GLFW_KEY_Q):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutX3D<float>(-5);
				break;
			}
			case(GLFW_KEY_W):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutX3D<float>(5);
				break;	
			}
			case(GLFW_KEY_A):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutY3D<float>(-5);
				break;
			}
			case(GLFW_KEY_S):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutY3D<float>(5);
				break;
			}
			case(GLFW_KEY_Z):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutZ3D<float>(-5);
				break;
			}
			case(GLFW_KEY_X):
			{
				rotationMatrix = rotationMatrix * Matrices::rotateAboutZ3D<float>(5);
				break;	
			}
		}
	}
}

namespace
{
	bool currentCursorPositionIsWithinControlCurveViewport()
	{
		float xPosition = currentCursorPositionInWorldSpace.xValue();
		float yPosition = currentCursorPositionInWorldSpace.yValue();
	
		if(xPosition <= 0 || xPosition >= 50 || yPosition <= -50 || yPosition >= 50)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool currentCursorPositionIsWithinPotteryViewport()
	{
		if(currentXPixelPosition < 0 || currentXPixelPosition > 500 || currentYPixelPosition < 0 || currentYPixelPosition > 500)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}