#pragma once

class GLFWwindow;

namespace WindowDimensions
{
	static const int INITIAL_WINDOW_WIDTH = 1000;
	static const int INITIAL_WINDOW_HEIGHT = 1000;
}

namespace GLFWBoilerplate
{
	GLFWwindow* initGLFW();
	void terminateGLFW(GLFWwindow* window);
}