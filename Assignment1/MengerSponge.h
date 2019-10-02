#pragma once

#include <vector>

namespace GLUtility
{
	template<typename CoordinatePrimitive, typename ColourPrimitive> class DrawableCube;
}

namespace MathTypes
{
	template<int dimensions, typename CoordinatePrimitive> class Vector;
}

class MengerSponge
{
public:
	MengerSponge() = default;
	~MengerSponge() = default;

	std::vector<GLUtility::DrawableCube<float, float>> cubesForMengerSponge(
		int levelOfRecursion, const MathTypes::Vector<3, float>& bottomLeftCorner, float sideLength) const;

private:
	GLUtility::DrawableCube<float, float> baseCube(
		const MathTypes::Vector<3, float>& bottomLeftCorner, float sideLength) const;
};