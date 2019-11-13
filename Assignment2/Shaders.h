#pragma once

namespace Shaders
{
	static const char* PHONG_VERTEX_SHADER_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 1) in vec2 vertexTexture;
		layout (location = 2) in vec3 vertexNormal;
		
		out vec3 VertexPosition;
		out vec2 VertexTexture;
		out vec3 VertexNormal;

		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			VertexPosition = (Model * vec4(vertex, 1)).xyz;
			VertexTexture = vertexTexture;
			VertexNormal = normalize(Model * vec4(vertexNormal, 1)).xyz;
		}
	)";

	static const char* PHONG_FRAGMENT_SHADER_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		in vec3 VertexPosition;
		in vec2 VertexTexture;
		in vec3 VertexNormal;

		uniform sampler2D ImageTexture;
		uniform vec3 LightPosition;
		uniform vec3 CameraPosition;

		out vec4 FragColor;

		void main()
		{
			vec3 colourFromTexture = texture(ImageTexture, VertexTexture).rgb;

			vec3 lightColour = vec3(1.0, 1.0, 1.0);
			vec3 lightDirection = normalize(LightPosition - VertexPosition);
			vec3 cameraDirection = normalize(CameraPosition - VertexPosition);
			
			float ambient = 0.2;
			float diffuse = abs(dot(lightDirection, VertexNormal));
			float specular = pow(max(dot(reflect(-lightDirection, normalize(VertexNormal)), cameraDirection), 0.0), 32) * 0.5;

			FragColor = vec4(clamp(colourFromTexture * (ambient + diffuse + specular), 0.0, 1.0), 1.0);
		}
	)";

	static const char* GOURAUD_VERTEX_SHADER_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 1) in vec2 vertexTexture;
		layout (location = 2) in vec3 vertexNormal;
		
		out vec2 VertexTexture;		
		out vec3 LightColour;

		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		uniform vec3 LightPosition;
		uniform vec3 CameraPosition;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			vec3 position = (Model * vec4(vertex, 1)).xyz;
			vec3 normal = normalize(Model * vec4(vertexNormal, 1)).xyz;
			vec3 lightColour = vec3(1.0, 1.0, 1.0);
			vec3 lightDirection = normalize(LightPosition - position);
			vec3 cameraDirection = normalize(CameraPosition - position);

			float ambient = 0.2;
			float diffuse = abs(dot(lightDirection, normal));
			float specular = pow(max(dot(reflect(-lightDirection, normal), cameraDirection), 0.0), 32) * 0.5;

			VertexTexture = vertexTexture;
			LightColour = clamp(lightColour * (ambient + diffuse + specular), 0.0, 1.0);
		}
	)";

	static const char* GOURAUD_FRAGMENT_SHADER_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		in vec2 VertexTexture;
		in vec3 LightColour;

		uniform sampler2D ImageTexture;

		out vec4 FragColor;

		void main()
		{
			vec3 colourFromTexture = texture(ImageTexture, VertexTexture).rgb;

			FragColor = vec4(colourFromTexture * LightColour, 1.0);
		}
	)";

	static const char* FLAT_VERTEX_SHADER_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 1) in vec2 vertexTexture;
		layout (location = 2) in vec3 vertexNormal;
		
		out Data
		{
			vec3 VertexPosition;
		 	vec2 VertexTexture;
		 	vec3 VertexNormal;
		} data;

		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			data.VertexPosition = (Model * vec4(vertex, 1)).xyz;
			data.VertexTexture = vertexTexture;
			data.VertexNormal = normalize(Model * vec4(vertexNormal, 1)).xyz;
		}
	)";

	static const char* FLAT_GEOMETRY_SHADER_TEXTURE_SUPPORT =
	R"(
		#version 330 core
		layout (triangles) in;
		layout (triangle_strip, max_vertices = 3) out;

		in Data
		{
			vec3 VertexPosition;
		 	vec2 VertexTexture;
		 	vec3 VertexNormal;
		} dataIn[];

		out Data
		{
			vec3 VertexPosition;
		 	vec2 VertexTexture;
		 	vec3 VertexNormal;
		} dataOut;


		void main()
		{
			vec3 ab = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
			vec3 bc = (gl_in[2].gl_Position - gl_in[1].gl_Position).xyz;

			vec3 normal = normalize(cross(ab, bc));

			gl_Position = gl_in[0].gl_Position;
			dataOut.VertexPosition = dataIn[0].VertexPosition;
			dataOut.VertexTexture = dataIn[0].VertexTexture;
			dataOut.VertexNormal = normal;
			EmitVertex();
	
			gl_Position = gl_in[1].gl_Position;
			dataOut.VertexPosition = dataIn[1].VertexPosition;
			dataOut.VertexTexture = dataIn[1].VertexTexture;
			dataOut.VertexNormal = normal;
			EmitVertex();

			gl_Position = gl_in[2].gl_Position;
			dataOut.VertexPosition = dataIn[2].VertexPosition;
			dataOut.VertexTexture = dataIn[2].VertexTexture;
			dataOut.VertexNormal = normal;
			EmitVertex();

			EndPrimitive();
		}
	)";

	static const char* FLAT_FRAGMENT_SHADER_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		in Data
		{
			vec3 VertexPosition;
		 	vec2 VertexTexture;
		 	vec3 VertexNormal;
		} dataIn;

		uniform vec3 LightPosition;
		uniform vec3 CameraPosition;
		uniform sampler2D ImageTexture;

		out vec4 FragColor;

		void main()
		{
			vec3 colourFromTexture = texture(ImageTexture, dataIn.VertexTexture).rgb;

			vec3 lightColour = vec3(1.0, 1.0, 1.0);
			vec3 lightDirection = normalize(LightPosition - dataIn.VertexPosition);
			vec3 cameraDirection = normalize(CameraPosition - dataIn.VertexPosition);
			
			float ambient = 0.2;
			float diffuse = abs(dot(lightDirection, normalize(dataIn.VertexNormal)));
			float specular = pow(max(dot(reflect(-lightDirection, normalize(dataIn.VertexNormal)), 
										cameraDirection), 0.0), 32) * 0.5;

			FragColor = vec4(colourFromTexture * lightColour * (ambient + diffuse + specular), 1.0);
		}
	)";

	static const char* PHONG_VERTEX_SHADER_NO_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 2) in vec3 vertexNormal;
		
		out vec3 VertexPosition;
		out vec3 VertexNormal;

		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			VertexPosition = (Model * vec4(vertex, 1)).xyz;
			VertexNormal = normalize(Model * vec4(vertexNormal, 1)).xyz;
		}
	)";

	static const char* PHONG_FRAGMENT_SHADER_NO_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		in vec3 VertexPosition;
		in vec3 VertexNormal;

		uniform sampler2D ImageTexture;
		uniform vec3 LightPosition;
		uniform vec3 CameraPosition;

		out vec4 FragColor;

		void main()
		{
			vec3 lightColour = vec3(1.0, 1.0, 1.0);
			vec3 lightDirection = normalize(LightPosition - VertexPosition);
			vec3 cameraDirection = normalize(CameraPosition - VertexPosition);
			
			float ambient = 0.2;
			float diffuse = abs(dot(lightDirection, VertexNormal));
			float specular = pow(max(dot(reflect(-lightDirection, normalize(VertexNormal)), cameraDirection), 0.0), 32) * 0.5;

			FragColor = vec4(clamp(vec3(0.0, 0.2, 0.7) * (ambient + diffuse + specular), 0.0, 1.0), 1.0);
		}
	)";

	static const char* GOURAUD_VERTEX_SHADER_NO_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 2) in vec3 vertexNormal;
		
		out vec3 LightColour;

		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		uniform vec3 LightPosition;
		uniform vec3 CameraPosition;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			vec3 position = (Model * vec4(vertex, 1)).xyz;
			vec3 normal = normalize(Model * vec4(vertexNormal, 1)).xyz;
			vec3 lightColour = vec3(1.0, 1.0, 1.0);
			vec3 lightDirection = normalize(LightPosition - position);
			vec3 cameraDirection = normalize(CameraPosition - position);

			float ambient = 0.2;
			float diffuse = abs(dot(lightDirection, normal));
			float specular = pow(max(dot(reflect(-lightDirection, normal), cameraDirection), 0.0), 32) * 0.5;

			LightColour = clamp(lightColour * (ambient + diffuse + specular), 0.0, 1.0);
		}
	)";

	static const char* GOURAUD_FRAGMENT_SHADER_NO_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		in vec3 LightColour;

		uniform sampler2D ImageTexture;

		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(vec3(0.0, 0.2, 0.7) * LightColour, 1.0);
		}
	)";

	static const char* FLAT_VERTEX_SHADER_NO_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		layout (location = 0) in vec3 vertex;
		layout (location = 2) in vec3 vertexNormal;
		
		out Data
		{
			vec3 VertexPosition;
		 	vec3 VertexNormal;
		} data;

		uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;

		void main()
		{
			gl_Position = Projection * View * Model * vec4(vertex, 1);

			data.VertexPosition = (Model * vec4(vertex, 1)).xyz;
			data.VertexNormal = normalize(Model * vec4(vertexNormal, 1)).xyz;
		}
	)";

	static const char* FLAT_GEOMETRY_SHADER_NO_TEXTURE_SUPPORT =
	R"(
		#version 330 core
		layout (triangles) in;
		layout (triangle_strip, max_vertices = 3) out;

		in Data
		{
			vec3 VertexPosition;
		 	vec3 VertexNormal;
		} dataIn[];

		out Data
		{
			vec3 VertexPosition;
		 	vec3 VertexNormal;
		} dataOut;


		void main()
		{
			vec3 ab = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
			vec3 bc = (gl_in[2].gl_Position - gl_in[1].gl_Position).xyz;

			vec3 normal = normalize(cross(ab, bc));

			gl_Position = gl_in[0].gl_Position;
			dataOut.VertexPosition = dataIn[0].VertexPosition;
			dataOut.VertexNormal = normal;
			EmitVertex();
	
			gl_Position = gl_in[1].gl_Position;
			dataOut.VertexPosition = dataIn[1].VertexPosition;
			dataOut.VertexNormal = normal;
			EmitVertex();

			gl_Position = gl_in[2].gl_Position;
			dataOut.VertexPosition = dataIn[2].VertexPosition;
			dataOut.VertexNormal = normal;
			EmitVertex();

			EndPrimitive();
		}
	)";

	static const char* FLAT_FRAGMENT_SHADER_NO_TEXTURE_SUPPORT =
	R"(
		#version 330 core

		in Data
		{
			vec3 VertexPosition;
		 	vec3 VertexNormal;
		} dataIn;

		uniform vec3 LightPosition;
		uniform vec3 CameraPosition;
		uniform sampler2D ImageTexture;

		out vec4 FragColor;

		void main()
		{
			vec3 lightColour = vec3(1.0, 1.0, 1.0);
			vec3 lightDirection = normalize(LightPosition - dataIn.VertexPosition);
			vec3 cameraDirection = normalize(CameraPosition - dataIn.VertexPosition);
			
			float ambient = 0.2;
			float diffuse = abs(dot(lightDirection, normalize(dataIn.VertexNormal)));
			float specular = pow(max(dot(reflect(-lightDirection, normalize(dataIn.VertexNormal)), 
											cameraDirection), 0.0), 32) * 0.5;

			FragColor = vec4(vec3(0.0, 0.2, 0.7) * lightColour * (ambient + diffuse + specular), 1.0);
		}
	)";
}