#include "Pot.h"

#include "math/Matrices.h"

Curves::Pot::Pot(const Curves::Curve& curve, const GLUtility::Colour<float>& colour)
: curve_(curve), colour_(colour)
{
	calculateVertexNormalsOfCurve();
	calculateSurfaceOfRevolutionCurves();
	calculateSurfaceOfRevolution();
}

void Curves::Pot::setControlCurve(const Curves::Curve& curve)
{
	curve_ = curve;
	vertexNormalsOfCurve_.clear();
	curves_.clear();
	surfaceOfRevolution_.clear();
	calculateVertexNormalsOfCurve();
	calculateSurfaceOfRevolutionCurves();
	calculateSurfaceOfRevolution();
}

std::vector<GLUtility::Vertex<float, float>> Curves::Pot::vertices() const
{
	using namespace std;

	vector<GLUtility::Vertex<float, float>> verts;
	
	transform(std::begin(surfaceOfRevolution_), std::end(surfaceOfRevolution_), back_inserter(verts), 
		[=](const decltype(surfaceOfRevolution_[0])& pointAndNormal)
		{
			return GLUtility::Vertex<float, float>(std::get<0>(pointAndNormal), colour_, std::get<1>(pointAndNormal));
		});

	return verts;
}

void Curves::Pot::calculateVertexNormalsOfCurve()
{
	std::vector<MathTypes::Vector<3, float>> normals;
	MathTypes::Vector<3, float> previousNormal(0, 0, 0);
	for(int i = 1; i < curve_.coordinatesOfSmoothedCurve().size(); i++)
	{
		auto differenceVector = curve_.coordinatesOfSmoothedCurve()[i] - curve_.coordinatesOfSmoothedCurve()[i-1];
		MathTypes::Vector<3, float> currentNormal(-1*differenceVector.yValue(), differenceVector.xValue(), 0);
		currentNormal = currentNormal.normalized();

		if(curve_.coordinatesOfSmoothedCurve().size() > 2)
		{
			if(i == 1)
			{
				//Can't take an average normal for the first vertex, as it is only part of one line segment
				normals.push_back(currentNormal);
			}
			else
			{
				//Take an average normal for vertices that are part of two line segments			
				normals.push_back(((0.5 * previousNormal) + (0.5 * currentNormal)).normalized());	
			}
			
			if(i == curve_.coordinatesOfSmoothedCurve().size() - 1)
			{
				//Can't take an average normal for the last vertex, as it is only part of one line segment
				normals.push_back(currentNormal);
			}
		}
		else if (curve_.coordinatesOfSmoothedCurve().size() == 2)
		{
			normals.push_back(currentNormal);
			normals.push_back(currentNormal);
		}
		previousNormal = currentNormal;
	}
	vertexNormalsOfCurve_ = normals;
}

void Curves::Pot::calculateSurfaceOfRevolutionCurves()
{
	auto pointsOfInitialCurve = curve_.coordinatesOfSmoothedCurve();

	if(pointsOfInitialCurve.size() == vertexNormalsOfCurve_.size())
	{
		for(float rotationAngle = 0; rotationAngle < 360; rotationAngle += 15)
		{
			std::vector<std::tuple<MathTypes::Vector<3, float>, MathTypes::Vector<3, float>>> rotatedPointsAndNormals;
			auto rotationMatrix = Matrices::rotateAboutY3D(rotationAngle);
			for(int i = 0; i < pointsOfInitialCurve.size(); i++)
			{
				auto rotatedPoint = 
					rotationMatrix * static_cast<MathTypes::Matrix<4, 1, float>>(pointsOfInitialCurve[i].homogenized());
				auto rotatedNormal = 
					rotationMatrix * static_cast<MathTypes::Matrix<4, 1, float>>(vertexNormalsOfCurve_[i].homogenized());
				
				rotatedPointsAndNormals.push_back(std::make_tuple(
					MathTypes::Vector<3, float>(rotatedPoint[0][0], rotatedPoint[1][0], rotatedPoint[2][0]),
					MathTypes::Vector<3, float>(rotatedNormal[0][0], rotatedNormal[1][0], rotatedNormal[2][0]).normalized()));
			}
			rotatedPointsAndNormals.shrink_to_fit();
			curves_.push_back(rotatedPointsAndNormals);
		}
	}
}

void Curves::Pot::calculateSurfaceOfRevolution()
{
	for(int i = 0; i < curves_.size() - 1; i++)
	{
		auto curveOne = curves_[i];
		auto curveTwo = curves_[i + 1];

		//Create a triangle between the bottom of the curve and centre of rotation
		surfaceOfRevolution_.push_back(curveOne.back());
		surfaceOfRevolution_.push_back(curveTwo.back());
		surfaceOfRevolution_.push_back(std::make_tuple(
			MathTypes::Vector<3, float>(0, std::get<0>(curveOne.back()).yValue(), 0),
			MathTypes::Vector<3, float>(0, -1, 0)));

		//Create triangles between the two curves
		for(int j = 0; j < curveOne.size() - 1; j++)
		{
			surfaceOfRevolution_.push_back(curveOne[j]);
			surfaceOfRevolution_.push_back(curveOne[j + 1]);
			surfaceOfRevolution_.push_back(curveTwo[j]);

			surfaceOfRevolution_.push_back(curveTwo[j]);
			surfaceOfRevolution_.push_back(curveOne[j + 1]);
			surfaceOfRevolution_.push_back(curveTwo[j + 1]);
		}
	}

	//Do the same two operations as above, but for the first and last curves
	auto firstCurve = curves_[0];
	auto lastCurve = curves_[curves_.size() - 1];
	surfaceOfRevolution_.push_back(firstCurve.back());
	surfaceOfRevolution_.push_back(lastCurve.back());
	surfaceOfRevolution_.push_back(std::make_tuple(
		MathTypes::Vector<3, float>(0, std::get<0>(firstCurve.back()).yValue(), 0),
		MathTypes::Vector<3, float>(0, -1, 0)));
	for(int j = 0; j < firstCurve.size() - 1; j++)
	{
		surfaceOfRevolution_.push_back(lastCurve[j]);
		surfaceOfRevolution_.push_back(lastCurve[j + 1]);
		surfaceOfRevolution_.push_back(firstCurve[j]);

		surfaceOfRevolution_.push_back(firstCurve[j]);
		surfaceOfRevolution_.push_back(lastCurve[j + 1]);
		surfaceOfRevolution_.push_back(firstCurve[j + 1]);
	}
}