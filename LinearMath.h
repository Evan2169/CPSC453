#pragma once

#include "Vector.h"

namespace LinearMath
{
	template<int dimensions, typename CoordinatePrimitive>
	CoordinatePrimitive dotProduct(const MathTypes::Vector<dimensions, CoordinatePrimitive>& lhs, 
		const MathTypes::Vector<dimensions, CoordinatePrimitive>& rhs);
	
	template<typename CoordinatePrimitive>
	MathTypes::Vector<3, CoordinatePrimitive> crossProduct(
		const MathTypes::Vector<3, CoordinatePrimitive>& lhs, 
		const MathTypes::Vector<3, CoordinatePrimitive>& rhs);

	template<int dimensions, typename CoordinatePrimitive>
	MathTypes::Vector<dimensions, CoordinatePrimitive> linearInterpolation(
		const MathTypes::Vector<dimensions, CoordinatePrimitive>& lineStart,
		const MathTypes::Vector<dimensions, CoordinatePrimitive>& lineEnd,		
		double fractionAlongLineFromStart);

	template<int dimensions, typename CoordinatePrimitive>
	CoordinatePrimitive distanceBetweenPoints(const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointA,
		const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointB);

	template<int dimensions, typename CoordinatePrimitive>
	CoordinatePrimitive distanceBetweenPointsSquared(const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointA,
		const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointB);
};

template<typename CoordinatePrimitive>
CoordinatePrimitive LinearMath::dotProduct(const MathTypes::Vector<2, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<2, CoordinatePrimitive>& rhs)
{
	const CoordinatePrimitive xProduct = lhs.xValue() * rhs.xValue();
	const CoordinatePrimitive yProduct = lhs.yValue() * rhs.yValue();

	return xProduct + yProduct;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive LinearMath::dotProduct(const MathTypes::Vector<3, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<3, CoordinatePrimitive>& rhs)
{
	const CoordinatePrimitive xProduct = lhs.xValue() * rhs.xValue();
	const CoordinatePrimitive yProduct = lhs.yValue() * rhs.yValue();
	const CoordinatePrimitive zProduct = lhs.zValue() * rhs.zValue();

	return xProduct + yProduct + zProduct;
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> LinearMath::crossProduct(
	const MathTypes::Vector<3, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<3, CoordinatePrimitive>& rhs)
{
	const CoordinatePrimitive iScalar = (lhs.yValue() * rhs.zValue()) - (lhs.zValue() * rhs.yValue());
	const CoordinatePrimitive jScalar = (lhs.xValue() * rhs.zValue()) - (lhs.zValue() * rhs.xValue());
	const CoordinatePrimitive kScalar = (lhs.xValue() * rhs.yValue()) - (lhs.yValue() * rhs.xValue());

	return MathTypes::Vector<3, CoordinatePrimitive>(iScalar, -1*jScalar, kScalar);
}

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> LinearMath::linearInterpolation(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& lineStart,
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& lineEnd,		
	double fractionAlongLineFromStart)
{
	const auto lineSegment = lineEnd - lineStart; 
   const auto pointOnLine = lineSegment * fractionAlongLineFromStart;
	return lineStart + pointOnLine;
}

template<int dimensions, typename CoordinatePrimitive>
CoordinatePrimitive LinearMath::distanceBetweenPoints(const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointA,
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointB)
{
	return (pointB - pointA).magnitude();
}

template<int dimensions, typename CoordinatePrimitive>
CoordinatePrimitive LinearMath::distanceBetweenPointsSquared(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointA,
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& pointB)
{
	return (pointB - pointA).magnitudeSquared();
}