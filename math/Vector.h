#pragma once

#include <cstdio>
#include <cmath>
#include <string>
#include <typeinfo>

#include "Matrix.h"

namespace MathTypes
{
	template<int dimensions, typename CoordinatePrimitive>
	class Vector
	{
	public:
		virtual ~Vector() = default;
	};
	
	//2D Template Specialization of Vector Class
	template<typename CoordinatePrimitive>
	class Vector<2, CoordinatePrimitive>
	{
	public:
		Vector(CoordinatePrimitive x, CoordinatePrimitive y);
		explicit Vector(std::array<CoordinatePrimitive, 2> data);
	
		CoordinatePrimitive xValue() const;
		CoordinatePrimitive yValue() const;

		CoordinatePrimitive magnitude() const;
		CoordinatePrimitive magnitudeSquared() const;
		Vector<2, CoordinatePrimitive> normalized() const;
		Vector<3, CoordinatePrimitive> homogenized() const;
		explicit operator Matrix<2, 1, CoordinatePrimitive>() const;

		std::string toString() const;
	
	private:
		CoordinatePrimitive x_;
		CoordinatePrimitive y_;
	};
	
	//3D Template Specialization of Vector Class
	template<typename CoordinatePrimitive>
	class Vector<3, CoordinatePrimitive>
	{
	public:
		Vector(CoordinatePrimitive x, CoordinatePrimitive y, CoordinatePrimitive z);
		explicit Vector(std::array<CoordinatePrimitive, 3> data);
	
		CoordinatePrimitive xValue() const;
		CoordinatePrimitive yValue() const;
		CoordinatePrimitive zValue() const;

		// CoordinatePrimitive magnitude() const;
		CoordinatePrimitive magnitude() const;
		CoordinatePrimitive magnitudeSquared() const;
		Vector<3, CoordinatePrimitive> normalized() const;
		Vector<4, CoordinatePrimitive> homogenized() const;
		explicit operator Matrix<3, 1, CoordinatePrimitive>() const;
	
		std::string toString() const;
	
	private:
		CoordinatePrimitive x_;
		CoordinatePrimitive y_;
		CoordinatePrimitive z_;
	};

	//4D Template Specialization of Vector Class
	template<typename CoordinatePrimitive>
	class Vector<4, CoordinatePrimitive>
	{
	public:
		Vector(CoordinatePrimitive x, CoordinatePrimitive y, CoordinatePrimitive z, CoordinatePrimitive w);
		explicit Vector(std::array<CoordinatePrimitive, 4> data);
	
		CoordinatePrimitive xValue() const;
		CoordinatePrimitive yValue() const;
		CoordinatePrimitive zValue() const;
		CoordinatePrimitive wValue() const;
	
		explicit operator Matrix<4, 1, CoordinatePrimitive>() const;

		std::string toString() const;
	
	private:
		CoordinatePrimitive x_;
		CoordinatePrimitive y_;
		CoordinatePrimitive z_;
		CoordinatePrimitive w_;
	};
};
	
template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> operator+(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& rhs);

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> operator-(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& lhs,
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& rhs);

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> operator*(
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& lhs, 
	double scalar);

template<int dimensions, typename CoordinatePrimitive>
MathTypes::Vector<dimensions, CoordinatePrimitive> operator*(
	double scalar, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& rhs);

template<int dimensions, typename CoordinatePrimitive>
bool operator==(const MathTypes::Vector<dimensions, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<dimensions, CoordinatePrimitive>& rhs);

template <typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive>::Vector(CoordinatePrimitive x, CoordinatePrimitive y)
	: x_(x), y_(y)
{
}

template <typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive>::Vector(std::array<CoordinatePrimitive, 2> data)
	: x_(data[0]), y_(data[1])
{
}


template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<2, CoordinatePrimitive>::xValue() const
{
	return x_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<2, CoordinatePrimitive>::yValue() const
{
	return y_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<2, CoordinatePrimitive>::magnitude() const
{
	return std::sqrt(magnitudeSquared());
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<2, CoordinatePrimitive>::magnitudeSquared() const
{
	return (x_ * x_) + (y_ * y_);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive> MathTypes::Vector<2, CoordinatePrimitive>::normalized() const
{
	const CoordinatePrimitive normalizingScalar = 1.0 / this->magnitude();
	return *this * normalizingScalar;
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> MathTypes::Vector<2, CoordinatePrimitive>::homogenized() const
{
	return Vector<3, CoordinatePrimitive>(x_, y_, 1);
}

template <typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive> operator+(const MathTypes::Vector<2, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<2, CoordinatePrimitive>& rhs)
{
	const CoordinatePrimitive xSum = lhs.xValue() + rhs.xValue();
	const CoordinatePrimitive ySum = lhs.yValue() + rhs.yValue();

	return MathTypes::Vector<2, CoordinatePrimitive>(xSum, ySum);
}

template <typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive> operator-(const MathTypes::Vector<2, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<2, CoordinatePrimitive>& rhs)
{
	const CoordinatePrimitive xDifference = lhs.xValue() - rhs.xValue();
	const CoordinatePrimitive yDifference = lhs.yValue() - rhs.yValue();

	return MathTypes::Vector<2, CoordinatePrimitive>(xDifference, yDifference);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive> operator*(const MathTypes::Vector<2, CoordinatePrimitive>& lhs, double scalar)
{
	const CoordinatePrimitive xScaled = lhs.xValue() * scalar;
	const CoordinatePrimitive yScaled = lhs.yValue() * scalar;

	return MathTypes::Vector<2, CoordinatePrimitive>(xScaled, yScaled);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive> operator*(double scalar, const MathTypes::Vector<2, CoordinatePrimitive>& rhs)
{
	return rhs * scalar;
}

template<typename CoordinatePrimitive>
bool operator==(const MathTypes::Vector<2, CoordinatePrimitive>& lhs, const MathTypes::Vector<2, CoordinatePrimitive>& rhs)
{
	return (lhs.xValue() == rhs.xValue()) && (lhs.yValue() == rhs.yValue());
}

template<typename CoordinatePrimitive>
MathTypes::Vector<2, CoordinatePrimitive>::operator Matrix<2, 1, CoordinatePrimitive>() const
{
	auto columnVector = {
		{x_}, 
		{y_}
	};

	return MathTypes::Matrix<2, 1, CoordinatePrimitive>(columnVector);
}

template <typename CoordinatePrimitive>
std::string MathTypes::Vector<2, CoordinatePrimitive>::toString() const
{
	const char* formatString = "Two Dimensional Vector. (%.3f, %.3f). Coordinate Type: %s";
	char buffer[100];

	sprintf(buffer, formatString, x_, y_, typeid(CoordinatePrimitive).name());

	return std::string(buffer);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive>::Vector(CoordinatePrimitive x, CoordinatePrimitive y, 
	CoordinatePrimitive z) : x_(x), y_(y), z_(z)
{
}

template <typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive>::Vector(std::array<CoordinatePrimitive, 3> data)
	: x_(data[0]), y_(data[1]), z_(data[2])
{
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<3, CoordinatePrimitive>::xValue() const
{
	return x_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<3, CoordinatePrimitive>::yValue() const
{
	return y_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<3, CoordinatePrimitive>::zValue() const
{
	return z_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<3, CoordinatePrimitive>::magnitude() const
{
	return std::sqrt(magnitudeSquared());
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<3, CoordinatePrimitive>::magnitudeSquared() const
{
	return (x_ * x_) + (y_ * y_) + (z_ * z_);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> MathTypes::Vector<3, CoordinatePrimitive>::normalized() const
{
	const CoordinatePrimitive normalizingScalar = 1.0 / this->magnitude();
	return *this * normalizingScalar;
}

template<typename CoordinatePrimitive>
MathTypes::Vector<4, CoordinatePrimitive> MathTypes::Vector<3, CoordinatePrimitive>::homogenized() const
{
	return Vector<4, CoordinatePrimitive>(x_, y_, z_, 1);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> operator+(const MathTypes::Vector<3, CoordinatePrimitive>& lhs,
	const MathTypes::Vector<3, CoordinatePrimitive>& rhs)
{
	const CoordinatePrimitive xSum = lhs.xValue() + rhs.xValue();
	const CoordinatePrimitive ySum = lhs.yValue() + rhs.yValue();
	const CoordinatePrimitive zSum = lhs.zValue() + rhs.zValue();

	return MathTypes::Vector<3, CoordinatePrimitive>(xSum, ySum, zSum);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> operator-(const MathTypes::Vector<3, CoordinatePrimitive>& lhs,
	const MathTypes::Vector<3, CoordinatePrimitive>& rhs)
{
	const CoordinatePrimitive xDifference = lhs.xValue() - rhs.xValue();
	const CoordinatePrimitive yDifference = lhs.yValue() - rhs.yValue();
	const CoordinatePrimitive zDifference = lhs.zValue() - rhs.zValue();

	return MathTypes::Vector<3, CoordinatePrimitive>(xDifference, yDifference, zDifference);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> operator*(const MathTypes::Vector<3, CoordinatePrimitive>& lhs, double scalar)
{
	const CoordinatePrimitive xScaled = lhs.xValue() * scalar;
	const CoordinatePrimitive yScaled = lhs.yValue() * scalar;
	const CoordinatePrimitive zScaled = lhs.zValue() * scalar;

	return MathTypes::Vector<3, CoordinatePrimitive>(xScaled, yScaled, zScaled);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive> operator*(double scalar, const MathTypes::Vector<3, CoordinatePrimitive>& rhs)
{
	return rhs * scalar;
}

template<typename CoordinatePrimitive>
bool operator==(const MathTypes::Vector<3, CoordinatePrimitive>& lhs, 
	const MathTypes::Vector<3, CoordinatePrimitive>& rhs)
{
	return (lhs.xValue() == rhs.xValue()) && (lhs.yValue() == rhs.yValue()) && (lhs.zValue() == rhs.zValue());
}

template<typename CoordinatePrimitive>
bool operator==(const MathTypes::Vector<3, CoordinatePrimitive> lhs, 
	const MathTypes::Vector<3, CoordinatePrimitive> rhs)
{
	return (lhs.xValue() == rhs.xValue()) && (lhs.yValue() == rhs.yValue()) && (lhs.zValue() == rhs.zValue());
}

template<typename CoordinatePrimitive>
MathTypes::Vector<3, CoordinatePrimitive>::operator Matrix<3, 1, CoordinatePrimitive>() const
{
	auto columnVector = {
		{x_}, 
		{y_},
		{z_}
	};

	return MathTypes::Matrix<3, 1, CoordinatePrimitive>(columnVector);
}

template <typename CoordinatePrimitive>
std::string MathTypes::Vector<3, CoordinatePrimitive>::toString() const
{
	const char* formatString = "Three Dimensional Vector. (%.3f, %.3f, %.3f). Coordinate Type: %s";
	char buffer[100];

	sprintf(buffer, formatString, x_, y_, z_, typeid(CoordinatePrimitive).name());

	return std::string(buffer);
}

template<typename CoordinatePrimitive>
MathTypes::Vector<4, CoordinatePrimitive>::Vector(CoordinatePrimitive x, CoordinatePrimitive y, 
	CoordinatePrimitive z, CoordinatePrimitive w) : x_(x), y_(y), z_(z), w_(w)
{
}

template <typename CoordinatePrimitive>
MathTypes::Vector<4, CoordinatePrimitive>::Vector(std::array<CoordinatePrimitive, 4> data)
	: x_(data[0]), y_(data[1]), z_(data[2]), w_(data[3])
{
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<4, CoordinatePrimitive>::xValue() const
{
	return x_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<4, CoordinatePrimitive>::yValue() const
{
	return y_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<4, CoordinatePrimitive>::zValue() const
{
	return z_;
}

template<typename CoordinatePrimitive>
CoordinatePrimitive MathTypes::Vector<4, CoordinatePrimitive>::wValue() const
{
	return w_;
}

template<typename CoordinatePrimitive>
MathTypes::Vector<4, CoordinatePrimitive>::operator Matrix<4, 1, CoordinatePrimitive>() const
{
	CoordinatePrimitive columnVector[4][1] = {
		{x_}, 
		{y_},
		{z_},
		{w_}
	};

	return MathTypes::Matrix<4, 1, CoordinatePrimitive>(columnVector);
}

template <typename CoordinatePrimitive>
std::string MathTypes::Vector<4, CoordinatePrimitive>::toString() const
{
	const char* formatString = "Four Dimensional Vector. (%.3f, %.3f, %.3f, %.3f). Coordinate Type: %s";
	char buffer[100];

	sprintf(buffer, formatString, x_, y_, z_, w_, typeid(CoordinatePrimitive).name());

	return std::string(buffer);
}
