#pragma once

#include <cmath>

#include "Matrix.h"
#include "Trigonometry.h"

namespace Matrices
{
	template<typename CoordinatePrimitive>
	MathTypes::Matrix<3, 3, CoordinatePrimitive> uniformScaleMatrix2D(CoordinatePrimitive scale);


	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> uniformScaleMatrix3D(CoordinatePrimitive scale);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<3, 3, CoordinatePrimitive> translationMatrix2D(
		CoordinatePrimitive xDelta,
		CoordinatePrimitive yDelta);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> translationMatrix3D(
		CoordinatePrimitive xDelta,
		CoordinatePrimitive yDelta,
		CoordinatePrimitive zDelta);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<3, 3, CoordinatePrimitive> rotate2D(CoordinatePrimitive angleInDegrees);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> rotateAboutX3D(CoordinatePrimitive angleInDegrees);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> rotateAboutY3D(CoordinatePrimitive angleInDegrees);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> rotateAboutZ3D(CoordinatePrimitive angleInDegrees);
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<3, 3, CoordinatePrimitive> 
Matrices::uniformScaleMatrix2D(CoordinatePrimitive scale)
{
	return MathTypes::Matrix<3, 3, CoordinatePrimitive>({
		{scale, 0,     0},
		{0,     scale, 0},
		{0,     0,     1}
	});
}

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

template<typename CoordinatePrimitive>
MathTypes::Matrix<3, 3, CoordinatePrimitive> Matrices::translationMatrix2D(
	CoordinatePrimitive xDelta,
 	CoordinatePrimitive yDelta)
{
	return MathTypes::Matrix<3, 3, CoordinatePrimitive>({
		{1, 0, xDelta},
		{0, 1, yDelta},
		{0, 0, 1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::translationMatrix3D(
	CoordinatePrimitive xDelta,
 	CoordinatePrimitive yDelta,
	CoordinatePrimitive zDelta)
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{1, 0, 0, xDelta},
		{0, 1, 0, yDelta},
		{0, 0, 1, zDelta},
		{0, 0, 0, 1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<3, 3, CoordinatePrimitive> Matrices::rotate2D(CoordinatePrimitive angleInDegrees)
{
	CoordinatePrimitive angleInRadians = Trigonometry::convertAngleFromDegreesToRadians(angleInDegrees);
	CoordinatePrimitive cosineOfAngle = static_cast<CoordinatePrimitive>(std::cos(angleInRadians));
	CoordinatePrimitive sineOfAngle = static_cast<CoordinatePrimitive>(std::sin(angleInRadians));

	return MathTypes::Matrix<3, 3, CoordinatePrimitive>({
		{cosineOfAngle, -1*sineOfAngle, 0},
		{sineOfAngle, 	 cosineOfAngle,  0},
		{0, 				 0, 				  1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::rotateAboutX3D(CoordinatePrimitive angleInDegrees)
{
	CoordinatePrimitive angleInRadians = Trigonometry::convertAngleFromDegreesToRadians(angleInDegrees);
	CoordinatePrimitive cosineOfAngle = static_cast<CoordinatePrimitive>(std::cos(angleInRadians));
	CoordinatePrimitive sineOfAngle = static_cast<CoordinatePrimitive>(std::sin(angleInRadians));

	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{1, 0, 				 0,              0},
		{0, cosineOfAngle, -1*sineOfAngle, 0},
		{0, sineOfAngle, 	 cosineOfAngle,  0},
		{0, 0, 				 0, 				  1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::rotateAboutY3D(CoordinatePrimitive angleInDegrees)
{
	CoordinatePrimitive angleInRadians = Trigonometry::convertAngleFromDegreesToRadians(angleInDegrees);
	CoordinatePrimitive cosineOfAngle = static_cast<CoordinatePrimitive>(std::cos(angleInRadians));
	CoordinatePrimitive sineOfAngle = static_cast<CoordinatePrimitive>(std::sin(angleInRadians));

	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{cosineOfAngle,  0, sineOfAngle,   0},
		{0,              1, 0,             0},
		{-1*sineOfAngle, 0, cosineOfAngle, 0},
		{0,              0, 0, 				  1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::rotateAboutZ3D(CoordinatePrimitive angleInDegrees)
{
	CoordinatePrimitive angleInRadians = Trigonometry::convertAngleFromDegreesToRadians(angleInDegrees);
	CoordinatePrimitive cosineOfAngle = static_cast<CoordinatePrimitive>(std::cos(angleInRadians));
	CoordinatePrimitive sineOfAngle = static_cast<CoordinatePrimitive>(std::sin(angleInRadians));

	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{cosineOfAngle, -1*sineOfAngle, 0, 0},
		{sineOfAngle, 	 cosineOfAngle,  0, 0},
		{0,             0, 				  1, 0},
		{0,             0,				  0, 1}
	});
}