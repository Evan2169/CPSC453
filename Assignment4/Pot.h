#pragma once

#include <tuple>
#include <vector>

#include "assignmentSpecific/Curve.h"
#include "glUtility/Vertex.h"

namespace Curves
{
	class Pot
	{
	public:
		Pot(const Curves::Curve& curve, const GLUtility::Colour<float>& colour);
		~Pot() = default;

		void setControlCurve(const Curves::Curve& curve);
		std::vector<GLUtility::Vertex<float, float>> vertices() const;

	private:
		void calculateVertexNormalsOfCurve();
		void calculateSurfaceOfRevolutionCurves();
		void calculateSurfaceOfRevolution();

	private:
		Curves::Curve curve_;
		GLUtility::Colour<float> colour_;
		std::vector<MathTypes::Vector<3, float>> vertexNormalsOfCurve_;
		std::vector<std::vector<std::tuple<MathTypes::Vector<3, float>, MathTypes::Vector<3, float>>>> curves_;
		std::vector<std::tuple<MathTypes::Vector<3, float>, MathTypes::Vector<3, float>>> surfaceOfRevolution_;
	};
}