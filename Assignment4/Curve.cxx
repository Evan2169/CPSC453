#include "Curve.h"

#include <algorithm>
#include <iterator>

#include "math/LinearMath.h"

namespace
{
	const float CALCULATION_EPSILON = 0.5;
}

Curves::Curve::Curve(
	const MathTypes::Vector<3, float>& start, 
	const MathTypes::Vector<3, float>& end,
	const GLUtility::Colour<float>& colourOfControlCurve,
	const GLUtility::Colour<float>& colourOfSmoothedCurve)
: points_({start, end})
, insertionPoint_(std::begin(points_) + 1)
, colourOfControlCurve_(colourOfControlCurve)
, colourOfSmoothedCurve_(colourOfSmoothedCurve)
{
}

void Curves::Curve::insertPoint()
{
	auto midpoint = (0.5 * *(insertionPoint_ - 1)) + (0.5 * *(insertionPoint_));
	insertionPoint_ = points_.insert(insertionPoint_, midpoint);
	for(int i = 0; i < 2; i++)
	{
		insertionPoint_++;
		if(insertionPoint_ == std::end(points_))
		{
			insertionPoint_ = std::begin(points_) + 1;
		}
	}
}

void Curves::Curve::deletePoint()
{
	if(points_.size() > 2)
	{
		for(int i = 0; i < 2; i++)
		{
			insertionPoint_--;
			if(insertionPoint_ == std::begin(points_))
			{
				insertionPoint_ = std::end(points_) - 1;
			}
		}
		insertionPoint_ = points_.erase(insertionPoint_);
	}
}

std::vector<GLUtility::Vertex<float, float>> Curves::Curve::vertices() const
{
	using namespace std;

	vector<GLUtility::Vertex<float, float>> verts;
	const auto coordinates = points_;
	
	transform(coordinates.begin(), coordinates.end(), back_inserter(verts), 
		[=](const decltype(coordinates[0])& nakedVertex)
		{
			return GLUtility::Vertex<float, float>(nakedVertex, colourOfControlCurve_);
		});

	return verts;
}

std::vector<GLUtility::Vertex<float, float>> Curves::Curve::verticesOfSmoothedCurve() const
{
	using namespace std;

	vector<GLUtility::Vertex<float, float>> verts;
	const auto coordinates = coordinatesOfSmoothedCurve();
	
	transform(coordinates.begin(), coordinates.end(), back_inserter(verts), 
		[=](const decltype(coordinates[0])& nakedVertex)
		{
			return GLUtility::Vertex<float, float>(nakedVertex, colourOfSmoothedCurve_);
		});

	return verts;
}

std::vector<GLUtility::Vertex<float, float>> Curves::Curve::verticesOfBothCurves() const
{
	std::vector<GLUtility::Vertex<float, float>> verts;
	auto controlVertices = vertices();
	auto smoothedVertices = verticesOfSmoothedCurve();
	verts.reserve(smoothedVertices.size() + controlVertices.size());
	verts.insert(std::begin(verts), std::begin(controlVertices), std::end(controlVertices));
	verts.insert(std::end(verts), std::begin(smoothedVertices), std::end(smoothedVertices));	
	return verts;	
}

std::vector<MathTypes::Vector<3, float>> Curves::Curve::coordinates() const
{
	return points_;
}

std::vector<MathTypes::Vector<3, float>> Curves::Curve::coordinatesOfSmoothedCurve() const
{
	//Two iterations of the Lane-Riesenfeld Algorithm with two chases
	auto pointsOfSmoothedCurve = points_;
	for(int iterations = 0; iterations < 2; iterations++)
	{
		//Step one: Create Points at halfway points
		auto insertionPoint = std::begin(pointsOfSmoothedCurve) + 1;
		bool endReached = false;
		while(!endReached)
		{
			auto midpoint = (0.5 * *(insertionPoint - 1)) + (0.5 * *(insertionPoint));
			insertionPoint = pointsOfSmoothedCurve.insert(insertionPoint, midpoint);
			for(int i = 0; i < 2; i++)
			{
				insertionPoint++;
				if(insertionPoint == std::end(pointsOfSmoothedCurve))
				{
					endReached = true;
				}
			}
		}
	
		//Step two: Chase
		for(int n = 0; n < 2; n++)
		{
			std::vector<MathTypes::Vector<3, float>> chasedCoordinates;
			chasedCoordinates.push_back(pointsOfSmoothedCurve.front());
			for(auto point = std::begin(pointsOfSmoothedCurve); point != std::end(pointsOfSmoothedCurve) - 1; point++) 
			{
				chasedCoordinates.push_back((0.5 * *(point)) + (0.5 * *(point + 1)));
			}
			chasedCoordinates.push_back(pointsOfSmoothedCurve.back());
			pointsOfSmoothedCurve = chasedCoordinates;
		}
	}

	return pointsOfSmoothedCurve;
}

MathTypes::Vector<3, float>* Curves::Curve::intersectedCoordinate(const MathTypes::Vector<3, float>& coordinateToCheck)
{
	auto intersected = 
		std::find_if(std::begin(points_), std::end(points_), [=](const MathTypes::Vector<3, float>& currentCoordinate)
		{
			return LinearMath::distanceBetweenPoints(coordinateToCheck, currentCoordinate) 
						< CALCULATION_EPSILON ? true : false;
		});

	if(intersected != std::end(points_))
	{
		return &*intersected;
	}
	else
	{
		return NULL;
	}
}