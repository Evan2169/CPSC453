#pragma once

#include "math/Vector.h"

namespace Shapes
{
	template<typename CoordinatePrimitive>
	class Sphere
	{
	public:
		Sphere(CoordinatePrimitive radius, const MathTypes::Vector<3, CoordinatePrimitive>& centre);
		~Sphere() = default;

		CoordinatePrimitive radius() const;
		MathTypes::Vector<3, CoordinatePrimitive> centre() const;

	private:
		const CoordinatePrimitive radius_;
		const MathTypes::Vector<3, CoordinatePrimitive> centre_;
	};
}

template<typename CoordinatePrimitive>
Shapes::Sphere<CoordinatePrimitive>::Sphere(
	const CoordinatePrimitive radius, 
	const MathTypes::Vector<3, CoordinatePrimitive>& centre)
	: radius_(radius)
	, centre_(centre)
{
}

template<typename CoordinatePrimitive>
CoordinatePrimitive Shapes::Sphere<CoordinatePrimitive>::radius() const
{
	return radius_;
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> Shapes::Sphere<CoordinatePrimitive>::centre() const
{
	return centre_;
}