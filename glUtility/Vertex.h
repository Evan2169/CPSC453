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

	template<typename CoordinatePrimitive, typename ColourPrimitive>
	struct Vertex
	{
		Vertex(Position<CoordinatePrimitive> position, Colour<ColourPrimitive> colour) 
		: position(position), colour(colour)
		{
		};

		Position<CoordinatePrimitive> position;
		Colour<ColourPrimitive> colour;
	};
}