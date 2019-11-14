#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>

#include "math/LinearMath.h"
#include "math/Matrix.h"
#include "math/Trigonometry.h"
#include "math/Vector.h"

namespace MatrixUtility
{
	template<typename CoordinatePrimitive>
	std::array<CoordinatePrimitive, 16> convertToColumnMajorArray(
		const MathTypes::Matrix<4, 4, CoordinatePrimitive>& matrix); 
}

namespace Matrices
{
	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> identityMatrix3D();

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> perspectiveProjectionMatrix3D(
		CoordinatePrimitive left, 
		CoordinatePrimitive right,
		CoordinatePrimitive bottom,
		CoordinatePrimitive top, 
		CoordinatePrimitive near, 
		CoordinatePrimitive far);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> orthographicProjectionMatrix3D(
		CoordinatePrimitive left, 
		CoordinatePrimitive right,
		CoordinatePrimitive bottom,
		CoordinatePrimitive top,
		CoordinatePrimitive near, 
		CoordinatePrimitive far);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> lookAtMatrix(
		const MathTypes::Vector<3, CoordinatePrimitive>& position, 
		const MathTypes::Vector<3, CoordinatePrimitive>& target,
		const MathTypes::Vector<3, CoordinatePrimitive>& up);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<3, 3, CoordinatePrimitive> uniformScaleMatrix2D(CoordinatePrimitive scale);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> uniformScaleMatrix3D(CoordinatePrimitive scale);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> scaleXMatrix3D(CoordinatePrimitive scale);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> scaleYMatrix3D(CoordinatePrimitive scale);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> scaleZMatrix3D(CoordinatePrimitive scale);

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

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> rotateAboutLine(
		const MathTypes::Vector<3, CoordinatePrimitive>& vectorAlongLine, CoordinatePrimitive angleInDegrees);

	template<typename CoordinatePrimitive>
	MathTypes::Matrix<4, 4, CoordinatePrimitive> translateDoTransformationAndTranslateBack(
		const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformation,
		const MathTypes::Vector<3, CoordinatePrimitive>& translationVector);
}

template<typename CoordinatePrimitive>
std::array<CoordinatePrimitive, 16> MatrixUtility::convertToColumnMajorArray(
	const MathTypes::Matrix<4, 4, CoordinatePrimitive>& matrix)
{
	std::array<CoordinatePrimitive, 16> columnMajorCopyOfMatrix;
	auto transposed = matrix.transpose();
	auto insertionPoint = std::begin(columnMajorCopyOfMatrix); 
	for(int i = 0; i < 4; i++)
	{
		auto rowToCopy = transposed.ithRow(i);
		std::copy(std::begin(rowToCopy), std::end(rowToCopy), insertionPoint);
		insertionPoint += 4;
	}

	return columnMajorCopyOfMatrix;
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::identityMatrix3D()
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::perspectiveProjectionMatrix3D(CoordinatePrimitive left, CoordinatePrimitive
	right, CoordinatePrimitive bottom, CoordinatePrimitive top, CoordinatePrimitive near, CoordinatePrimitive far)
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{2*(near)/(right-left), 0,     					 (right+left)/(right-left), 0},
		{0,     					  2*(near)/(top-bottom), (top+bottom)/(top-bottom), 0},
		{0,     					  0,     					 -1*(far+near)/(far-near),  -2*far*near/(far-near)},
		{0,     					  0,     					 -1,      						 0}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::orthographicProjectionMatrix3D(
	CoordinatePrimitive left, 
	CoordinatePrimitive right,
	CoordinatePrimitive bottom,
	CoordinatePrimitive top,
	CoordinatePrimitive near, 
	CoordinatePrimitive far)
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{2/(right-left), 0,              0,            -1*(right+left)/(right-left)},
		{0,              2/(top-bottom), 0,            -1*(top+bottom)/(top-bottom)},
		{0,              0,              2/(near-far), -1*(near+far)/(near-far)},
		{0,              0,              0,            1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::lookAtMatrix(
	const MathTypes::Vector<3, CoordinatePrimitive>& position, 
	const MathTypes::Vector<3, CoordinatePrimitive>& target,
	const MathTypes::Vector<3, CoordinatePrimitive>& up) 
{
  auto forwards = (position - target).normalized();
  auto right = LinearMath::crossProduct(up, forwards).normalized();
  auto upwards = LinearMath::crossProduct(forwards, right).normalized();

  return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{right.xValue(),    right.yValue(),    right.zValue(),    -1*LinearMath::dotProduct(right, position)},
		{upwards.xValue(),  upwards.yValue(),  upwards.zValue(),  -1*LinearMath::dotProduct(upwards, position)},
		{forwards.xValue(), forwards.yValue(), forwards.zValue(), -1*LinearMath::dotProduct(forwards, position)},
		{0,                 0,                 0,                 1}
	});
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
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::scaleXMatrix3D(CoordinatePrimitive scale)
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{scale, 0,     0,     0},
		{0,     1, 		0,     0},
		{0,     0,     1, 	 0},
		{0,     0,     0,     1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::scaleYMatrix3D(CoordinatePrimitive scale)
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{1,     0,     0,     0},
		{0,     scale, 0,     0},
		{0,     0,     1,     0},
		{0,     0,     0,     1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::scaleZMatrix3D(CoordinatePrimitive scale)
{
	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{1,     0,     0,     0},
		{0,     1,     0,     0},
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

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::rotateAboutLine(
	const MathTypes::Vector<3, CoordinatePrimitive>& vectorAlongLine, CoordinatePrimitive angleInDegrees)
{
	auto normalized = vectorAlongLine.normalized();
	CoordinatePrimitive x = normalized.xValue();
	CoordinatePrimitive y = normalized.yValue();
	CoordinatePrimitive z = normalized.zValue();
	CoordinatePrimitive angleInRadians = Trigonometry::convertAngleFromDegreesToRadians(angleInDegrees);
	CoordinatePrimitive cos = static_cast<CoordinatePrimitive>(std::cos(angleInRadians));
	CoordinatePrimitive sin = static_cast<CoordinatePrimitive>(std::sin(angleInRadians));

	return MathTypes::Matrix<4, 4, CoordinatePrimitive>({
		{x*x + (1 - x*x)*cos,   x*y*(1 - cos) - z*sin, x*z*(1 - cos) + y*sin, 0},
		{x*y*(1 - cos) + z*sin, y*y + (1 - y*y)*cos,   y*z*(1 - cos) - x*sin, 0},
		{x*z*(1 - cos) - y*sin, y*z*(1 - cos) + x*sin, z*z + (1 - z*z)*cos,   0},
		{0,                     0,                     0, 				          1}
	});
}

template<typename CoordinatePrimitive>
MathTypes::Matrix<4, 4, CoordinatePrimitive> Matrices::translateDoTransformationAndTranslateBack(
	const MathTypes::Matrix<4, 4, CoordinatePrimitive>& transformation,
	const MathTypes::Vector<3, CoordinatePrimitive>& translationVector)
{
   return Matrices::translationMatrix3D<CoordinatePrimitive>(
          				-translationVector.xValue(), -translationVector.yValue(), -translationVector.zValue())
          * transformation 
			 * Matrices::translationMatrix3D<CoordinatePrimitive>(
							translationVector.xValue(), translationVector.yValue(), translationVector.zValue());

}