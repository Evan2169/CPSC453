#pragma once

struct GLFWwindow;

namespace Callbacks
{
	static const int   INITIAL_WINDOW_DIMENSIONS = 800;
	static const int   INITIAL_MENGER_RECURSION_DEPTH = 1;
	static const int   INITIAL_X_ROTATION_DEGREES = 0;
	static const int   INITIAL_Y_ROTATION_DEGREES = 0;
	static const int 	 INITIAL_Z_ROTATION_DEGREES = 0;
	static const float INITIAL_ZOOM_RATIO = 1.0;
	static const bool  INITIAL_PERSPECTIVE_ENABLED = false;

	void handleWindowSizeChanging(GLFWwindow* window, int width, int height);
	int currentMinimumWindowDimension();
	int currentMaximumWindowDimension();
	int currentWindowWidth();
	int currentWindowHeight();

	void setCurrentMengerRecursionDepthBasedOnScrollWheel(GLFWwindow* window, double xOffset, double yOffset);
	int currentMengerRecursionDepth();

	void handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

	int currentXRotationDegrees();
	int currentYRotationDegrees();
	int currentZRotationDegrees();

	float currentZoomRatio();

	bool currentPerspectiveEnabled();
}