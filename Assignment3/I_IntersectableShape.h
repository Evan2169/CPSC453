#pragma once

#include <list>
#include <optional>

#include "assignmentSpecific/Ray.h"

namespace GLUtility
{
	template<typename ColourPrimitive> class Colour;
}

namespace MathTypes
{
	template<int dimensions, typename CoordinatePrimitive> class Vector;
}

namespace RayTracing
{
	class I_IntersectableShape
	{
	public:
		~I_IntersectableShape() = default;

		virtual GLUtility::Colour<float> colourOfShape() const = 0;
		virtual std::optional<MathTypes::Vector<3, float>> surfaceNormalAtPoint(
			const MathTypes::Vector<3, float>& point) const = 0;
		virtual bool surfaceIsReflective() const = 0;

		virtual std::optional<std::list<MathTypes::Vector<3, float>>> intersectionPoints(const Ray& ray) const = 0;
		virtual std::optional<MathTypes::Vector<3, float>> closestIntersectionPoint(const Ray& ray) const = 0;
	};
}