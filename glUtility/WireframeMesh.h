#pragma once

#include <iterator>
#include <numeric>
#include <vector>

#include "math/LinearMath.h"
#include "math/Vector.h"

namespace GLUtility
{
	template<typename CoordinatePrimitive, typename IndexPrimitive>
	class WireframeMesh
	{
	public:
		WireframeMesh(const std::vector<CoordinatePrimitive>& vertices, const std::vector<IndexPrimitive>& vertexIndices);
		WireframeMesh(
			const std::vector<CoordinatePrimitive>& vertices, 
			const std::vector<CoordinatePrimitive>& textures,
			const std::vector<IndexPrimitive>& vertexIndices,
			const std::vector<IndexPrimitive>& textureIndices);
		WireframeMesh(
			const std::vector<CoordinatePrimitive>& vertices, 
			const std::vector<CoordinatePrimitive>& textures,
			const std::vector<CoordinatePrimitive>& normals,
			const std::vector<IndexPrimitive>& vertexIndices,
			const std::vector<IndexPrimitive>& textureIndices,
			const std::vector<IndexPrimitive>& normalIndices);
		~WireframeMesh() = default;

		const CoordinatePrimitive* vertices() const;
		int numberOfVertices() const;
		const CoordinatePrimitive* textures() const;
		int numberOfTextures() const;
		const CoordinatePrimitive* normals() const;
		int numberOfNormals() const;

		const IndexPrimitive* vertexIndices() const;
		int numberOfVertexIndices() const;
		const IndexPrimitive* textureIndices() const;
		int numberOfTextureIndices() const;
		const IndexPrimitive* normalIndices() const;
		int numberOfNormalIndices() const;

	private:
		void alignTextures();
		void alignNormals();
		void calculateVertexNormals();
		MathTypes::Vector<3, CoordinatePrimitive> normalForTriangleStartingAtIndexPosition(int position);
		void shrinkAllListsToFit();

	private:
		std::vector<CoordinatePrimitive> vertices_;
		std::vector<CoordinatePrimitive> textures_;
		std::vector<CoordinatePrimitive> normals_;

		std::vector<IndexPrimitive> vertexIndices_;
		std::vector<IndexPrimitive> textureIndices_;
		std::vector<IndexPrimitive> normalIndices_;
	};
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::WireframeMesh(
	const std::vector<CoordinatePrimitive>& vertices,
	const std::vector<IndexPrimitive>& vertexIndices) 
: vertices_(vertices), textures_(), normals_(), vertexIndices_(vertexIndices), textureIndices_(), normalIndices_()
{
	shrinkAllListsToFit();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::WireframeMesh(
	const std::vector<CoordinatePrimitive>& vertices, 
	const std::vector<CoordinatePrimitive>& textures,
	const std::vector<IndexPrimitive>& vertexIndices,
	const std::vector<IndexPrimitive>& textureIndices) 
: vertices_(vertices)
, textures_(textures)
, normals_()
, vertexIndices_(vertexIndices)
, textureIndices_(textureIndices)
, normalIndices_()
{
	shrinkAllListsToFit();
	calculateVertexNormals();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::WireframeMesh(
	const std::vector<CoordinatePrimitive>& vertices, 
	const std::vector<CoordinatePrimitive>& textures,
	const std::vector<CoordinatePrimitive>& normals,
	const std::vector<IndexPrimitive>& vertexIndices,
	const std::vector<IndexPrimitive>& textureIndices,
	const std::vector<IndexPrimitive>& normalIndices) 
: vertices_(vertices)
, textures_(textures)
, normals_(normals)
, vertexIndices_(vertexIndices)
, textureIndices_(textureIndices)
, normalIndices_(normalIndices)
{
	shrinkAllListsToFit();
	alignTextures();
	alignNormals();
	if(normals_.size() == 0)
	{
		calculateVertexNormals();
	}
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
const CoordinatePrimitive* GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::vertices() const
{
	return vertices_.data();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
int GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::numberOfVertices() const
{
	return vertices_.size();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
const CoordinatePrimitive* GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::textures() const
{
	return textures_.data();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
int GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::numberOfTextures() const
{
	return textures_.size();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
const CoordinatePrimitive* GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::normals() const
{
	return normals_.data();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
int GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::numberOfNormals() const
{
	return normals_.size();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
const IndexPrimitive* GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::vertexIndices() const
{
	return vertexIndices_.data();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
int GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::numberOfVertexIndices() const
{
	return vertexIndices_.size();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
const IndexPrimitive* GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::textureIndices() const
{
	return textureIndices_.data();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
int GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::numberOfTextureIndices() const
{
	return textureIndices_.size();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
const IndexPrimitive* GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::normalIndices() const
{
	return normalIndices_.data();
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
int GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::numberOfNormalIndices() const
{
	return normalIndices_.size();
}

//Complexity of alignTextures, alignNormals, and calculateVertexNormals due to lack of encapsulating vertex
//data in an intermediary class.

template<typename CoordinatePrimitive, typename IndexPrimitive>
void GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::alignTextures()
{
	std::vector<CoordinatePrimitive> sortedTextures(textures_.size());
	for(int i = 0; i < vertexIndices_.size() && i < textureIndices_.size(); i++)
	{
		sortedTextures[vertexIndices_[i] * 2] = textures_[textureIndices_[i] * 2];
		sortedTextures[(vertexIndices_[i] * 2) + 1] = textures_[(textureIndices_[i] * 2) + 1];
	}
	textures_ = sortedTextures;
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
void GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::alignNormals()
{
	std::vector<CoordinatePrimitive> sortedNormals(normals_.size());
	for(int i = 0; i < vertexIndices_.size() && i < normalIndices_.size(); i++)
	{
		sortedNormals[vertexIndices_[i] * 3] = normals_[normalIndices_[i] * 3];
		sortedNormals[(vertexIndices_[i] * 3) + 1] = normals_[(normalIndices_[i] * 3) + 1];
		sortedNormals[(vertexIndices_[i] * 3) + 2] = normals_[(normalIndices_[i] * 3) + 2];
	}
	normals_ = sortedNormals;
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
void GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::calculateVertexNormals()
{
	std::vector<std::vector<MathTypes::Vector<3, CoordinatePrimitive>>> normalsAtIndices(vertices_.size());
	normals_.reserve(vertices_.size());
	for(int i = 0; i < vertexIndices_.size(); i+=3)
	{
		auto normalForTriangle = normalForTriangleStartingAtIndexPosition(i);
		normalsAtIndices[vertexIndices_[i]].push_back(normalForTriangle);
		normalsAtIndices[vertexIndices_[i+1]].push_back(normalForTriangle);
		normalsAtIndices[vertexIndices_[i+2]].push_back(normalForTriangle);
	}
	for(int i = 0; i < normalsAtIndices.size(); i++)
	{
		int n = 0;
		auto averageNormal = std::accumulate(std::begin(normalsAtIndices[i]), std::end(normalsAtIndices[i]), 
			MathTypes::Vector<3, CoordinatePrimitive>(0, 0, 0),
			[&](MathTypes::Vector<3, CoordinatePrimitive> sum, MathTypes::Vector<3, CoordinatePrimitive> current)
			{
				n++;
				sum = sum + current;
				return sum;
			});
		averageNormal = averageNormal * (1./n);
		normals_[vertexIndices_[i] * 3] = averageNormal.xValue();
		normals_[(vertexIndices_[i] * 3) + 1] = averageNormal.yValue();
		normals_[(vertexIndices_[i] * 3) + 2] = averageNormal.zValue();
	}
}

template<typename CoordinatePrimitive, typename IndexPrimitive>
MathTypes::Vector<3, CoordinatePrimitive> 
GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::normalForTriangleStartingAtIndexPosition(int position)
{
	MathTypes::Vector<3, CoordinatePrimitive> a(
		vertices_[vertexIndices_[position] * 3], 
		vertices_[(vertexIndices_[position] * 3) + 1],
		vertices_[(vertexIndices_[position] * 3) +2]);
	MathTypes::Vector<3, CoordinatePrimitive> b(
		vertices_[vertexIndices_[position + 1] * 3], 
		vertices_[(vertexIndices_[position + 1] * 3) + 1],
		vertices_[(vertexIndices_[position + 1] * 3) +2]);
	MathTypes::Vector<3, CoordinatePrimitive> c(
		vertices_[vertexIndices_[position + 2] * 3], 
		vertices_[(vertexIndices_[position + 2] * 3) + 1],
		vertices_[(vertexIndices_[position + 2] * 3) +2]);

	auto ab = b - a;
	auto bc = c - b;
	return LinearMath::crossProduct(ab, bc);
}


template<typename CoordinatePrimitive, typename IndexPrimitive>
void GLUtility::WireframeMesh<CoordinatePrimitive, IndexPrimitive>::shrinkAllListsToFit()
{
	vertices_.shrink_to_fit();
	textures_.shrink_to_fit();
	normals_.shrink_to_fit();
	vertexIndices_.shrink_to_fit();
	textureIndices_.shrink_to_fit();
	normalIndices_.shrink_to_fit();
}
