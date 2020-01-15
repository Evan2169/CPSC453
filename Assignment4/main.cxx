#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "assignmentSpecific/Curve.h"
#include "assignmentSpecific/GlfwCallbacks.h"
#include "assignmentSpecific/Pot.h"
#include "assignmentSpecific/Shaders.h"
#include "assignmentSpecific/VertexArrayObjectConfiguration.h"
#include "glUtility/ShaderTools.h"
#include "glUtility/Vertex.h"
#include "glfwUtility/GLFWBoilerplate.h"
#include "math/Matrices.h"
#include "math/Matrix.h"

namespace
{

	template<typename LoopFunction>
	void renderLoop(GLFWwindow* window, const LoopFunction& task);
	void loadCameraAndLightPositionUniformsToProgram(GLuint programId);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifierKeys);
	void cursorPositionCallback(GLFWwindow* window, double updatedXPosition, double updatedYPosition);
	void scrollWheelCallback(GLFWwindow* window, double xOffset, double yOffset);
	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

  	auto potteryTranslationMatrix = Matrices::identityMatrix3D<float>();
  	auto potteryRotationMatrix = Matrices::identityMatrix3D<float>();
  	auto potteryScaleMatrix = Matrices::identityMatrix3D<float>();	
  	const MathTypes::Vector<3, float> lightPosition(10, 10, 60);
  	const MathTypes::Vector<3, float> cameraPosition(0, 0, 61);
  	const auto cameraMatrix = Matrices::lookAtMatrix<float>(
   	cameraPosition, 
   	MathTypes::Vector<3, float>(0, 0, 0),
   	MathTypes::Vector<3, float>(0, 1, 0));
  	const auto potteryProjectionMatrix = 
  		Matrices::orthographicProjectionMatrix3D<float>(-50, 50, -50, 50, -100, 100);
  	const auto controlCurveProjectionMatrix = 
  		Matrices::orthographicProjectionMatrix3D<float>(0, 50, -50, 50, -100, 100);
  	const auto viewportMatrix = 
  		Matrices::viewportMatrix3D<float>(WindowDimensions::INITIAL_WINDOW_WIDTH / 2, WindowDimensions::INITIAL_WINDOW_HEIGHT);

  	Curves::Curve controlCurve(
		MathTypes::Vector<3, float>(25, 20, 0), 
		MathTypes::Vector<3, float>(25, -20, 0), 
		GLUtility::Colour<float>(.3, .3, .5),
		GLUtility::Colour<float>(.3, .5, .5));
  	Curves::Pot pot(controlCurve, GLUtility::Colour<float>(0.7, 0.2, 0.2));
  	GLFWcursor* handCursor;
}


int main(int ac, char** av)
{
	auto* window = GLFWBoilerplate::initGLFW();
  	glfwSetMouseButtonCallback(window, mouseButtonCallback);
  	glfwSetCursorPosCallback(window, cursorPositionCallback);
  	glfwSetScrollCallback(window, scrollWheelCallback);
  	glfwSetKeyCallback(window, keyboardCallback);
	handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	
	auto vertexArrayObjectIdForControlCurve = GLUtility::createVertexArrayObject();
	auto vertexBufferObjectIdForControlCurve = GLUtility::createBufferObject();
	auto vertexArrayObjectIdForPotteryCurve = GLUtility::createVertexArrayObject();
	auto vertexBufferObjectIdForPotteryCurve = GLUtility::createBufferObject();

	GLuint lineProgram = 
		GLUtility::createProgramFromShaders(
		&Shaders::LINE_VERTEX_SHADER, 
		&Shaders::LINE_FRAGMENT_SHADER);
	GLUtility::setupVertexArrayObject(
		vertexArrayObjectIdForControlCurve, 
		vertexBufferObjectIdForControlCurve);
	GLuint potteryProgram = 
		GLUtility::createProgramFromShaders(
		&Shaders::POTTERY_VERTEX_SHADER, 
		&Shaders::POTTERY_FRAGMENT_SHADER);
	GLUtility::setupVertexArrayObject(
		vertexArrayObjectIdForPotteryCurve, 
		vertexBufferObjectIdForPotteryCurve);
	loadCameraAndLightPositionUniformsToProgram(potteryProgram);

	glEnable(GL_DEPTH_TEST);
  	glEnable(GL_MULTISAMPLE);
	glPointSize(10);
  	glClearColor(0.3f, 0.0f, 0.3f, 0.0f);

	renderLoop(window, [&]()
	{
		//Deal with control points
		glViewport(
			WindowDimensions::INITIAL_WINDOW_WIDTH / 2,
			0,
			WindowDimensions::INITIAL_WINDOW_WIDTH / 2,
			WindowDimensions::INITIAL_WINDOW_HEIGHT);
		glUseProgram(lineProgram);
   	glUniformMatrix4fv(
   		glGetUniformLocation(lineProgram, "Projection"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(controlCurveProjectionMatrix).data());
   	glUniformMatrix4fv(
   		glGetUniformLocation(lineProgram, "View"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(cameraMatrix).data());
   	glUniformMatrix4fv(
   		glGetUniformLocation(lineProgram, "Model"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(Matrices::identityMatrix3D<float>()).data());
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectIdForControlCurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLUtility::Vertex<float, float>) * controlCurve.verticesOfBothCurves().size(), 
			controlCurve.verticesOfBothCurves().data(), GL_STATIC_DRAW);
		glBindVertexArray(vertexArrayObjectIdForControlCurve);
		glDrawArrays(GL_POINTS, 0, controlCurve.vertices().size());
		glDrawArrays(GL_LINE_STRIP, 0, controlCurve.vertices().size());
		glDrawArrays(GL_LINE_STRIP, controlCurve.vertices().size(), controlCurve.verticesOfSmoothedCurve().size());


		//Deal with pottery
   	glViewport(0, 0, WindowDimensions::INITIAL_WINDOW_WIDTH / 2, WindowDimensions::INITIAL_WINDOW_HEIGHT);
   	glUseProgram(potteryProgram);
   	glUniformMatrix4fv(
   		glGetUniformLocation(potteryProgram, "Projection"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(potteryProjectionMatrix).data());
   	glUniformMatrix4fv(
   		glGetUniformLocation(potteryProgram, "View"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(cameraMatrix).data());
   	glUniformMatrix4fv(
   		glGetUniformLocation(potteryProgram, "Model"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(potteryTranslationMatrix * potteryRotationMatrix * potteryScaleMatrix).data());
   	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectIdForPotteryCurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLUtility::Vertex<float, float>) * pot.vertices().size(), 
			pot.vertices().data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(vertexArrayObjectIdForPotteryCurve);
		glDrawArrays(GL_TRIANGLES, 0, pot.vertices().size());
	});

	glfwDestroyCursor(handCursor);
	GLFWBoilerplate::terminateGLFW(window);
}

namespace
{
	template<typename LoopFunction>
	void renderLoop(GLFWwindow* window, const LoopFunction& task)
	{
		while(!glfwWindowShouldClose(window))
		{
	   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	   	task();
	
	   	glfwSwapBuffers(window);
	    	glfwPollEvents();  
		}
	}

	void loadCameraAndLightPositionUniformsToProgram(GLuint programId)
	{
   	glUseProgram(programId);
		glUniform3f(glGetUniformLocation(programId, "CameraPosition"), 
	   		cameraPosition.xValue(), cameraPosition.yValue(), cameraPosition.zValue());
		glUniform3f(glGetUniformLocation(programId, "LightPosition"), 
	   		lightPosition.xValue(), lightPosition.yValue(), lightPosition.zValue());
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifierKeys)
	{
		GlfwCallbacks::handleMouseButtonInput(window, button, action, modifierKeys, controlCurve, pot);
	}

	void cursorPositionCallback(GLFWwindow* window, double updatedXPosition, double updatedYPosition)
	{
		GlfwCallbacks::adjustControlPointsForMouseMovement(
			window, 
			updatedXPosition, 
			updatedYPosition,
			LinearMath::inverse(viewportMatrix * cameraMatrix * controlCurveProjectionMatrix),
			controlCurve,
			handCursor,
			pot);
	}

	void scrollWheelCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		GlfwCallbacks::modifyControlPointsForScrollWheelInput(window, xOffset, yOffset, controlCurve, pot, potteryScaleMatrix);
	}

	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		GlfwCallbacks::handleKeyboardInput(window, key, scancode, action, mods, potteryRotationMatrix, potteryTranslationMatrix);
	}
}