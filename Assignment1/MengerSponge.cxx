#include "MengerSponge.h"

#include <algorithm>
#include <functional>
#include <list>
#include <tuple>
#include <vector>

#include "glUtility/DrawableCube.h"
#include "math/Vector.h"

using namespace GLUtility;
using namespace MathTypes;
using namespace Shapes;

std::vector<DrawableCube<float, float>> MengerSponge::cubesForMengerSponge(
	int levelOfRecursion, const Vector<3, float>& bottomLeftCorner, float sideLength) const
{
	std::vector<DrawableCube<float, float>> allCubes;

	if(levelOfRecursion > 0)
	{
		const float sideLengthDividedByThree = sideLength / 3;

		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 3; k++)
				{
					if(cubeShouldBeDrawnGivenIndicesOfPosition(i, j, k))
					{
						auto cubes = cubesForMengerSponge(
							levelOfRecursion - 1, 
							bottomLeftCorner + Vector<3, float>(i*sideLengthDividedByThree, 
																			j*sideLengthDividedByThree, 
																			-k*sideLengthDividedByThree), 
							sideLengthDividedByThree);
						allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));
					}
				}
			}
		}

	}
	else if(levelOfRecursion == 0)
	{
		allCubes.push_back(baseCube(bottomLeftCorner, sideLength));
	}

	return allCubes;
}

DrawableCube<float, float> MengerSponge::baseCube(const Vector<3, float>& bottomLeftCorner, float sideLength) const
{
   return DrawableCube<float, float>(
   	Cube<float>(bottomLeftCorner, sideLength),
   	Colour<float>(0.5, 0.0, 1.0), 
   	Colour<float>(1.0, 0.0, 1.0),
   	Colour<float>(0.0, 1.0, 0.5),
   	Colour<float>(0.0, 0.1, 1.0), 
   	Colour<float>(0.0, 0.5, 1.0),
   	Colour<float>(0.0, 1.0, 1.0));
}

bool MengerSponge::cubeShouldBeDrawnGivenIndicesOfPosition(int widthIndex, int heightIndex, int depthIndex) const
{
	using std::make_tuple;
	std::list<std::tuple<int, int, int>> positionsOfCubesThatShouldNotBeDrawn({make_tuple(1, 1, 0), make_tuple(1, 1, 1), 
		make_tuple(1, 1, 2), make_tuple(0, 1, 1), make_tuple(2, 1, 1), make_tuple(1, 0, 1), make_tuple(1, 2, 1)});

	return std::find(std::begin(positionsOfCubesThatShouldNotBeDrawn), std::end(positionsOfCubesThatShouldNotBeDrawn),
		make_tuple(widthIndex, heightIndex, depthIndex)) == std::end(positionsOfCubesThatShouldNotBeDrawn);
}