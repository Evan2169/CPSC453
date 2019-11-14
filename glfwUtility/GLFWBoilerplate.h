#pragma once

class GLFWwindow;

namespace WindowDimensions
{
	static const int INITIAL_WINDOW_WIDTH = 800;
	static const int INITIAL_WINDOW_HEIGHT = 800;
}

namespace GLFWBoilerplate
{
	GLFWwindow* initGLFW();
	void terminateGLFW(GLFWwindow* window);
}