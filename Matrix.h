#pragma once

#include <array>
#include <cassert>
#include <cstdio>
#include <initializer_list>
#include <string>
#include <typeinfo>

namespace MathTypes
{
	template <int rows, int columns, typename CoordinatePrimitive>
	class Matrix
	{
		public:
			explicit Matrix(std::initializer_list<std::initializer_list<CoordinatePrimitive>> data);
			explicit Matrix(CoordinatePrimitive data[rows][columns]);
			~Matrix() = default;

			std::array<CoordinatePrimitive, columns> operator[](int row) const;
			std::array<CoordinatePrimitive, columns> ithRow(int i);
			std::array<CoordinatePrimitive, rows> jthColumn(int j);

			std::string toString() const;

		private:
			std::array<std::array<CoordinatePrimitive, columns>, rows> data_;
	};
}

template <int lhsRows, int lhsColumns, int rhsRows, int rhsColumns, typename CoordinatePrimitive>
MathTypes::Matrix<lhsRows, rhsColumns, CoordinatePrimitive> operator*(
	MathTypes::Matrix<lhsRows, lhsColumns, CoordinatePrimitive> lhs, 
	MathTypes::Matrix<rhsRows, rhsColumns, CoordinatePrimitive> rhs);

template<int rows, int columns, typename CoordinatePrimitive>
MathTypes::Matrix<rows, columns, CoordinatePrimitive>::Matrix(
	std::initializer_list<std::initializer_list<CoordinatePrimitive>> data)
{
	assert(data.size() == rows);

	int i = 0;
	for(auto rowIterator = data.begin(); rowIterator != data.end(); rowIterator++)
	{
		assert(rowIterator->size() == columns);

		int j = 0;
		for(auto columnIterator = rowIterator->begin(); columnIterator != rowIterator->end(); columnIterator++)
		{
			data_[i][j] = *columnIterator;
			j++;
		}
		i++;
	}
}

template<int rows, int columns, typename CoordinatePrimitive>
MathTypes::Matrix<rows, columns, CoordinatePrimitive>::Matrix(CoordinatePrimitive data[rows][columns])
{
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			data_[i][j] = data[i][j];
		}
	}
}

template<int rows, int columns, typename CoordinatePrimitive>
std::string MathTypes::Matrix<rows, columns, CoordinatePrimitive>::toString() const
{
	const char* baseDescription = "%dX%d Matrix. Coordinate Type: %s. Data:\n";
	char buffer[100];
	sprintf(buffer, baseDescription, rows, columns, typeid(CoordinatePrimitive).name());

	const char* numberFormat = "%.3f ";
	std::string matrixDescription(buffer); 
	for(auto rowIterator = data_.begin(); rowIterator != data_.end(); rowIterator++)
	{
		for(auto columnIterator = rowIterator->begin(); columnIterator != rowIterator->end(); columnIterator++)
		{
			sprintf(buffer, numberFormat, *columnIterator);
			matrixDescription += std::string(buffer);
		}
		matrixDescription += std::string("\n");
	}

	return matrixDescription;
}

template <int rows, int columns, typename CoordinatePrimitive>
std::array<CoordinatePrimitive, columns> MathTypes::Matrix<rows, columns, CoordinatePrimitive>::operator[](int row) const
{
	assert(row >= 0 && row < data_.size());

	return data_[row];
}

template <int lhsRows, int lhsColumns, int rhsRows, int rhsColumns, typename CoordinatePrimitive>
MathTypes::Matrix<lhsRows, rhsColumns, CoordinatePrimitive> operator*(
	MathTypes::Matrix<lhsRows, lhsColumns, CoordinatePrimitive> lhs, 
	MathTypes::Matrix<rhsRows, rhsColumns, CoordinatePrimitive> rhs)
	{
		assert(lhsColumns == rhsRows);

		auto innerProduct = [=](const std::array<CoordinatePrimitive, lhsColumns>& row, 
			const std::array<CoordinatePrimitive, rhsRows>& column)
		{
			CoordinatePrimitive currentSum = 0;

			//lhsColumns == rhsRows
			for(int index = 0; index < rhsRows; index++)
			{
				currentSum += row[index]*column[index];
			}
			return currentSum;
		};

		CoordinatePrimitive data[lhsRows][rhsColumns];
		for(int i = 0; i < lhsRows; i++)
		{
			auto currentRow = lhs.ithRow(i);
			for(int j = 0; j < rhsColumns; j++)
			{
				auto currentColumn = rhs.jthColumn(j);

				data[i][j] = innerProduct(currentRow, currentColumn);
			}
		}

		return MathTypes::Matrix<lhsRows, rhsColumns, CoordinatePrimitive>(data);
	}

template<int rows, int columns, typename CoordinatePrimitive>
std::array<CoordinatePrimitive, columns> MathTypes::Matrix<rows, columns, CoordinatePrimitive>::ithRow(int i)
{
	return operator[](i);
}

template<int rows, int columns, typename CoordinatePrimitive>
std::array<CoordinatePrimitive, rows> MathTypes::Matrix<rows, columns, CoordinatePrimitive>::jthColumn(int j)
{
	std::array<CoordinatePrimitive, rows> buffer;

	for(int i = 0; i < rows; i++)
	{
		buffer[i] = data_[i][j];
	}

	return buffer;
}