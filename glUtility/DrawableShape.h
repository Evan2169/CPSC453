#pragma once

#include <algorithm>
#include "glad/glad.h"
#include <iterator>
#include <vector>

#include "Matrix.h"
#include "Vector.h"
#include "Vertex.h"

namespace GLUtility
{
	template<int dimensions, typename BaseShape, typename CoordinatePrimitive, typename ColourPrimitive>
	class DrawableShape
	{
	public:
		DrawableShape(const BaseShape& baseShape, const GLUtility::Colour<ColourPrimitive>& colour);
		~DrawableShape() = default;

		void transformUnderlyingShape(const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix);
		void renderUnderlyingTriangles() const;
		std::vector<Vertex<CoordinatePrimitive, ColourPrimitive>> vertices() const;

	private:
		BaseShape baseShape_;
		GLUtility::Colour<ColourPrimitive> colour_;
	};
}

template<int dimensions, typename BaseShape, typename CoordinatePrimitive, typename ColourPrimitive>
GLUtility::DrawableShape<dimensions, BaseShape, CoordinatePrimitive, ColourPrimitive>::DrawableShape(
	const BaseShape& baseShape, 
	const GLUtility::Colour<ColourPrimitive>& colour) 
	: baseShape_(baseShape)
	, colour_(colour)
{
}

template<int dimensions, typename BaseShape, typename CoordinatePrimitive, typename ColourPrimitive>
void GLUtility::DrawableShape<dimensions, BaseShape, CoordinatePrimitive, ColourPrimitive>::transformUnderlyingShape(
	const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformationMatrix)
{
   baseShape_.transform(transformationMatrix);
}

template<int dimensions, typename BaseShape, typename CoordinatePrimitive, typename ColourPrimitive>
void GLUtility::DrawableShape<dimensions, BaseShape, CoordinatePrimitive, ColourPrimitive>::renderUnderlyingTriangles() const
{
   glColor3f(colour_.red, colour_.green, colour_.blue);
	glBegin(GL_TRIANGLES);
    for(const auto& vertex : vertices())
    {
      glVertex3f(vertex.position.x, vertex.position.y, -1*vertex.position.z);
    }
    glEnd();
}


template<int dimensions, typename BaseShape, typename CoordinatePrimitive, typename ColourPrimitive>
std::vector<GLUtility::Vertex<CoordinatePrimitive, ColourPrimitive>> 
GLUtility::DrawableShape<dimensions, BaseShape, CoordinatePrimitive, ColourPrimitive>::vertices() const
{
	using namespace std;

	vector<GLUtility::Vertex<CoordinatePrimitive, ColourPrimitive>> verts;
	const auto coordinates = baseShape_.vertices();
	
	transform(coordinates.begin(), coordinates.end(), back_inserter(verts), 
		[=](const decltype(coordinates[0])& nakedVertex)
		{
			return Vertex<CoordinatePrimitive, ColourPrimitive>(nakedVertex, colour_);
		});

	return verts;
}
