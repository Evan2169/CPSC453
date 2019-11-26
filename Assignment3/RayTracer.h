#pragma once

#include <list>
#include <memory>
#include <optional>

#include "Ray.h"

namespace RayTracing
{
	class I_IntersectableShape;
	class ImagePlane;
}

namespace geometry
{
	template<typename T> class Grid2;
}

namespace raster
{
	struct RGB;
}

namespace MathTypes
{
	template<int dimensions, typename CoordinatePrimitive> class Vector;
}

namespace GLUtility
{
	template<typename ColourPrimitive> class Colour;
}

namespace RayTracing
{
	class RayTracer
	{
		public:
			RayTracer(const std::list<std::shared_ptr<I_IntersectableShape>>& objectsOfScene);
			~RayTracer() = default;

			geometry::Grid2<raster::RGB> renderSceneGivenParameters(
				const MathTypes::Vector<3, float>& eyePosition,
				const MathTypes::Vector<3, float>& lightPosition,
				RayTracing::ImagePlane& imagePlane);

		private:
			std::optional<MathTypes::Vector<3, float>> determineClosestIntersectionPoint(
				I_IntersectableShape** closestIntersectedShape,
				const Ray& ray) const;
			bool rayIntersectsAnObject(const Ray& ray) const;
			float determineTotalLightAtPoint(
				const MathTypes::Vector<3, float>& point,
				const MathTypes::Vector<3, float>& surfaceNormal,
				const MathTypes::Vector<3, float>& pointToLight,
				const MathTypes::Vector<3, float>& pointToEye) const;
			GLUtility::Colour<float> determineColourAtPoint(
				float lightAtPoint,
				const MathTypes::Vector<3, float>& point,
				const MathTypes::Vector<3, float>& surfaceNormal,
				const MathTypes::Vector<3, float>& pointToEye,
				const I_IntersectableShape* shapeThatPointLiesOn) const;
			GLUtility::Colour<float> reflectedColourFromRay(
				const GLUtility::Colour<float> initialColour,
				const Ray& reflectionRay,
				int levelOfReflectionRecursion) const;

		private:
			std::list<std::shared_ptr<I_IntersectableShape>> objectsOfScene_;
	};
}