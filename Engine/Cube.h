#pragma once

//#include "Software3D.h"
#include "S3Dtypes.h"
#include "Math3D.h"


using namespace Software3D;

class Cube
{
public:
	static IndexedList<BasicVertex> GetPlain(float size = 1.0f)
	{
		const float side = size / 2.0f;

		std::vector<Vector3> vertices;

		vertices.emplace_back(-side, -side, -side); // 0
		vertices.emplace_back(side, -side, -side); // 1
		vertices.emplace_back(-side, side, -side); // 2
		vertices.emplace_back(side, side, -side); // 3
		vertices.emplace_back(-side, -side, side); // 4
		vertices.emplace_back(side, -side, side); // 5
		vertices.emplace_back(-side, side, side); // 6
		vertices.emplace_back(side, side, side); // 7

		std::vector<BasicVertex> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].position = vertices[i];
		}
		return{
			std::move(verts),{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}


	static IndexedList<TextureVertex> GetSkinned(float size = 1.0f)
	{
		IndexedList<TextureVertex> skinnedList;

		const float side = size / 2.0f;
		const auto ConvertTexCoord = [](float u, float v)
		{
			return Vector2{ (u + 1.0f) / 3.0f,v / 4.0f };
		};

		std::vector<Vector3> vertices;
		std::vector<Vector2> tc;

		vertices.emplace_back(-side, -side, -side); // 0
		tc.emplace_back(ConvertTexCoord(1.0f, 0.0f));
		vertices.emplace_back(side, -side, -side); // 1
		tc.emplace_back(ConvertTexCoord(0.0f, 0.0f));
		vertices.emplace_back(-side, side, -side); // 2
		tc.emplace_back(ConvertTexCoord(1.0f, 1.0f));
		vertices.emplace_back(side, side, -side); // 3
		tc.emplace_back(ConvertTexCoord(0.0f, 1.0f));
		vertices.emplace_back(-side, -side, side); // 4
		tc.emplace_back(ConvertTexCoord(1.0f, 3.0f));
		vertices.emplace_back(side, -side, side); // 5
		tc.emplace_back(ConvertTexCoord(0.0f, 3.0f));
		vertices.emplace_back(-side, side, side); // 6
		tc.emplace_back(ConvertTexCoord(1.0f, 2.0f));
		vertices.emplace_back(side, side, side); // 7
		tc.emplace_back(ConvertTexCoord(0.0f, 2.0f));
		vertices.emplace_back(-side, -side, -side); // 8
		tc.emplace_back(ConvertTexCoord(1.0f, 4.0f));
		vertices.emplace_back(side, -side, -side); // 9
		tc.emplace_back(ConvertTexCoord(0.0f, 4.0f));
		vertices.emplace_back(-side, -side, -side); // 10
		tc.emplace_back(ConvertTexCoord(2.0f, 1.0f));
		vertices.emplace_back(-side, -side, side); // 11
		tc.emplace_back(ConvertTexCoord(2.0f, 2.0f));
		vertices.emplace_back(side, -side, -side); // 12
		tc.emplace_back(ConvertTexCoord(-1.0f, 1.0f));
		vertices.emplace_back(side, -side, side); // 13
		tc.emplace_back(ConvertTexCoord(-1.0f, 2.0f));

		
		std::vector<TextureVertex> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].position = vertices[i];
			verts[i].texture = tc[i];
		}
		
		skinnedList.indices = {
				0,2,1,   2,3,1,
				4,8,5,   5,8,9,
				2,6,3,   3,6,7,
				4,5,7,   4,7,6,
				2,10,11, 2,11,6,
				12,3,7,  12,7,13
		};

		skinnedList.vertices = verts;
		

		return skinnedList;
	}
};