#pragma once

#include <iterator>
#include <list>
#include <string>
#include <vector>

#include "shapes/Quadrilateral.h"
#include "math/Vector.h"

namespace Shapes
{
	template<typename CoordinatePrimitive>
	class Cube
	{
	public:
		//Constructs a cube starting at frontBottomLeft extending in the directions +x, +y, and-z
		Cube(const MathTypes::Vector<3, CoordinatePrimitive>& frontBottomLeft, CoordinatePrimitive sideLength);
		~Cube() = default;

		Quadrilateral<3, CoordinatePrimitive> front() const;
		Quadrilateral<3, CoordinatePrimitive> back() const;
		Quadrilateral<3, CoordinatePrimitive> top() const;
		Quadrilateral<3, CoordinatePrimitive> bottom() const;
		Quadrilateral<3, CoordinatePrimitive> right() const;
		Quadrilateral<3, CoordinatePrimitive> left() const;

		std::vector<MathTypes::Vector<3, CoordinatePrimitive>> vertices() const;
		std::vector<Shapes::Triangle<3, CoordinatePrimitive>> underlyingTriangles() const;
		void transform(const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix);

		std::string toString() const;

	private:
		using Vector = MathTypes::Vector<3, CoordinatePrimitive>;
		std::list<Quadrilateral<3, CoordinatePrimitive>> allSides() const;

	private:
		Quadrilateral<3, CoordinatePrimitive> front_;
		Quadrilateral<3, CoordinatePrimitive> back_;
		Quadrilateral<3, CoordinatePrimitive> top_;
		Quadrilateral<3, CoordinatePrimitive> bottom_;
		Quadrilateral<3, CoordinatePrimitive> right_;
		Quadrilateral<3, CoordinatePrimitive> left_;
	};
}

//TODO: Clean up this constructor
template<typename CoordinatePrimitive>
Shapes::Cube<CoordinatePrimitive>::Cube(const MathTypes::Vector<3, CoordinatePrimitive>& frontBottomLeft, 
	CoordinatePrimitive sideLength)

	: front_(frontBottomLeft, frontBottomLeft + Vector(sideLength, 0, 0), 
		frontBottomLeft + Vector(0, sideLength, 0), frontBottomLeft + Vector(sideLength, sideLength, 0))

	, back_(frontBottomLeft + Vector(sideLength, 0, -1*sideLength), frontBottomLeft + Vector(0, 0, -1*sideLength),  
		frontBottomLeft + Vector(sideLength, sideLength, -1*sideLength), frontBottomLeft + Vector(0, sideLength, -1*sideLength))

	, top_(frontBottomLeft + Vector(0, sideLength, 0), frontBottomLeft + Vector(sideLength, sideLength, 0), 
		frontBottomLeft + Vector(0, sideLength, -1*sideLength), frontBottomLeft + Vector(sideLength, sideLength, -1*sideLength))

	, bottom_(frontBottomLeft + Vector(0, 0, -1*sideLength), frontBottomLeft + Vector(sideLength, 0, -1*sideLength), 
		frontBottomLeft, frontBottomLeft + Vector(sideLength, 0, 0))

	, right_(frontBottomLeft + Vector(sideLength, 0, 0), frontBottomLeft + Vector(sideLength, 0, -1*sideLength), 
		frontBottomLeft + Vector(sideLength, sideLength, 0), frontBottomLeft + Vector(sideLength, sideLength, -1*sideLength))

	, left_(frontBottomLeft + Vector(0, 0, -1*sideLength), frontBottomLeft, 
		frontBottomLeft + Vector(0, sideLength, -1*sideLength), frontBottomLeft + Vector(0, sideLength, 0))
{
}

template<typename CoordinatePrimitive>
Shapes::Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::front() const
{
	return front_;
}

template<typename CoordinatePrimitive>
Shapes::Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::back() const
{
	return back_;
}

template<typename CoordinatePrimitive>
Shapes::Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::top() const
{
	return top_;
}

template<typename CoordinatePrimitive>
Shapes::Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::bottom() const
{
	return bottom_;
}

template<typename CoordinatePrimitive>
Shapes::Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::right() const
{
	return right_;
}

template<typename CoordinatePrimitive>
Shapes::Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::left() const
{
	return left_;
}

template<typename CoordinatePrimitive>
std::vector<MathTypes::Vector<3, CoordinatePrimitive>> Shapes::Cube<CoordinatePrimitive>::vertices() const
{
	std::vector<MathTypes::Vector<3, CoordinatePrimitive>> vertices;
	for(const auto& side : allSides())
	{
		for(const auto& vert : side.vertices())
		{
			vertices.push_back(vert);
		}
	}
	return vertices;
}

template<typename CoordinatePrimitive>
std::vector<Shapes::Triangle<3, CoordinatePrimitive>> Shapes::Cube<CoordinatePrimitive>::underlyingTriangles() const
{
	std::vector<Shapes::Triangle<3, CoordinatePrimitive>> triangles;
	for(const auto& side : allSides())
	{
		for(const auto& triangle : side.underlyingTriangles())
		{
			triangles.push_back(triangle);
		}
	}
	return triangles;
}

template<typename CoordinatePrimitive>
void Shapes::Cube<CoordinatePrimitive>::transform(
	const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix)
{
	front_.transform(transformationMatrix);
	back_.transform(transformationMatrix);
	top_.transform(transformationMatrix);
	bottom_.transform(transformationMatrix);
	right_.transform(transformationMatrix);
	left_.transform(transformationMatrix);
}


template<typename CoordinatePrimitive>
std::list<Shapes::Quadrilateral<3, CoordinatePrimitive>> Shapes::Cube<CoordinatePrimitive>::allSides() const
{
	return std::list<Shapes::Quadrilateral<3, CoordinatePrimitive>>({front_, back_, top_, bottom_, right_, left_}); 
}

template<typename CoordinatePrimitive>
std::string Shapes::Cube<CoordinatePrimitive>::toString() const
{
	const char* formatString = 
	R"(Cube. Faces (Quadrilaterals):
	{
		Front  : %s
		Back   : %s
		Top    : %s 
		Bottom : %s
		Right  : %s
		Left   : %s
	})";
	char buffer[15000];

	sprintf(buffer, formatString, 
		front_.toString().c_str(),
		back_.toString().c_str(),
		top_.toString().c_str(),
		bottom_.toString().c_str(),
		right_.toString().c_str(),
		left_.toString().c_str());

	return std::string(buffer);
}