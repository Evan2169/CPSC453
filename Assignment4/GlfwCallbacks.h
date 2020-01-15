#pragma once

#include <array>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace MathTypes
{
	template<int rows, int columns, typename CoordinatePrimitive> class Matrix;
	template<int dimensions, typename CoordinatePrimitive> class Vector;
}

namespace Curves
{
	class Curve;
	class Pot;
}

namespace GlfwCallbacks
{
	void handleMouseButtonInput(
		GLFWwindow* window, 
		int button, 
		int action, 
		int modifierKeys,
		Curves::Curve& curve,
		Curves::Pot& pot);
	
	void adjustControlPointsForMouseMovement(
		GLFWwindow* window, 
		double updatedXPosition, 
		double updatedYPosition,
		const MathTypes::Matrix<4, 4, float>& inverseTransformationMatrix,
		Curves::Curve& curve,
		GLFWcursor* cursorWhenHoveringOverPoint,
		Curves::Pot& pot);

	void modifyControlPointsForScrollWheelInput(
		GLFWwindow* window, 
		double xOffset, 
		double yOffset,
		Curves::Curve& curve,
		Curves::Pot& pot,
		MathTypes::Matrix<4, 4, float>& scaleMatrix);

	void handleKeyboardInput(
		GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mods,
		MathTypes::Matrix<4, 4, float>& rotationMatrix,
		MathTypes::Matrix<4, 4, float>& translationMatrix);
}