#pragma once

#include <iterator>
#include <list>
#include <string>
#include <vector>

#include "Quadrilateral.h"
#include "Vector.h"

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
Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::front() const
{
	return front_;
}

template<typename CoordinatePrimitive>
Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::back() const
{
	return back_;
}

template<typename CoordinatePrimitive>
Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::top() const
{
	return top_;
}

template<typename CoordinatePrimitive>
Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::bottom() const
{
	return bottom_;
}

template<typename CoordinatePrimitive>
Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::right() const
{
	return right_;
}

template<typename CoordinatePrimitive>
Quadrilateral<3, CoordinatePrimitive> Shapes::Cube<CoordinatePrimitive>::left() const
{
	return left_;
}

template<typename CoordinatePrimitive>
std::vector<MathTypes::Vector<3, CoordinatePrimitive>> Shapes::Cube<CoordinatePrimitive>::vertices() const
{
	std::vector<MathTypes::Vector> vertices;
	for(const auto& side : allSides())
	{
		vertices.insert(std::begin(vertices), std::begin(side), std::end(side));
	}
	return vertices;
}

template<typename CoordinatePrimitive>
std::list<Quadrilateral<3, CoordinatePrimitive>> Shapes::Cube<CoordinatePrimitive>::allSides() const
{
	return std::list<Quadrilateral<3, CoordinatePrimitive>>({front_, back_, top_, bottom_, right_, left_}); 
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

	printf("Here bop\n");
	fflush(stdout);

	return std::string(buffer);
}