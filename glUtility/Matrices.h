#pragma once

#include "Matrix.h"

namespace Matrices
{
	template<typename CoordinatePrimitive>
	MathTypes::Matrix<3, 3, CoordinatePrimitive> uniformScaleMatrix2D(CoordinatePrimitive scale);


	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> uniformScaleMatrix3D(CoordinatePrimitive scale);
}

// template<2, typename CoordinatePrimitive>
// MathTypes::Matrix<3, 3, CoordinatePrimitive> 
// Matrices::uniformScaleMatrix(CoordinatePrimitive scale)
// {
// 	return MathTypes::Matrix<3, 3, CoordinatePrimitive>({
// 		{scale, 0,     0},
// 		{0,     scale, 0},
// 		{0,     0,     1}
// 	});
// }

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> 
Matrices::uniformScaleMatrix3D(CoordinatePrimitive scale)
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{scale, 0,     0,     0},
		{0,     scale, 0,     0},
		{0,     0,     scale, 0},
		{0,     0,     0,     1}
	});
}
