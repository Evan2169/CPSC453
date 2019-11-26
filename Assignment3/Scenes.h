#pragma once

#include <list>
#include <memory>

#include "assignmentSpecific/IntersectableShape.h"
#include "glUtility/Vertex.h"
#include "math/Vector.h"
#include "shapes/Quadrilateral.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"

namespace Scenes
{
	std::list<std::shared_ptr<RayTracing::I_IntersectableShape>> simpleScene()
	{
		auto blueGreenSphere = new RayTracing::IntersectableShape<Shapes::Sphere<float>>(
			GLUtility::Colour<float>(0.1, 0.8, 0.3), false, Shapes::Sphere<float>(10, MathTypes::Vector<3, float>(0, -10, -15)));

		auto mirrorSurface = new RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<Shapes::Quadrilateral<3, float>>>(
			GLUtility::Colour<float>(0.7, 0.7, 0.7), true, Shapes::Quadrilateral<3, float>(
			MathTypes::Vector<3, float>(-40, -30, 40),
			MathTypes::Vector<3, float>(40, -30, 40),
			MathTypes::Vector<3, float>(-40, -30, -40),
			MathTypes::Vector<3, float>(40, -30, -40)));

		return std::list<std::shared_ptr<RayTracing::I_IntersectableShape>>({
			std::shared_ptr<RayTracing::I_IntersectableShape>(blueGreenSphere), 
			std::shared_ptr<RayTracing::I_IntersectableShape>(mirrorSurface)
		});
	}

	std::list<std::shared_ptr<RayTracing::I_IntersectableShape>> mediumComplexityScene()
	{
		auto reflectiveRedSphere = new RayTracing::IntersectableShape<Shapes::Sphere<float>>(
			GLUtility::Colour<float>(0.9, 0.1, 0.1), true, Shapes::Sphere<float>(10, MathTypes::Vector<3, float>(0, 0, 0)));

		auto reflectiveMirrorSurface = new RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<Shapes::Quadrilateral<3, float>>>(
		GLUtility::Colour<float>(0.8, 0.8, 0.8), true, Shapes::Quadrilateral<3, float>(
			MathTypes::Vector<3, float>(0, -30, -40),
			MathTypes::Vector<3, float>(40, -30, -20),
			MathTypes::Vector<3, float>(0, 30, -40),
			MathTypes::Vector<3, float>(40, 30, -20)));

		auto tealSurface = new RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<Shapes::Quadrilateral<3, float>>>(
			GLUtility::Colour<float>(0.1, .5, .5), false, Shapes::Quadrilateral<3, float>(
			MathTypes::Vector<3, float>(-40, -30, 40),
			MathTypes::Vector<3, float>(40, -30, 40),
			MathTypes::Vector<3, float>(-40, -30, -40),
			MathTypes::Vector<3, float>(40, -30, -40)));

		return std::list<std::shared_ptr<RayTracing::I_IntersectableShape>>({
			std::shared_ptr<RayTracing::I_IntersectableShape>(reflectiveRedSphere), 
			std::shared_ptr<RayTracing::I_IntersectableShape>(reflectiveMirrorSurface),
			std::shared_ptr<RayTracing::I_IntersectableShape>(tealSurface)
		});
	}

	std::list<std::shared_ptr<RayTracing::I_IntersectableShape>> complexScene()
	{
		std::list<std::shared_ptr<RayTracing::I_IntersectableShape>> objectsInScene;
		for(int i = -4; i <= 4; i++)
		{
			objectsInScene.push_back(std::shared_ptr<RayTracing::I_IntersectableShape>(
				new RayTracing::IntersectableShape<Shapes::Sphere<float>>(
						GLUtility::Colour<float>(0.7, 0.7, 0.7), true, 
						Shapes::Sphere<float>(10, MathTypes::Vector<3, float>(20 * i, -30, 0)))
				));
			objectsInScene.push_back(std::shared_ptr<RayTracing::I_IntersectableShape>(
				new RayTracing::IntersectableShape<Shapes::Sphere<float>>(
						GLUtility::Colour<float>(0.7, 0.7, 0.7), true, 
						Shapes::Sphere<float>(10, MathTypes::Vector<3, float>(20 * i, 20, -20)))
				));
		}
		objectsInScene.push_back(std::shared_ptr<RayTracing::I_IntersectableShape>(
			new RayTracing::IntersectableShape<Shapes::Sphere<float>>(
				GLUtility::Colour<float>(0.1, 0.9, 0.9), false, 
				Shapes::Sphere<float>(10, MathTypes::Vector<3, float>(0, -10, -10)))
			));
		objectsInScene.push_back(std::shared_ptr<RayTracing::I_IntersectableShape>(
			new RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<Shapes::Quadrilateral<3, float>>>(
				GLUtility::Colour<float>(0.5, 0.5, 0.5), true, Shapes::Quadrilateral<3, float>(
				MathTypes::Vector<3, float>(-80, -30, -20),
				MathTypes::Vector<3, float>(0, -30, -20),
				MathTypes::Vector<3, float>(-80, 100, -20),
				MathTypes::Vector<3, float>(0, 100, -20)))
			));
		objectsInScene.push_back(std::shared_ptr<RayTracing::I_IntersectableShape>(
			new RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<Shapes::Quadrilateral<3, float>>>(
				GLUtility::Colour<float>(0.9, 0.9, 0.5), true, Shapes::Quadrilateral<3, float>(
				MathTypes::Vector<3, float>(0, -30, -20),
				MathTypes::Vector<3, float>(80, -30, -20),
				MathTypes::Vector<3, float>(0, 100, -20),
				MathTypes::Vector<3, float>(80, 100, -20)))
			));
		
		return objectsInScene;		
	}
}