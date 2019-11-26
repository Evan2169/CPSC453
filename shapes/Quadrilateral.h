#pragma once

#include <vector>

#include "math/Vector.h"
#include "shapes/Triangle.h"

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
		
		std::vector<MathTypes::Vector<dimensions, CoordinatePrimitive>> vertices() const;
		std::vector<Shapes::Triangle<dimensions, CoordinatePrimitive>> underlyingTriangles() const;
		void transform(const MathTypes::Matrix<dimensions+1, dimensions+1, CoordinatePrimitive>& transformationMatrix);

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
std::vector<MathTypes::Vector<dimensions, CoordinatePrimitive>>
Shapes::Quadrilateral<dimensions, CoordinatePrimitive>::vertices() const
{
	auto vertsOne = topLeftBottomRightBottomLeft_.vertices();
	auto vertsTwo = topLeftTopRightBottomRight_.vertices();

	std::vector<MathTypes::Vector<dimensions, CoordinatePrimitive>> 
	vertices({vertsOne[0], vertsOne[1], vertsOne[2], vertsTwo[0], vertsTwo[1], vertsTwo[2]});

	return vertices;
}

template<int dimensions, typename CoordinatePrimitive>
std::vector<Shapes::Triangle<dimensions, CoordinatePrimitive>>
Shapes::Quadrilateral<dimensions, CoordinatePrimitive>::underlyingTriangles() const
{
	return std::vector({topLeftTopRightBottomRight_, topLeftBottomRightBottomLeft_});
}


template<int dimensions, typename CoordinatePrimitive>
void Shapes::Quadrilateral<dimensions, CoordinatePrimitive>::transform(
	const MathTypes::Matrix<dimensions+1, dimensions+1, CoordinatePrimitive>& transformationMatrix)
{
	topLeftBottomRightBottomLeft_.transform(transformationMatrix);
	topLeftTopRightBottomRight_.transform(transformationMatrix);
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
	char buffer[2000];

	auto vertsOne = topLeftBottomRightBottomLeft_.vertices();
	auto vertsTwo = topLeftTopRightBottomRight_.vertices();

	sprintf(buffer, formatString, vertsOne[2].toString().c_str(), vertsOne[1].toString().c_str(),
	 vertsOne[0].toString().c_str(), vertsTwo[1].toString().c_str());

	return std::string(buffer);
}