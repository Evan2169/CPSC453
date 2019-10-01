#pragma once

#include <cmath>

namespace Trigonometry
{
	template<typename Primitive>
	constexpr Primitive pi();

	template<typename Primitive>
	Primitive convertAngleFromDegreesToRadians(Primitive angleInDegrees);

	template<typename Primitive>
	Primitive convertAngleFromRadiansToDegrees(Primitive angleInRadians);
}

template<typename Primitive>
constexpr Primitive Trigonometry::pi()
{
	return static_cast<Primitive>(M_PI);
}

template<typename Primitive>
Primitive Trigonometry::convertAngleFromDegreesToRadians(Primitive angleInDegrees)
{
	Primitive numberOfRadiansIn180Degrees = pi<Primitive>();
	Primitive radiansPerDegree = numberOfRadiansIn180Degrees / static_cast<Primitive>(180);

	return angleInDegrees * radiansPerDegree;
}

template<typename Primitive>
Primitive Trigonometry::convertAngleFromRadiansToDegrees(Primitive angleInRadians)
{
	Primitive numberOfDegreesInPiRadians = static_cast<Primitive>(180);
	Primitive degreesPerRadian = numberOfDegreesInPiRadians / pi<Primitive>();

	return angleInRadians * degreesPerRadian;
}