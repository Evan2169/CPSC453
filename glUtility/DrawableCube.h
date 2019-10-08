#pragma once

#include <list>

#include "glUtility/DrawableShape.h"
#include "glUtility/Vertex.h"
#include "math/Matrix.h"
#include "shapes/Cube.h"
#include "shapes/Quadrilateral.h"

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

		void transformUnderlyingCube(const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix);
		void render() const;
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
void GLUtility::DrawableCube<CoordinatePrimitive, ColourPrimitive>::transformUnderlyingCube(
	const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix)
{
	front_.transformUnderlyingShape(transformationMatrix);
	back_.transformUnderlyingShape(transformationMatrix);
	top_.transformUnderlyingShape(transformationMatrix);
	bottom_.transformUnderlyingShape(transformationMatrix);
	right_.transformUnderlyingShape(transformationMatrix);
	left_.transformUnderlyingShape(transformationMatrix);
}

template<typename CoordinatePrimitive, typename ColourPrimitive>
void GLUtility::DrawableCube<CoordinatePrimitive, ColourPrimitive>::render() const
{
    for(const auto& side : allSides())
    {
      side.renderUnderlyingTriangles();
    }
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
