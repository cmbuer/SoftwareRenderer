#pragma once
//#include "Software3D.h"
#include "Math3D.h"

using namespace Software3D;

class Sphere 
{
public:
	static IndexedList<BasicVertex> GetPlain(float radius = 1.0f, int latDiv = 12, int longDiv = 24)
	{
		const Vector3 base = { 0.0f,0.0f,radius };
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<BasicVertex> vertices;
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = base * Matrix3x3::RotationX(lattitudeAngle * iLat);
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				vertices.back().position = latBase * Matrix3x3::RotationZ(longitudeAngle * iLong);
			}
		}

		// add the cap vertices
		const auto iNorthPole = vertices.size();
		vertices.emplace_back();
		vertices.back().position = base;
		const auto iSouthPole = vertices.size();
		vertices.emplace_back();
		vertices.back().position = -base;

		const auto calcIdx = [latDiv, longDiv](int iLat, int iLong)
		{ return iLat * longDiv + iLong; };
		std::vector<unsigned int> indices;
		for (int iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (int iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (int iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);


		return{ std::move(vertices),std::move(indices) };
	}
	

	static IndexedList<FixedColorVertex> GetSolidColor(float radius = 1.0f, int latDiv = 12, int longDiv = 24, Color color = Colors::White)
	{
		const Vector3 base = { 0.0f,0.0f,radius };
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<FixedColorVertex> vertices;
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = base * Matrix3x3::RotationX(lattitudeAngle * iLat);
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				vertices.back().position = latBase * Matrix3x3::RotationZ(longitudeAngle * iLong);
				vertices.back().color = color;
			}
		}

		// add the cap vertices
		const auto iNorthPole = vertices.size();
		vertices.emplace_back();
		vertices.back().position = base;
		vertices.back().color = color;
		const auto iSouthPole = vertices.size();
		vertices.emplace_back();
		vertices.back().position = -base;
		vertices.back().color = color;

		const auto calcIdx = [latDiv, longDiv](int iLat, int iLong)
		{ return iLat * longDiv + iLong; };
		std::vector<unsigned int> indices;
		for (int iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (int iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (int iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);


		return{ std::move(vertices),std::move(indices) };
	}


	static IndexedList<NormalVertex> GetPlainNormals(float radius = 1.0f, int latDiv = 12, int longDiv = 24)
	{
		IndexedList<BasicVertex> plainSphere = GetPlain(radius, latDiv, longDiv);
		IndexedList<NormalVertex> normalSphere;
		normalSphere.vertices.resize(plainSphere.vertices.size());
		normalSphere.indices = plainSphere.indices;
		
		for (int i = 0; i < plainSphere.vertices.size(); i++) {
			normalSphere.vertices[i].position = plainSphere.vertices[i].position;
			normalSphere.vertices[i].surfaceNormal = plainSphere.vertices[i].position.GetNormalized();
		}

		return normalSphere;
	}

	// test function w/ fixed tex coords
	static IndexedList<TextureVertex> GetSkinned(float radius = 1.0f, int latDiv = 12, int longDiv = 24)
	{
		IndexedList<BasicVertex> plainSphere = GetPlain(radius, latDiv, longDiv);
		IndexedList<TextureVertex> skinnedSphere;
		skinnedSphere.vertices.resize(plainSphere.vertices.size());
		skinnedSphere.indices = plainSphere.indices;

		for (int i = 0; i < plainSphere.vertices.size(); i++) {
			skinnedSphere.vertices[i].position = plainSphere.vertices[i].position;
			skinnedSphere.vertices[i].texture = { 0.5f, 0.5f };
		}

		return skinnedSphere;
	}
};


