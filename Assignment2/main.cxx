#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <functional>
#include <iterator>
#include <string>

#include "assignmentSpecific/GlfwCallbacks.h"
#include "assignmentSpecific/ObjectFileReader.h"
#include "assignmentSpecific/Shaders.h"
#include "glUtility/ShaderTools.h"
#include "glUtility/VertexArrayObjectConfiguration.h"
#include "glUtility/WireframeMesh.h"
#include "glfwUtility/GLFWBoilerplate.h"
#include "math/Matrices.h"
#include "math/Matrix.h"

using Vector = MathTypes::Vector<3, float>;

namespace
{
	void calculateCanonicalViewVolumeVariablesForVertices(const std::vector<GLfloat>& vertices);
	template<typename LoopFunction>
	void renderLoop(GLFWwindow* window, const LoopFunction& task);
	GLUtility::WireframeMesh<GLfloat, GLuint> setupMeshDataForArguments(int ac, char** av, 
		GLuint vertexArrayObjectId, GLuint textureId);
	void setupShadersDependingOnWhetherOrNotMeshUsesTextures(
		const GLUtility::WireframeMesh<GLfloat, GLuint>& mesh,
		GLuint& flatShaderProgramId,
		GLuint& phongShaderProgramId, 
		GLuint& gouraudProgramId);
	void loadCameraPositionUniformAndTextureMapToProgram(GLuint programId, GLuint textureId);

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifierKeys);
	void cursorPositionCallback(GLFWwindow* window, double updatedXPosition, double updatedYPosition);
	void scrollWheelCallback(GLFWwindow* window, double xOffset, double yOffset);
	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

  	auto translationMatrix = Matrices::identityMatrix3D<float>();
  	float translationVectorMagnitude;
  	auto rotationMatrix = Matrices::identityMatrix3D<float>();
  	auto scaleMatrix = Matrices::identityMatrix3D<float>();
  	
  	auto cameraMatrix = Matrices::identityMatrix3D<float>();
  	auto orthographicProjectionMatrix = Matrices::identityMatrix3D<float>();
  	auto perspectiveProjectionMatrix = Matrices::identityMatrix3D<float>();
  	auto projectionMatrix = Matrices::identityMatrix3D<float>();

	MathTypes::Matrix<4, 4, float> lightRotationMatrix = Matrices::identityMatrix3D<float>();
  	MathTypes::Vector<3, float> lightPosition(0, 0, 0);
  	MathTypes::Vector<3, float> cameraPosition(0, 0, 0);

  	GLuint currentProgramId;
  	std::array<GLuint, 3> programIds;
}


int main(int ac, char** av)
{
	auto* window = GLFWBoilerplate::initGLFW();
	
	auto vertexArrayObjectId = GLUtility::createVertexArrayObject();
	auto textureId = GLUtility::createTexture();
	auto mesh = setupMeshDataForArguments(ac, av, vertexArrayObjectId, textureId);
	GLuint flatShaderProgramId;
	GLuint phongShaderProgramId;
	GLuint gouraudProgramId;
	setupShadersDependingOnWhetherOrNotMeshUsesTextures(mesh, flatShaderProgramId, 
															phongShaderProgramId, gouraudProgramId);
	loadCameraPositionUniformAndTextureMapToProgram(flatShaderProgramId, textureId);
	loadCameraPositionUniformAndTextureMapToProgram(phongShaderProgramId, textureId);
	loadCameraPositionUniformAndTextureMapToProgram(gouraudProgramId, textureId);
	currentProgramId = flatShaderProgramId;

	glEnable(GL_DEPTH_TEST);
  	glEnable(GL_MULTISAMPLE);
  	glClearColor(0.3f, 0.0f, 0.3f, 0.0f);

  	glfwSetMouseButtonCallback(window, mouseButtonCallback);
  	glfwSetCursorPosCallback(window, cursorPositionCallback);
   glfwSetScrollCallback(window, scrollWheelCallback);
   glfwSetKeyCallback(window, keyboardCallback);

	renderLoop(window, [&]()
	{
   	glUseProgram(currentProgramId);
   	auto currentLightPosition = lightRotationMatrix * translationMatrix * rotationMatrix * scaleMatrix*
   		static_cast<MathTypes::Matrix<4, 1, float>>(lightPosition.homogenized());
	   glUniform3f(glGetUniformLocation(currentProgramId, "LightPosition"), 
	   	currentLightPosition[0][0], currentLightPosition[0][1], currentLightPosition[0][2]);
   	glUniformMatrix4fv(
   		glGetUniformLocation(currentProgramId, "Projection"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(projectionMatrix).data());
   	glUniformMatrix4fv(
   		glGetUniformLocation(currentProgramId, "View"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(cameraMatrix).data());
   	glUniformMatrix4fv(
   		glGetUniformLocation(currentProgramId, "Model"), 1, false, 
   		MatrixUtility::convertToColumnMajorArray(translationMatrix * rotationMatrix * scaleMatrix).data());
		glBindVertexArray(vertexArrayObjectId);
		glDrawElements(GL_TRIANGLES, mesh.numberOfVertexIndices(), GL_UNSIGNED_INT, (void *)0);
	});

	GLFWBoilerplate::terminateGLFW(window);
}

namespace
{
	void calculateCanonicalViewVolumeVariablesForVertices(const std::vector<GLfloat>& vertices)
	{
		const auto largest = std::abs(*std::max_element(std::begin(vertices), std::end(vertices), 
			[](GLfloat a, GLfloat b)
			{
				return std::abs(a) < std::abs(b);				
			}));
		const auto twoTimesLargest = 2.0 * largest;

		orthographicProjectionMatrix = Matrices::orthographicProjectionMatrix3D<float>(
			-twoTimesLargest, twoTimesLargest, -twoTimesLargest, twoTimesLargest, -twoTimesLargest * 2, twoTimesLargest * 2);
  		perspectiveProjectionMatrix = Matrices::perspectiveProjectionMatrix3D<float>(
  			-largest, largest, -largest, largest, largest*0.5, twoTimesLargest*2);
  		cameraPosition = Vector(0, 0, twoTimesLargest*1.1);
		cameraMatrix = Matrices::lookAtMatrix<float>(
			cameraPosition, 
			Vector(0, 0, 0), 
			Vector(0, 1, twoTimesLargest*1.1));
		projectionMatrix = orthographicProjectionMatrix;
  		lightPosition = Vector(largest, 0, 0);

		translationVectorMagnitude = largest * (1./12.5);
	}

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

	GLUtility::WireframeMesh<GLfloat, GLuint> setupMeshDataForArguments(int ac, char** av, GLuint vertexArrayObjectId, 
																														GLuint textureId)
	{
		if(ac != 2)
		{
			std::cout << 
			R"(
			Usage: ./AssignmentTwo_EvanHampton object
				
				object can be one of the following values: teapot, spot, nef_low, nef_high
			)" << std::endl;
			exit(-1);
		}
		else
		{
			std::string objectFilePath("");
			std::string textureFilePath("");

			if(strcmp(av[1], "teapot") == 0)
			{
				objectFilePath = "../models/teapot/teapot_triangulated.obj";
				textureFilePath = "";
			}
			else if(strcmp(av[1], "spot") == 0)
			{
				objectFilePath = "../models/spot/spot_triangulated.obj";
				textureFilePath = "../models/spot/spot_texture.png";
			}
			else if(strcmp(av[1], "nef_low") == 0)
			{
				objectFilePath = "../models/Nefertiti_Low/Nefertiti_Low.obj";
				textureFilePath = "../models/Nefertiti_Low/COLOR_Low.jpg";
			}
			else if(strcmp(av[1], "nef_high") == 0)
			{
				objectFilePath = "../models/Nefertiti_High/Nefertiti_High.obj";
				textureFilePath = "../models/Nefertiti_High/COLOR_High.jpg";
			}

			ObjectFileReader reader(objectFilePath.c_str());
			calculateCanonicalViewVolumeVariablesForVertices(reader.vertices());
			GLUtility::WireframeMesh mesh(reader.vertices(), reader.textures(), reader.normals(), reader.vertexIndices(),
												reader.textureIndices(), reader.normalIndices());
			GLUtility::setupVertexArrayObjectAndLoadMeshDataWithTextureSupport(
												vertexArrayObjectId, textureId, mesh, textureFilePath.c_str());
			return mesh;
		}
	}

	void setupShadersDependingOnWhetherOrNotMeshUsesTextures(
		const GLUtility::WireframeMesh<GLfloat, GLuint>& mesh,
		GLuint& flatShaderProgramId,
		GLuint& phongShaderProgramId, 
		GLuint& gouraudProgramId)
	{
		if(mesh.numberOfTextures() == 0)
		{
			flatShaderProgramId = GLUtility::createProgramFromShaders(
				&Shaders::FLAT_VERTEX_SHADER_NO_TEXTURE_SUPPORT, 
				&Shaders::FLAT_GEOMETRY_SHADER_NO_TEXTURE_SUPPORT,
				&Shaders::FLAT_FRAGMENT_SHADER_NO_TEXTURE_SUPPORT);
			gouraudProgramId = GLUtility::createProgramFromShaders(
				&Shaders::GOURAUD_VERTEX_SHADER_NO_TEXTURE_SUPPORT, 
				&Shaders::GOURAUD_FRAGMENT_SHADER_NO_TEXTURE_SUPPORT);
			phongShaderProgramId = GLUtility::createProgramFromShaders(
				&Shaders::PHONG_VERTEX_SHADER_NO_TEXTURE_SUPPORT, 
				&Shaders::PHONG_FRAGMENT_SHADER_NO_TEXTURE_SUPPORT);
		}
		else
		{
			flatShaderProgramId = GLUtility::createProgramFromShaders(
				&Shaders::FLAT_VERTEX_SHADER_TEXTURE_SUPPORT, 
				&Shaders::FLAT_GEOMETRY_SHADER_TEXTURE_SUPPORT,
				&Shaders::FLAT_FRAGMENT_SHADER_TEXTURE_SUPPORT);
			gouraudProgramId = GLUtility::createProgramFromShaders(
				&Shaders::GOURAUD_VERTEX_SHADER_TEXTURE_SUPPORT, 
				&Shaders::GOURAUD_FRAGMENT_SHADER_TEXTURE_SUPPORT);
			phongShaderProgramId = GLUtility::createProgramFromShaders(
				&Shaders::PHONG_VERTEX_SHADER_TEXTURE_SUPPORT, 
				&Shaders::PHONG_FRAGMENT_SHADER_TEXTURE_SUPPORT);
		}
		programIds[0] = flatShaderProgramId;
		programIds[1] = gouraudProgramId;
		programIds[2] = phongShaderProgramId;
	}

	void loadCameraPositionUniformAndTextureMapToProgram(GLuint programId, GLuint textureId)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
   	glUseProgram(programId);
		glUniform1i(glGetUniformLocation(programId, "ImageTexture"), 0);	
		glUniform3f(glGetUniformLocation(programId, "CameraPosition"), 
	   		cameraPosition.xValue(), cameraPosition.yValue(), cameraPosition.zValue());
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifierKeys)
	{
		GlfwCallbacks::handleMouseButtonInput(window, button, action, modifierKeys, projectionMatrix, 
																orthographicProjectionMatrix, perspectiveProjectionMatrix);
	}

	void cursorPositionCallback(GLFWwindow* window, double updatedXPosition, double updatedYPosition)
	{
		GlfwCallbacks::adjustModelMatrixForMouseMovement(window, updatedXPosition, updatedYPosition, translationVectorMagnitude,
																				translationMatrix, rotationMatrix);
	}

	void scrollWheelCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		GlfwCallbacks::adjustScaleMatrixForScrollWheelInput(window, xOffset, yOffset, scaleMatrix);
	}

	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		GlfwCallbacks::handleKeyboardInput(window, key, scancode, action, mods, programIds, currentProgramId, lightRotationMatrix);
	}
}