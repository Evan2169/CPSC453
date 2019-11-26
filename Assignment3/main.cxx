#include <cstring>
#include <list>
#include <iostream>
#include <string>

#include "assignmentSpecific/TutorialLibraries/image.h"
#include "assignmentSpecific/TutorialLibraries/ImagePlane.h"

#include "assignmentSpecific/RayTracer.h"
#include "assignmentSpecific/Scenes.h"
#include "math/Vector.h"

namespace
{
	void parseCommandLineArguments(int ac, char** av, int* width, int* height, std::string* fileName, 
		std::list<std::shared_ptr<RayTracing::I_IntersectableShape>>* scene);

	const MathTypes::Vector<3, float> eyePosition(0, 10, 25);
	const MathTypes::Vector<3, float> lookingDirection(0, -0.4, -1);
	const MathTypes::Vector<3, float> up(0, 1, 0);
	const MathTypes::Vector<3, float> lightPosition(25, 25, 10);
	const float eyeToImagePlane = 25;
	const float planeWidth = 50;
	const float planeHeight = 50;
}

int main(int ac, char** av)
{
	int resolutionWidth, resolutionHeight;
	std::string fileName;
	std::list<std::shared_ptr<RayTracing::I_IntersectableShape>> scene;
	parseCommandLineArguments(ac, av, &resolutionWidth, &resolutionHeight, &fileName, &scene);

	auto imagePlane = RayTracing::makeImagePlane(
		eyePosition, lookingDirection, up, resolutionWidth, resolutionHeight, planeWidth, planeHeight, eyeToImagePlane);
	RayTracing::RayTracer tracer(scene);
	raster::write_screen_to_file(fileName.c_str(), tracer.renderSceneGivenParameters(eyePosition, lightPosition, imagePlane));
}

namespace
{
	void parseCommandLineArguments(int ac, char** av, int* width, int* height, std::string* fileName, 
		std::list<std::shared_ptr<RayTracing::I_IntersectableShape>>* scene)
	{
		if(ac != 4)
		{
			std::cerr << "Error in arguments. Wrong number of arguments." << std::endl;
			std::cerr << 
			R"(
			Usage: ./AssignmentThree_EvanHampton resolution scene_complexity output_file_name
				
				-resolution: "INTxINT"
				-scene_complexity: "low", "medium", or "high"
				-output_file_name: "AnythingYourHeartDesires.png"
			)"<< std::endl;

			exit(-1);
		}
		else
		{
			std::string resolution(av[1]);
			*width = std::stoi(resolution.substr(0, resolution.find("x")));
			*height = std::stoi(resolution.substr(resolution.find("x") + 1, resolution.length()));
			if(*width <= 0 || *height <= 0)
			{
			std::cerr << "Error in arguments. Resolution must be greater than zero." << std::endl;
			std::cerr << 
			R"(
			Usage: ./AssignmentThree_EvanHampton resolution scene_complexity output_file_name
				
				-resolution: "INTxINT"
				-scene_complexity: "low", "medium", or "high"
				-output_file_name: "AnythingYourHeartDesires.png"
			)"<< std::endl;

			exit(-1);
			}

			*fileName = av[3];

			if(strcmp(av[2], "low") == 0)
			{
				*scene = Scenes::simpleScene();
			}
			else if(strcmp(av[2], "medium") == 0)
			{
				*scene = Scenes::mediumComplexityScene();
			}
			else if(strcmp(av[2], "high") == 0)
			{
				*scene = Scenes::complexScene();
			}
			else
			{
			std::cerr << "Error in arguments. Unrecognized scene complexity." << std::endl;
			std::cerr << 
			R"(
			Usage: ./AssignmentThree_EvanHampton resolution scene_complexity output_file_name
				
				-resolution: "INTxINT"
				-scene_complexity: "low", "medium", or "high"
				-output_file_name: "AnythingYourHeartDesires.png"
			)"<< std::endl;

			exit(-1);
			}
		}
	}
}
