#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "math/Matrix.h"
#include "math/Vector.h"

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
		
		std::vector<MathTypes::Vector<dimensions, CoordinatePrimitive>> vertices() const;
		void transform(const MathTypes::Matrix<dimensions+1, dimensions+1, CoordinatePrimitive>& transformationMatrix);

		std::string toString() const;

	private:
	   MathTypes::Vector<2, CoordinatePrimitive> transformedPoint(
	   	const MathTypes::Matrix<3, 3, CoordinatePrimitive>& transformationMatrix, 
	   	const MathTypes::Vector<2, CoordinatePrimitive>& pointToTransform);

	   MathTypes::Vector<3, CoordinatePrimitive> transformedPoint(
	   	const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix, 
	   	const MathTypes::Vector<3, CoordinatePrimitive>& pointToTransform);

	private:
		MathTypes::Vector<dimensions, CoordinatePrimitive> pointOne_;
		MathTypes::Vector<dimensions, CoordinatePrimitive> pointTwo_;
		MathTypes::Vector<dimensions, CoordinatePrimitive> pointThree_;
	};
}

template<int dimensions, typename CoordinatePrimitive>
Shapes::Triangle<dimensions, CoordinatePrimitive>::Triangle(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& p1, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& p2, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& p3)
	: pointOne_(p1), pointTwo_(p2), pointThree_(p3)
{
}

template<int dimensions, typename CoordinatePrimitive>
std::vector<MathTypes::Vector<dimensions, CoordinatePrimitive>>
Shapes::Triangle<dimensions, CoordinatePrimitive>::vertices() const
{
	std::vector<MathTypes::Vector<dimensions, CoordinatePrimitive>> vertices = {pointOne_, pointTwo_, pointThree_};

	return vertices;
}

template<int dimensions, typename CoordinatePrimitive>
void Shapes::Triangle<dimensions, CoordinatePrimitive>::transform(
	const MathTypes::Matrix<dimensions+1, dimensions+1, CoordinatePrimitive>& transformationMatrix)
{
	pointOne_ = transformedPoint(transformationMatrix, pointOne_);
	pointTwo_ = transformedPoint(transformationMatrix, pointTwo_);
	pointThree_ = transformedPoint(transformationMatrix, pointThree_);
}

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive> Shapes::Triangle<dimensions, CoordinatePrimitive>::transformedPoint(
  	const MathTypes::Matrix<3, 3, CoordinatePrimitive>& transformationMatrix, 
  	const MathTypes::Vector<2, CoordinatePrimitive>& pointToTransform)
{
	MathTypes::Matrix<3, 1, CoordinatePrimitive> product = transformationMatrix * 
			static_cast<MathTypes::Matrix<3, 1, CoordinatePrimitive>>(pointToTransform.homogenized());
	return MathTypes::Vector<2, CoordinatePrimitive>(product.jthColumn(0)[0], product.jthColumn(0)[1]);
}

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> Shapes::Triangle<dimensions, CoordinatePrimitive>::transformedPoint(
  	const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix, 
  	const MathTypes::Vector<3, CoordinatePrimitive>& pointToTransform)
{
	MathTypes::Matrix<4, 1, CoordinatePrimitive> product = transformationMatrix * 
			static_cast<MathTypes::Matrix<4, 1, CoordinatePrimitive>>(pointToTransform.homogenized());
	return MathTypes::Vector<3, CoordinatePrimitive>(
		product.jthColumn(0)[0], product.jthColumn(0)[1], product.jthColumn(0)[2]);
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