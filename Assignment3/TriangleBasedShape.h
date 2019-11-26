#pragma once

#include <vector>

#include "shapes/Triangle.h"

namespace RayTracing
{
	template<typename UnderlyingShape>
	class TriangleBasedShape
	{
	public:
		TriangleBasedShape(const UnderlyingShape& shape);
		~TriangleBasedShape() = default;

		std::vector<Shapes::Triangle<3, float>> underlyingTriangles() const;

	private:
		const UnderlyingShape shape_;
	};
}

template<typename UnderlyingShape>
RayTracing::TriangleBasedShape<UnderlyingShape>::TriangleBasedShape(const UnderlyingShape& shape)
: shape_(shape)
{
}

template<typename UnderlyingShape>
std::vector<Shapes::Triangle<3, float>> RayTracing::TriangleBasedShape<UnderlyingShape>::underlyingTriangles() const
{
	return shape_.underlyingTriangles();
}