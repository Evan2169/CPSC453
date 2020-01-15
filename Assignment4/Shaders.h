#pragma once

namespace Shaders
{
	static const char* POTTERY_VERTEX_SHADER =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 1) in vec3 vertexColour;
		layout (location = 2) in vec3 vertexNormal;
		
		out vec3 VertexPosition;
		out vec3 VertexColour;
		out vec3 VertexNormal;
		out vec3 Light;

		uniform vec3 LightPosition;
		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			VertexPosition = (Model * vec4(vertex, 1)).xyz;
			VertexNormal = normalize(Model * vec4(vertexNormal, 1)).xyz;
			VertexColour = vertexColour;
			Light = LightPosition;
		}
	)";

	static const char* POTTERY_FRAGMENT_SHADER =
	R"(
		#version 330 core

		in vec3 VertexPosition;
		in vec3 VertexNormal;
		in vec3 VertexColour;
		in vec3 Light;

		uniform vec3 CameraPosition;

		out vec4 FragColor;

		void main()
		{
			vec3 lightColour = vec3(1.0, 1.0, 1.0);
			vec3 lightDirection = normalize(Light - VertexPosition);
			vec3 cameraDirection = normalize(CameraPosition - VertexPosition);
			
			float ambient = 0.2;
			float diffuse = abs(dot(lightDirection, VertexNormal));
			float specular = pow(max(dot(reflect(-lightDirection, normalize(VertexNormal)), cameraDirection), 0.0), 32) * 0.5;

			FragColor = vec4(clamp(VertexColour * (ambient + diffuse + specular), 0.0, 1.0), 1.0);
		}
	)";

	static const char* LINE_VERTEX_SHADER =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 1) in vec3 vertexColour;
		layout (location = 2) in vec3 vertexNormal;
		
		out vec3 VertexColour;

		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			VertexColour = vertexColour;
		}
	)";

	static const char* LINE_FRAGMENT_SHADER =
	R"(
		#version 330 core

		in vec3 VertexColour;

		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(VertexColour, 1.0);
		}
	)";
}