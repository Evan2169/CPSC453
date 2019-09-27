#pragma once

#include "Triangle.h"
#include "Vector.h"

namespace Shapes
{

	template<int dimensions, typename CoordinatePrimitive>
	class Quadrilateral
	{
	public:
		Quadrilateral(
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& bottomLeft,
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& bottomRight,
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& topLeft,
			const MathTypes::Vector<dimensions, CoordinatePrimitive>& topRight);
		~Quadrilateral() = default;
		
		std::array<MathTypes::Vector<dimensions, CoordinatePrimitive>, 6> vertices() const;

		std::string toString() const;

	private:
		Triangle<dimensions, CoordinatePrimitive> topLeftBottomRightBottomLeft_;
		Triangle<dimensions, CoordinatePrimitive> topLeftTopRightBottomRight_;
	};
}

template<int dimensions, typename CoordinatePrimitive>
Shapes::Quadrilateral<dimensions, CoordinatePrimitive>::Quadrilateral(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& bottomLeft, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& bottomRight,
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& topLeft,
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& topRight)
	: topLeftBottomRightBottomLeft_(topLeft, bottomRight, bottomLeft)
	, topLeftTopRightBottomRight_(topLeft, topRight, bottomRight)
{
}

template<int dimensions, typename CoordinatePrimitive>
std::array<MathTypes::Vector<dimensions, CoordinatePrimitive>, 6>
Shapes::Quadrilateral<dimensions, CoordinatePrimitive>::vertices() const
{
	auto vertsOne = topLeftBottomRightBottomLeft_.vertices();
	auto vertsTwo = topLeftTopRightBottomRight_.vertices();

	std::array<MathTypes::Vector<dimensions, CoordinatePrimitive>, 6> vertices 
	= {vertsOne[0], vertsOne[1], vertsOne[2], vertsTwo[0], vertsTwo[1], vertsTwo[2]};

	return vertices;
}


template<int dimensions, typename CoordinatePrimitive>
std::string Shapes::Quadrilateral<dimensions, CoordinatePrimitive>::toString() const
{
	const char* formatString = 
	R"(Quadrilateral. Coordinates:
	{
		Bottom Left:  %s
		Bottom Right: %s
		Top Left:     %s 
		Top Right:    %s
	})";
	char buffer[500];

	auto vertsOne = topLeftBottomRightBottomLeft_.vertices();
	auto vertsTwo = topLeftTopRightBottomRight_.vertices();

	sprintf(buffer, formatString, vertsOne[3].toString().c_str(), vertsOne[1].toString().c_str(),
	 vertsOne[0].toString().c_str(), vertsTwo[1].toString().c_str());

	return std::string(buffer);
}