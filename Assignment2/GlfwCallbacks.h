#pragma once

#include <array>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace MathTypes
{
	template<int rows, int columns, typename CoordinatePrimitive> class Matrix;
	template<int dimensions, typename CoordinatePrimitive> class Vector;
}

namespace GlfwCallbacks
{
	void handleMouseButtonInput(
		GLFWwindow* window, 
		int button, 
		int action, 
		int modifierKeys,
		MathTypes::Matrix<4, 4, float>& projectionMatrix,
		const MathTypes::Matrix<4, 4, float>& orthographicProjectionMatrix,
		const MathTypes::Matrix<4, 4, float>& perspectiveProjectionMatrix);
	
	void adjustModelMatrixForMouseMovement(
		GLFWwindow* window, 
		double updatedXPosition, 
		double updatedYPosition, 
		float translationVectorMagnitude,
		MathTypes::Matrix<4, 4, float>& translationMatrix,
		MathTypes::Matrix<4, 4, float>& rotationMatrix);

	void adjustScaleMatrixForScrollWheelInput(
		GLFWwindow* window, 
		double xOffset, 
		double yOffset,
		MathTypes::Matrix<4, 4, float>& scaleMatrix);

	void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods, 
					std::array<GLuint, 3>& programIds, GLuint& currentProgramId, MathTypes::Matrix<4, 4, float>& lightRotationMatrix);
}