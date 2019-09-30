#pragma once

#include <array>
#include <cstdio>
#include <string>

#include "Vector.h"

namespace Shapes
{
	template<int dimensions, typename CoordinatePrimitive>
	class Triangle
	{
	public:
		Triangle(
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& p1, 
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& p2, 
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& p3);
		~Triangle() = default;
		
		std::array<MathTypes::Vector<dimensions, CoordinatePrimitive>, 3> vertices() const;

		std::string toString() const;

	private:
		MathTypes::Vector<dimensions, CoordinatePrimitive> pointOne_;
		MathTypes::Vector<dimensions, CoordinatePrimitive> pointTwo_;
		MathTypes::Vector<dimensions, CoordinatePrimitive> pointThree_;
	};
};

template<int dimensions, typename CoordinatePrimitive>
Shapes::Triangle<dimensions, CoordinatePrimitive>::Triangle(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& p1, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& p2, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& p3)
	: pointOne_(p1), pointTwo_(p2), pointThree_(p3)
{
}

template<int dimensions, typename CoordinatePrimitive>
std::array<MathTypes::Vector<dimensions, CoordinatePrimitive>, 3>
Shapes::Triangle<dimensions, CoordinatePrimitive>::vertices() const
{
	std::array<MathTypes::Vector<dimensions, CoordinatePrimitive>, 3> vertices = {pointOne_, pointTwo_, pointThree_};

	return vertices;
}


template<int dimensions, typename CoordinatePrimitive>
std::string Shapes::Triangle<dimensions, CoordinatePrimitive>::toString() const
{
	const char* formatString = 
	R"(Triangle. Coordinates:
	{
		Point One:   %s
		Point Two:   %s
		Point Three: %s 
	})";
	char buffer[500];

	sprintf(buffer, formatString, pointOne_.toString().c_str(), 
		pointTwo_.toString().c_str(), pointThree_.toString().c_str());

	return std::string(buffer);
}