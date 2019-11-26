#pragma once

#include "assignmentSpecific/I_IntersectableShape.h"

#include <cmath>
#include <list>
#include <optional>

#include "assignmentSpecific/Ray.h"
#include "assignmentSpecific/TriangleBasedShape.h"
#include "glUtility/Vertex.h"
#include "math/LinearMath.h"
#include "math/Matrix.h"
#include "math/Vector.h"
#include "shapes/Sphere.h"

namespace RayTracing
{
	const float CALCULATION_EPSILON= 1e-5;

	template<typename UnderlyingShape>
	class IntersectableShape : public I_IntersectableShape
	{
	public:
		~IntersectableShape() = default;
	};

	template<>
	class IntersectableShape<Shapes::Sphere<float>> : public I_IntersectableShape
	{
	public:
		IntersectableShape(
			const GLUtility::Colour<float>& colour,
			bool surfaceIsReflective,
			const Shapes::Sphere<float>& underlyingSphere);

		GLUtility::Colour<float> colourOfShape() const override;
		std::optional<MathTypes::Vector<3, float>> surfaceNormalAtPoint(const MathTypes::Vector<3, float>& point) const override;
		bool surfaceIsReflective() const override;

		std::optional<std::list<MathTypes::Vector<3, float>>> intersectionPoints(const Ray& ray) const override;
		std::optional<MathTypes::Vector<3, float>> closestIntersectionPoint(const Ray& ray) const override;

	private:
		bool pointIsOnSurface(const MathTypes::Vector<3, float>& point) const;

	private:
		const GLUtility::Colour<float> colour_;
		bool surfaceIsReflective_;
		const Shapes::Sphere<float> underlyingSphere_;		
	};

	template<typename UnderlyingTriangleBasedShape>
	class IntersectableShape<RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>> : public I_IntersectableShape
	{
	public:
		IntersectableShape(
			const GLUtility::Colour<float>& colour,
			bool surfaceIsReflective,
			const RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>& underlyingShape);

		GLUtility::Colour<float> colourOfShape() const override;
		std::optional<MathTypes::Vector<3, float>> surfaceNormalAtPoint(const MathTypes::Vector<3, float>& point) const override;
		bool surfaceIsReflective() const override;

		std::optional<std::list<MathTypes::Vector<3, float>>> intersectionPoints(const Ray& ray) const override;
		std::optional<MathTypes::Vector<3, float>> closestIntersectionPoint(const Ray& ray) const override;

	private:
		const GLUtility::Colour<float> colour_;
		bool surfaceIsReflective_;
		const RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape> underlyingShape_;		
	};
}

RayTracing::IntersectableShape<Shapes::Sphere<float>>::IntersectableShape(
	const GLUtility::Colour<float>& colour,
	bool surfaceIsReflective,
	const Shapes::Sphere<float>& underlyingSphere)
	: colour_(colour)
	, surfaceIsReflective_(surfaceIsReflective)
	, underlyingSphere_(underlyingSphere)
{
}

template<typename UnderlyingTriangleBasedShape>
RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>>::IntersectableShape(
	const GLUtility::Colour<float>& colour,
	bool surfaceIsReflective,
	const RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>& underlyingShape)
	: colour_(colour)
	, surfaceIsReflective_(surfaceIsReflective)
	, underlyingShape_(underlyingShape)
{
}

std::optional<std::list<MathTypes::Vector<3, float>>> 
RayTracing::IntersectableShape<Shapes::Sphere<float>>::intersectionPoints(const Ray& ray) const
{
	//Andrew mentioned lack of precision of quadratic formula with floating point numbers in tutorial.
	//The quadratic formula code below is inspired by discussion in the following link
	//https://math.stackexchange.com/questions/311382/solving-a-quadratic-equation-with-precision-when-using-floating-point-variables

	auto centreToOrigin = ray.origin() - underlyingSphere_.centre();
	float a = LinearMath::dotProduct(ray.direction(), ray.direction());
	float b = 2 * LinearMath::dotProduct(centreToOrigin, ray.direction());
	float c = LinearMath::dotProduct(centreToOrigin, centreToOrigin) 
					- (underlyingSphere_.radius() * underlyingSphere_.radius());
	float discriminant = b*b - 4*a*c;
	discriminant = std::abs(discriminant) > CALCULATION_EPSILON ? discriminant : 0;

	if (a == 0)
	{
		return std::nullopt;
	}
	else if (discriminant > 0)
	{
		float firstRoot = b < 0 ? 
			((-b + std::sqrt(discriminant)) / 2*a) : ((-b - std::sqrt(discriminant)) / 2*a);
		float secondRoot = c/(a*firstRoot);

		std::list<MathTypes::Vector<3, float>> intersectionPoints;
		if(firstRoot > 0)
		{
			intersectionPoints.push_back(ray.pointAlongLine(firstRoot));
		}
		if(secondRoot > 0)
		{
			intersectionPoints.push_back(ray.pointAlongLine(secondRoot));
		}
		if(intersectionPoints.size() > 0)
		{
			return intersectionPoints;
		}
	}
	else if(discriminant == 0)
	{
		float distanceFromOrigin = -b / (2*a);
		if(distanceFromOrigin > 0)
		{
			return std::list<MathTypes::Vector<3, float>>({ray.pointAlongLine(-b / (2*a))});
		}
	}

	return std::nullopt;
}

std::optional<MathTypes::Vector<3, float>> 
RayTracing::IntersectableShape<Shapes::Sphere<float>>::closestIntersectionPoint(const Ray& ray) const
{
	auto intersections = intersectionPoints(ray);
	if(!intersections)
	{
		return std::nullopt;
	}
	else
	{
		intersections->sort([=](const MathTypes::Vector<3, float>& a, const MathTypes::Vector<3, float>& b)
			{
				float distanceFromAToOriginOfRay = LinearMath::distanceBetweenPoints(a, ray.origin());
				float distanceFromBToOriginOfRay = LinearMath::distanceBetweenPoints(b, ray.origin());
				if(distanceFromAToOriginOfRay < distanceFromBToOriginOfRay)
				{
					return distanceFromAToOriginOfRay;
				}
				else
				{
					return distanceFromBToOriginOfRay;
				}
			});
		return intersections->front();
	}
}

GLUtility::Colour<float> RayTracing::IntersectableShape<Shapes::Sphere<float>>::colourOfShape() const
{
	return colour_;
}

bool RayTracing::IntersectableShape<Shapes::Sphere<float>>::surfaceIsReflective() const
{
	return surfaceIsReflective_;
}

std::optional<MathTypes::Vector<3, float>> RayTracing::IntersectableShape<Shapes::Sphere<float>>::surfaceNormalAtPoint(
	const MathTypes::Vector<3, float>& point) const 
{
	if(pointIsOnSurface(point))
	{
		return (point -  underlyingSphere_.centre()).normalized();
	}
	else
	{
		return std::nullopt;
	}
}

bool RayTracing::IntersectableShape<Shapes::Sphere<float>>::pointIsOnSurface(
	const MathTypes::Vector<3, float>& point) const
{
	if(std::abs((point -  underlyingSphere_.centre()).magnitude() - underlyingSphere_.radius()) < CALCULATION_EPSILON)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename UnderlyingTriangleBasedShape>
std::optional<std::list<MathTypes::Vector<3, float>>> 
RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>>::intersectionPoints(
	const Ray& ray) const
{
	std::list<MathTypes::Vector<3, float>> intersections;
	for(const auto& triangle : underlyingShape_.underlyingTriangles())
	{
		auto ba = triangle.vertices()[0] - triangle.vertices()[1];
		auto ca = triangle.vertices()[0] - triangle.vertices()[2];
		auto oa = triangle.vertices()[0] - ray.origin();
		auto d = ray.direction();

		MathTypes::Matrix<3, 3, float> A({
			{d.xValue(), ba.xValue(), ca.xValue()},
			{d.yValue(), ba.yValue(), ca.yValue()},
			{d.zValue(), ba.zValue(), ca.zValue()}
		});
		auto B = static_cast<MathTypes::Matrix<3, 1, float>>(oa).jthColumn(0);

		float beta = LinearMath::determinant(A.withColumnReplaced(1, B)) / LinearMath::determinant(A);
		float gamma = LinearMath::determinant(A.withColumnReplaced(2, B)) / LinearMath::determinant(A);

		if(beta >= 0 && gamma >= 0 && ((beta + gamma) >= 0) && ((beta + gamma) <= 1))
		{
			float t = LinearMath::determinant(A.withColumnReplaced(0, B)) / LinearMath::determinant(A);
			if(t > 0)
			{
				intersections.push_back(ray.pointAlongLine(t));
			}
		}
	}

	if(intersections.size() == 0)
	{
		return std::nullopt;
	}
	else
	{
		return intersections;
	}
}

template<typename UnderlyingTriangleBasedShape>
std::optional<MathTypes::Vector<3, float>> 
RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>>::closestIntersectionPoint(
	const Ray& ray) const
{
	auto intersections = intersectionPoints(ray);
	if(!intersections)
	{
		return std::nullopt;
	}
	else
	{
		intersections->sort([=](const MathTypes::Vector<3, float>& a, const MathTypes::Vector<3, float>& b)
			{
				float distanceFromAToOriginOfRay = LinearMath::distanceBetweenPoints(a, ray.origin());
				float distanceFromBToOriginOfRay = LinearMath::distanceBetweenPoints(b, ray.origin());
				if(distanceFromAToOriginOfRay < distanceFromBToOriginOfRay)
				{
					return distanceFromAToOriginOfRay;
				}
				else
				{
					return distanceFromBToOriginOfRay;
				}
			});
		return intersections->front();
	}
}

template<typename UnderlyingTriangleBasedShape>
GLUtility::Colour<float>
RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>>::colourOfShape() const
{
	return colour_;
}

template<typename UnderlyingTriangleBasedShape>
bool
RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>>::surfaceIsReflective() const
{
	return surfaceIsReflective_;
}

template<typename UnderlyingTriangleBasedShape>
std::optional<MathTypes::Vector<3, float>>
RayTracing::IntersectableShape<RayTracing::TriangleBasedShape<UnderlyingTriangleBasedShape>>::surfaceNormalAtPoint(
	const MathTypes::Vector<3, float>& point) const
{
	for(const auto& triangle : underlyingShape_.underlyingTriangles())
	{
		auto a = triangle.vertices()[0];
		auto ab = triangle.vertices()[1] - triangle.vertices()[0];
		auto ac = triangle.vertices()[2] - triangle.vertices()[0];

		MathTypes::Matrix<3, 3, float> A({
			{a.xValue(), ab.xValue(), ac.xValue()},
			{a.yValue(), ab.yValue(), ac.yValue()},
			{a.zValue(), ab.zValue(), ac.zValue()}
		});
		auto B = static_cast<MathTypes::Matrix<3, 1, float>>(point).jthColumn(0);

		float beta = LinearMath::determinant(A.withColumnReplaced(1, B)) / LinearMath::determinant(A);
		float gamma = LinearMath::determinant(A.withColumnReplaced(2, B)) / LinearMath::determinant(A);

		if(beta >= 0 && gamma >= 0 && ((beta + gamma) >= 0) && ((beta + gamma) <= 1))
		{
			auto bc = triangle.vertices()[2] - triangle.vertices()[1];
			return LinearMath::crossProduct(ab, bc).normalized();
		}
	}	

	return std::nullopt;
}