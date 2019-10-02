#include "MengerSponge.h"

#include <functional>

#include "DrawableCube.h"
#include "Vector.h"

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

		//Front bottom left
		auto cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, 0, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Middle bottom left
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, 0, -1*sideLengthDividedByThree), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back bottom left
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, 0, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Front middle left
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, sideLengthDividedByThree, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back middle left
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, sideLengthDividedByThree, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Front top left
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, sideLengthDividedByThree*2, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Middle top left
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, sideLengthDividedByThree*2, -1*sideLengthDividedByThree), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back top left
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(0, sideLengthDividedByThree*2, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Front bottom middle
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree, 0, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back bottom middle
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree, 0, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Front top middle
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree, sideLengthDividedByThree*2, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back top middle
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree, sideLengthDividedByThree*2, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Front bottom right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, 0, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Middle bottom right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, 0, -1*sideLengthDividedByThree), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back bottom right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, 0, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Front middle right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, sideLengthDividedByThree, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back middle right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, sideLengthDividedByThree, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Front top right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, sideLengthDividedByThree*2, 0), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Middle top right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, sideLengthDividedByThree*2, -1*sideLengthDividedByThree), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));

		//Back top right
		cubes = cubesForMengerSponge(
			levelOfRecursion - 1, 
			bottomLeftCorner + Vector<3, float>(sideLengthDividedByThree*2, sideLengthDividedByThree*2, -1*sideLengthDividedByThree*2), 
			sideLengthDividedByThree);
		allCubes.insert(std::end(allCubes), std::begin(cubes), std::end(cubes));
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
   	Colour<float>(1, 0, 0), 
   	Colour<float>(0.85, 0.85, 0.85),
   	Colour<float>(0, 1, 0),
   	Colour<float>(0.75, 0.75, 0.75), 
   	Colour<float>(0, 0, 1),
   	Colour<float>(0.65, 0.65, 0.65));
   	// Colour<float>(0.90, 0.90, 0.90), 
   	// Colour<float>(0.85, 0.85, 0.85),
   	// Colour<float>(0.80, 0.80, 0.80),
   	// Colour<float>(0.75, 0.75, 0.75), 
   	// Colour<float>(0.70, 0.70, 0.70),
   	// Colour<float>(0.65, 0.65, 0.65));
}