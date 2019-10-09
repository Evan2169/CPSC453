#pragma once

namespace Shaders
{
	static const char* VERTEX_SHADER =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 1) in vec3 vertexColour;
		uniform mat4 MVP;
		out vec3 colour;

		void main()
		{
			gl_Position = MVP * vec4(vertex, 1.0);
			colour = vertexColour;
		}
	)";

	static const char* FRAGMENT_SHADER =
	R"(
		#version 330 core

		in vec3 colour;
		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(colour, 1.0);
		}
	)";
}