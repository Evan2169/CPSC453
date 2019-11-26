#include "assignmentSpecific/RayTracer.h"

#include <limits>

#include "assignmentSpecific/TutorialLibraries/grid2.h"
#include "assignmentSpecific/TutorialLibraries/ImagePlane.h"
#include "assignmentSpecific/TutorialLibraries/image.h"

#include "assignmentSpecific/I_IntersectableShape.h"
#include "glUtility/Vertex.h"
#include "math/LinearMath.h"
#include "math/Vector.h"

namespace
{
	const GLUtility::Colour<float> BACKGROUND_COLOUR(0.05, 0.05, 0.1);
}

RayTracing::RayTracer::RayTracer(const std::list<std::shared_ptr<I_IntersectableShape>>& objectsOfScene)
	: objectsOfScene_(objectsOfScene)
{
}

geometry::Grid2<raster::RGB> RayTracing::RayTracer::renderSceneGivenParameters(
	const MathTypes::Vector<3, float>& eyePosition,
	const MathTypes::Vector<3, float>& lightPosition,
	RayTracing::ImagePlane& imagePlane)
{
	for(int x = 0; x < imagePlane.screen.width(); x++)
	{
		for(int y = 0; y < imagePlane.screen.height(); y++)
		{
			auto pointOnImagePlane = imagePlane.pixelTo3D(x, y);
			auto rayDirection = (pointOnImagePlane - eyePosition).normalized();
			RayTracing::Ray rayFromImagePlane(pointOnImagePlane, rayDirection);

			I_IntersectableShape* closestIntersectedShape = NULL;
			auto closestIntersectionPointOfAllObjects = determineClosestIntersectionPoint(
				&closestIntersectedShape, rayFromImagePlane);

			auto outputColour = BACKGROUND_COLOUR;
			if(closestIntersectedShape != NULL)
			{
				auto intersectionToLight = (lightPosition - *closestIntersectionPointOfAllObjects).normalized();
				auto surfaceNormal = *(closestIntersectedShape->surfaceNormalAtPoint(*closestIntersectionPointOfAllObjects));
				auto intersectionToEye = (eyePosition - *closestIntersectionPointOfAllObjects).normalized();

				float totalLight = determineTotalLightAtPoint(
					*closestIntersectionPointOfAllObjects, surfaceNormal, intersectionToLight, intersectionToEye);
				outputColour = determineColourAtPoint(
					totalLight, *closestIntersectionPointOfAllObjects, surfaceNormal, intersectionToEye, closestIntersectedShape);
			}
			imagePlane.screen({x, y}) = raster::convertToRGB(outputColour);
		}
	}
	return imagePlane.screen;
}

std::optional<MathTypes::Vector<3, float>> RayTracing::RayTracer::determineClosestIntersectionPoint(
	I_IntersectableShape** closestIntersectedShape,
	const Ray& ray) const
{
	I_IntersectableShape* temporaryClosestIntersectedShape = NULL;
	MathTypes::Vector<3, float> closestIntersectionPointOfAllObjects(
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(), 
		std::numeric_limits<float>::infinity());
	for(const auto object : objectsOfScene_)
	{
		auto closestIntersectionPointOfObject = object->closestIntersectionPoint(ray);
		if(closestIntersectionPointOfObject && 
			LinearMath::distanceBetweenPointsSquared(ray.origin(), *closestIntersectionPointOfObject) 
			< LinearMath::distanceBetweenPointsSquared(ray.origin(), closestIntersectionPointOfAllObjects))
		{
			temporaryClosestIntersectedShape = object.get();
			closestIntersectionPointOfAllObjects = *closestIntersectionPointOfObject;
		}
	}

	if(temporaryClosestIntersectedShape != NULL)
	{
		*closestIntersectedShape = temporaryClosestIntersectedShape;
		return closestIntersectionPointOfAllObjects;
	}
	else
	{
		return std::nullopt;
	}
}

bool RayTracing::RayTracer::rayIntersectsAnObject(const Ray& ray) const
{
	for(const auto object : objectsOfScene_)
	{
		auto intersectionPoints = object->intersectionPoints(ray);
		if(intersectionPoints)
		{
			return true;
		}
	}
	return false;
}

float RayTracing::RayTracer::determineTotalLightAtPoint(
	const MathTypes::Vector<3, float>& point,
	const MathTypes::Vector<3, float>& surfaceNormal,
	const MathTypes::Vector<3, float>& pointToLight,
	const MathTypes::Vector<3, float>& pointToEye) const
{
	auto reflectedLight = 2 * LinearMath::dotProduct(pointToLight, surfaceNormal) 
									* (surfaceNormal - pointToLight);
	
	float ambientComponent = 0.4;
	float diffuseComponent = 0.4 * LinearMath::dotProduct(pointToLight, surfaceNormal);
	float specularComponent = LinearMath::dotProduct(reflectedLight, pointToEye);
	specularComponent = 0.2 * specularComponent * specularComponent;

	RayTracing::Ray rayToLight(point + 0.1*pointToLight, pointToLight);
	if(rayIntersectsAnObject(rayToLight))
	{
		diffuseComponent = 0;
		specularComponent = 0;
	}
	return ambientComponent + diffuseComponent + specularComponent;
}

GLUtility::Colour<float> RayTracing::RayTracer::determineColourAtPoint(
	float lightAtPoint,
	const MathTypes::Vector<3, float>& point,
	const MathTypes::Vector<3, float>& surfaceNormal,
	const MathTypes::Vector<3, float>& pointToEye,
	const I_IntersectableShape* shapeThatPointLiesOn) const
{
	auto reflectionFromEye = 2 * LinearMath::dotProduct(pointToEye, surfaceNormal) 
										* (surfaceNormal - pointToEye);

	auto colourAtPoint = shapeThatPointLiesOn->colourOfShape();
	colourAtPoint.red = colourAtPoint.red * lightAtPoint;
	colourAtPoint.green = colourAtPoint.green * lightAtPoint;
	colourAtPoint.blue = colourAtPoint.blue * lightAtPoint;

	if(shapeThatPointLiesOn->surfaceIsReflective())
	{
		RayTracing::Ray reflectionRay(point + 0.1*reflectionFromEye, reflectionFromEye);
		colourAtPoint = reflectedColourFromRay(colourAtPoint, reflectionRay, 0);
	}
	return colourAtPoint;
}

GLUtility::Colour<float> RayTracing::RayTracer::reflectedColourFromRay(
	const GLUtility::Colour<float> initialColour,
	const Ray& reflectionRay,
	int levelOfReflectionRecursion) const
{
	I_IntersectableShape* closestReflectingShape = NULL;
	auto closestReflectionPointOfAllObjects = determineClosestIntersectionPoint(&closestReflectingShape, reflectionRay);

	auto outputColour = initialColour;
	if(closestReflectingShape != NULL && levelOfReflectionRecursion < 10)
	{
		outputColour = outputColour * closestReflectingShape->colourOfShape();

		if(closestReflectingShape->surfaceIsReflective())
		{
			auto surfaceNormal = *(closestReflectingShape->surfaceNormalAtPoint(*closestReflectionPointOfAllObjects));
			auto reflectionVector = (*closestReflectionPointOfAllObjects - reflectionRay.origin()).normalized();
			auto reflectedReflectionVector = 2 * LinearMath::dotProduct(reflectionVector, surfaceNormal) 
														  * (surfaceNormal - reflectionVector);

			outputColour = reflectedColourFromRay(
				outputColour, 
				RayTracing::Ray(*closestReflectionPointOfAllObjects + 0.1*reflectedReflectionVector, reflectedReflectionVector),
				levelOfReflectionRecursion+1);
		}
	}
	return outputColour;
}