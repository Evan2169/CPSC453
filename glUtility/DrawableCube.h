#pragma once

#include <list>

#include "Cube.h"
#include "DrawableShape.h"
#include "Quadrilateral.h"
#include "Vertex.h"

namespace GLUtility
{
	template<typename CoordinatePrimitive, typename ColourPrimitive>
	class DrawableCube
	{
	public:
		DrawableCube(
			const Shapes::Cube<CoordinatePrimitive>& cube, 
			const Colour<CoordinatePrimitive>& frontColour,
			const Colour<CoordinatePrimitive>& backColour,
			const Colour<CoordinatePrimitive>& topColour,
			const Colour<CoordinatePrimitive>& bottomColour,
			const Colour<CoordinatePrimitive>& rightColour,
			const Colour<CoordinatePrimitive>& leftColour);
		~DrawableCube() = default;

		std::vector<Vertex<CoordinatePrimitive, ColourPrimitive>> vertices() const;

	private:
		using SideType = DrawableShape<3, Shapes::Quadrilateral<3, CoordinatePrimitive>,
													CoordinatePrimitive, ColourPrimitive>;
		std::list<SideType> allSides() const; 

	private:
		SideType front_;
		SideType back_;
		SideType top_;
		SideType bottom_;
		SideType right_;
		SideType left_;
	};
}

template<typename CoordinatePrimitive, typename ColourPrimitive>
GLUtility::DrawableCube<CoordinatePrimitive, ColourPrimitive>::DrawableCube(
	const Shapes::Cube<CoordinatePrimitive>& cube, 
	const Colour<CoordinatePrimitive>& frontColour,
	const Colour<CoordinatePrimitive>& backColour,
	const Colour<CoordinatePrimitive>& topColour,
	const Colour<CoordinatePrimitive>& bottomColour,
	const Colour<CoordinatePrimitive>& rightColour,
	const Colour<CoordinatePrimitive>& leftColour)
	:	front_(cube.front(), frontColour)
	,	back_(cube.back(), backColour)
	,	top_(cube.top(), topColour)
	,	bottom_(cube.bottom(), bottomColour)
	,  right_(cube.right(), rightColour)
	,	left_(cube.left(), leftColour)
{
}

template<typename CoordinatePrimitive, typename ColourPrimitive>
std::vector<GLUtility::Vertex<CoordinatePrimitive, ColourPrimitive>>
GLUtility::DrawableCube<CoordinatePrimitive, ColourPrimitive>::vertices() const
{
	std::vector<Vertex<CoordinatePrimitive, ColourPrimitive>> vertices;
	for(const auto& side : allSides())
	{
		for(const auto& vert : side.vertices())
		{
			vertices.push_back(vert);
		}
	}
	return vertices;
}

template<typename CoordinatePrimitive, typename ColourPrimitive>
std::list<GLUtility::DrawableShape<3, Shapes::Quadrilateral<3, CoordinatePrimitive>, CoordinatePrimitive, ColourPrimitive>> 
GLUtility::DrawableCube<CoordinatePrimitive, ColourPrimitive>::allSides() const
{
	return std::list<SideType>({front_, back_, top_, bottom_, right_, left_});
} 
