#include "GlfwCallbacks.h"

#include <algorithm>
#include <cmath>
#include <iterator>

#include "math/Matrices.h"
#include "math/Matrix.h"
#include "math/Vector.h"

namespace
{
	static bool currentlyLeftClicking = false;
	static bool currentlyRightClicking = false;

	static int currentXPosition = 0;
	static int currentYPosition = 0;

	static int currentScrollWheelOffset = 0;
	
	enum class ProjectionMatrixState
	{
		PerspectiveProjection,
		OrthographicProjection
	};
	static ProjectionMatrixState nextProjectionMatrixState = ProjectionMatrixState::PerspectiveProjection;

	bool wireframeModeCurrentlyEnabled = false;
}

void GlfwCallbacks::handleMouseButtonInput(
	GLFWwindow* window, 
	int button, 
	int action, 
	int modifierKeys, 
	MathTypes::Matrix<4, 4, float>& projectionMatrix,
	const MathTypes::Matrix<4, 4, float>& orthographicProjectionMatrix,
	const MathTypes::Matrix<4, 4, float>& perspectiveProjectionMatrix)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		currentlyLeftClicking = action == GLFW_PRESS ? true : false;
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		currentlyRightClicking = action == GLFW_PRESS ? true : false;
	}
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		switch(nextProjectionMatrixState)
		{
			case(ProjectionMatrixState::PerspectiveProjection):
				projectionMatrix = perspectiveProjectionMatrix;
				nextProjectionMatrixState = ProjectionMatrixState::OrthographicProjection;
				break;
			case(ProjectionMatrixState::OrthographicProjection):
				projectionMatrix = orthographicProjectionMatrix;
				nextProjectionMatrixState = ProjectionMatrixState::PerspectiveProjection;
				break;
		}
	}
}

void GlfwCallbacks::adjustModelMatrixForMouseMovement(
	GLFWwindow* window, 
	double updatedXPosition, 
	double updatedYPosition,
	float translationVectorMagnitude,
	MathTypes::Matrix<4, 4, float>& translationMatrix, 
	MathTypes::Matrix<4, 4, float>& rotationMatrix)
{
	if(currentlyLeftClicking)
	{
		float xDelta = updatedXPosition - currentXPosition;
		float yDelta = updatedYPosition - currentYPosition;
		const auto movementVector = MathTypes::Vector<3, float>(xDelta, yDelta, 0).normalized();

		translationMatrix = translationMatrix * Matrices::translationMatrix3D<float>(
															 (movementVector.xValue())*translationVectorMagnitude, 
															 (movementVector.yValue())*-translationVectorMagnitude, 0);
	}
	else if(currentlyRightClicking)
	{
		const float xDelta = updatedXPosition - currentXPosition;
		const float yDelta = updatedYPosition - currentYPosition;
		const float comparisonEpsilon = 0.001;

		//We want to rotate about a vector orthogonal to mouse movement. 
		if(std::abs(xDelta) < comparisonEpsilon)
		{
			const auto rotateAboutVector = MathTypes::Vector<3, float>(-yDelta, 0, 0).normalized();
			rotationMatrix = rotationMatrix * Matrices::rotateAboutLine<float>(rotateAboutVector, 5);
		}
		else if(std::abs(yDelta) < comparisonEpsilon)
		{
			const auto rotateAboutVector = MathTypes::Vector<3, float>(0, xDelta, 0).normalized();
			rotationMatrix = rotationMatrix * Matrices::rotateAboutLine<float>(rotateAboutVector, 5);
		}
		else
		{
			const auto rotateAboutVector = MathTypes::Vector<3, float>(-yDelta, -xDelta, 0).normalized();
			rotationMatrix = rotationMatrix * Matrices::rotateAboutLine<float>(rotateAboutVector, 5);
		}
	}

	currentXPosition = updatedXPosition;
	currentYPosition = updatedYPosition;
}

void GlfwCallbacks::adjustScaleMatrixForScrollWheelInput(
	GLFWwindow* window, 
	double xOffset, 
	double yOffset,
	MathTypes::Matrix<4, 4, float>& scaleMatrix)
{
	int newScrollWheelOffset = currentScrollWheelOffset + static_cast<int>(yOffset);

	if(newScrollWheelOffset < currentScrollWheelOffset)
	{
		scaleMatrix = scaleMatrix * Matrices::uniformScaleMatrix3D<float>(0.90);
	}
	else if(newScrollWheelOffset > currentScrollWheelOffset)
	{
		scaleMatrix = scaleMatrix * Matrices::uniformScaleMatrix3D<float>(1.10);
	}
}

void GlfwCallbacks::handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods,
					std::array<GLuint, 3>& programIds, GLuint& currentProgramId, MathTypes::Matrix<4, 4, float>& lightRotationMatrix)
{
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
			case(GLFW_KEY_W):
			{
				if(wireframeModeCurrentlyEnabled)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				wireframeModeCurrentlyEnabled = !wireframeModeCurrentlyEnabled;
				break;
			}
			case(GLFW_KEY_S):
			{
				std::rotate(std::begin(programIds), std::begin(programIds) + 1, std::end(programIds));
				currentProgramId = programIds[0];
				break;
			}
		}
	}
	else if (action == GLFW_REPEAT)
	{
		switch(key)
		{
			case(GLFW_KEY_A):
				lightRotationMatrix = Matrices::rotateAboutY3D<float>(5) * lightRotationMatrix;
				break;
		}
	}
}
