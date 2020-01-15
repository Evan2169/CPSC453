#pragma once

#include <vector>

#include "glUtility/Vertex.h"
#include "math/Vector.h"

namespace Curves
{
	class Curve
	{
	public:
		Curve(
			const MathTypes::Vector<3, float>& start,	
			const MathTypes::Vector<3, float>& end,
			const GLUtility::Colour<float>& colourOfControlCurve,
			const GLUtility::Colour<float>& colourOfSmoothedCurve);
		~Curve() = default;

		void insertPoint();
		void deletePoint();
		std::vector<GLUtility::Vertex<float, float>> vertices() const;
		std::vector<MathTypes::Vector<3, float>> coordinates() const;
		std::vector<GLUtility::Vertex<float, float>> verticesOfSmoothedCurve() const;
		std::vector<MathTypes::Vector<3, float>> coordinatesOfSmoothedCurve() const;
		std::vector<GLUtility::Vertex<float, float>> verticesOfBothCurves() const;
		MathTypes::Vector<3, float>* intersectedCoordinate(const MathTypes::Vector<3, float>& coordinateToCheck);

	private:
		std::vector<MathTypes::Vector<3, float>> points_;
		std::vector<MathTypes::Vector<3, float>>::iterator insertionPoint_;
		GLUtility::Colour<float> colourOfControlCurve_;
		GLUtility::Colour<float> colourOfSmoothedCurve_;
	};
}