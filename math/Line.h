#pragma once

#include "Vector.h"

namespace MathTypes
{
	template<int dimensions, typename CoordinatePrimitive>
	class Line
	{
	public:
		Line(
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& origin, 
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& direction);
		~Line() = default;

		MathTypes::Vector<dimensions, CoordinatePrimitive> pointAlongLine(CoordinatePrimitive distance) const;

		MathTypes::Vector<dimensions, CoordinatePrimitive> origin() const;
		MathTypes::Vector<dimensions, CoordinatePrimitive> direction() const;

	private:
		MathTypes::Vector<dimensions, CoordinatePrimitive> origin_;
		MathTypes::Vector<dimensions, CoordinatePrimitive> direction_;
	};
}

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Line<dimensions, CoordinatePrimitive>::Line(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& origin, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& direction)
	: origin_(origin)
	, direction_(direction.normalized())
{
}

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> 
MathTypes::Line<dimensions, CoordinatePrimitive>::pointAlongLine(CoordinatePrimitive distance) const
{
	return origin_ + (distance * direction_);
}

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> 
MathTypes::Line<dimensions, CoordinatePrimitive>::origin() const
{
	return origin_;
}

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> 
MathTypes::Line<dimensions, CoordinatePrimitive>::direction() const
{
	return direction_;
}
