#pragma once

namespace MathTypes
{
	template<int dimensions, typename CoordinatePrimitive> class Vector;
}

namespace GLUtility
{
	template<typename ColourPrimitive>
	struct Colour
	{
		Colour() = default;
		Colour(ColourPrimitive red, ColourPrimitive green, ColourPrimitive blue) : red(red), green(green), blue(blue)
		{
		};

		ColourPrimitive red;
		ColourPrimitive green;
		ColourPrimitive blue;
	};

	template<typename CoordinatePrimitive>
	struct Position
	{
		Position() = default;
		Position(MathTypes::Vector<2, CoordinatePrimitive> vec) : x(vec.xValue()), y(vec.yValue())
		{
		};
		Position(MathTypes::Vector<3, CoordinatePrimitive> vec) : x(vec.xValue()), y(vec.yValue()), z(vec.zValue())
		{
		};

		CoordinatePrimitive x;
		CoordinatePrimitive y;
		CoordinatePrimitive z;
	};

	template<typename CoordinatePrimitive>
	struct Normal
	{
		Normal() = default;
		Normal(MathTypes::Vector<2, CoordinatePrimitive> vec) : x(vec.xValue()), y(vec.yValue())
		{
		};
		Normal(MathTypes::Vector<3, CoordinatePrimitive> vec) : x(vec.xValue()), y(vec.yValue()), z(vec.zValue())
		{
		};

		CoordinatePrimitive x;
		CoordinatePrimitive y;
		CoordinatePrimitive z;
	};

	template<typename CoordinatePrimitive, typename ColourPrimitive>
	struct Vertex
	{
		Vertex(Position<CoordinatePrimitive> position, Colour<ColourPrimitive> colour) 
		: position(position), colour(colour)
		{
		};
		Vertex(Position<CoordinatePrimitive> position, Colour<ColourPrimitive> colour, Normal<CoordinatePrimitive> normal) 
		: position(position), colour(colour), normal(normal)
		{
		};

		Position<CoordinatePrimitive> position;
		Colour<ColourPrimitive> colour;
		Normal<CoordinatePrimitive> normal;
	};
}

template<typename ColourPrimitive>
GLUtility::Colour<ColourPrimitive> operator*(
	const GLUtility::Colour<ColourPrimitive>& lhs,
	const GLUtility::Colour<ColourPrimitive>& rhs)
{
	return GLUtility::Colour<ColourPrimitive>(
		lhs.red * rhs.red,
		lhs.green * rhs.green,
		lhs.blue * rhs.blue);
}