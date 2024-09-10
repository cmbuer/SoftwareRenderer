#pragma once
#include <functional>
#include <thread>
#include <xmmintrin.h>
#include <cmath>
#include "S3Dtypes.h"
#include "S3Dd3dinterface.h"
#include "S3DopenGLinterface.h"

namespace Software3D {

	class Pipeline {

	public:


		enum class HardwareInterface { Direct3D, OpenGL };
		HardwareInterface hwInterface = HardwareInterface::Direct3D;

		enum class TriangleType { FlatTop, FlatBottom };

		enum class WindingDirection { Clockwise = 1, CounterClockwise = -1 };
		WindingDirection windingDirection = WindingDirection::Clockwise;

		enum class CullingState { Back = 1, Front = -1, FrontAndBack = 0 };
		CullingState cullingState = CullingState::Back;


		enum class Rasterizer { Scanline, EdgeFunction };
		Rasterizer rasterizer = Rasterizer::Scanline;
		
		struct ColorBuffer {
			Color* colors;
			int size;
		};

		struct DepthBuffer {

			int width, height;
			std::unique_ptr<float[]> zBuffer;

			DepthBuffer(int width, int height) :
				width(width),
				height(height)
			{
				zBuffer = std::make_unique<float[]>(width * height);
				Clear();
			}
			bool TestAndSet(int x, int y, float depth) {
				if (depth < zBuffer[y * width + x]) {
					zBuffer[y * width + x] = depth;
					return true;
				}
				return false;
			}
			void Clear() {									
				for (int i = 0; i < width * height; i++)
					zBuffer[i] = INFINITY;
			}
		};
		
		

		DepthBuffer depthBuffer;
		std::unique_ptr<Surface> Texture;


		ColorBuffer colorBuffer;

		Matrix3x3 rotation;
		Vector3 translation;

		D3DInterface* hardwarePipeline;
		OpenGLinterface* openglpipeline;

		Pipeline() = default;

		Pipeline(D3DInterface* hw) :
			depthBuffer(hardwarePipeline->ScreenWidth, hardwarePipeline->ScreenHeight)
		{
			hardwarePipeline = hw;
		}

		Pipeline(D3DInterface* hw, OpenGLinterface* og) :
			depthBuffer(hardwarePipeline->ScreenWidth, hardwarePipeline->ScreenHeight)
		{
			hardwarePipeline = hw;
			openglpipeline = og;
		}
		
		void BeginFrame() {
			depthBuffer.Clear();
			hardwarePipeline->BeginFrame();
			//openglpipeline->buffer.Clear(Colors::Red);
		}

		void EndFrame() {
			if (hwInterface == HardwareInterface::Direct3D)
				hardwarePipeline->EndFrame();
			else if (hwInterface == HardwareInterface::OpenGL) {				
				openglpipeline->DrawLegacy();
			}
		}

		
		void BindColorBuffer(Color* colors, int size) {
			colorBuffer.colors = colors;
			colorBuffer.size = size;
		}
		

		void BindTexture(const std::wstring& filename) { Texture = std::make_unique<Surface>(Surface::FromFile(filename)); }
		void BindRotation(const Matrix3x3& rotation_in) { rotation = rotation_in; }
		void BindTranslation(const Vector3& translation_in) { translation = translation_in; }

	

		// transforms coordinates to NDC space, and further converts X and Y to screen space
		template <class VertexType>
		VertexType NDCtransform4(VertexType& vertex, Vector4 clipSpacePosition) {

			float worldZinverse = 1.0f / clipSpacePosition.w;  // (clip space W contains the world space Z value.)

			VertexType outVertex;
			outVertex = vertex;
			outVertex.position = clipSpacePosition;			
			outVertex *= worldZinverse;
			outVertex.position.w = worldZinverse;
			outVertex.position.x = (outVertex.position.x + 1.0f) * hardwarePipeline->ScreenWidth / 2.0f;				// x and y are converted fully to screen space here
			outVertex.position.y = (-outVertex.position.y + 1.0f) * hardwarePipeline->ScreenHeight / 2.0f;
			return outVertex;
		}


		template <class VertexType, class EffectType>
		void DrawSimple(const IndexedList<VertexType>& trianglesIn, EffectType effect) {

			std::vector<std::thread> threads;		// for multithreaded calls to the rasterizer


			// Input Assembler //
			// The applicable behaviorable of the Input Assembler stage is intertwined the
			// vertex shader application, etc.  Given that in this build the primitive
			// type is already defined/assumed as a triangle and that there is no semantic
			// tagging, the IA's role here is limited.
			

			// Vertex Shader //
			auto transformedVertex = effect->VertexShaderFn(trianglesIn.vertices[0]);
			IndexedList<decltype(transformedVertex)> triangles;
			triangles.indices = trianglesIn.indices;
			triangles.vertices.resize(trianglesIn.vertices.size());

			for (int i = 0; i < triangles.vertices.size(); i++) {
				VertexType v = trianglesIn.vertices[i];
				triangles.vertices[i] = effect->VertexShaderFn(v);				
			}

			// back face culling: if surface normal of triangle points in the same general
			// direction as the viewport normal (dot product >= 0), we cull
			triangles.cullFlags.resize(triangles.indices.size() / 3, false);
			for (int i = 0; i < triangles.indices.size() / 3; i++) {

				int vdx0 = triangles.indices[3 * i];
				int vdx1 = triangles.indices[3 * i + 1];
				int vdx2 = triangles.indices[3 * i + 2];
				triangles.cullFlags[i] = (int)cullingState * (int)windingDirection *( (triangles.vertices[vdx1] - triangles.vertices[vdx0]).position.Cross((triangles.vertices[vdx2] - triangles.vertices[vdx0]).position) * triangles.vertices[vdx0].position) >= 0.0f;

			}

						
			for (int i = 0; i < triangles.indices.size() / 3; i++) {

				if (triangles.cullFlags[i])
					continue;

				
				Triangle<decltype(transformedVertex)> t;
				t.vertexA = triangles.vertices[triangles.indices[3 * i]];
				t.vertexB = triangles.vertices[triangles.indices[3 * i + 1]];
				t.vertexC = triangles.vertices[triangles.indices[3 * i + 2]];


				// Geometry Shader //

				auto gsOutTri = effect->GeometryShaderFn(t);
				typedef decltype(gsOutTri.vertexA) VertexOutType;

				// Clipping //
				
				// Transformation to Homogenous Coordinates and Gemometric Clipping (near and far planes)
				Matrix4x4 projection = Matrix4x4::ProjectionHFOV(60.0f, 1.33333f, 1.0f, 10.0f);
				gsOutTri.vertexA.position *= projection;
				gsOutTri.vertexB.position *= projection;
				gsOutTri.vertexC.position *= projection;

				Vector4 clipA = gsOutTri.vertexA.position;
				Vector4 clipB = gsOutTri.vertexB.position;
				Vector4 clipC = gsOutTri.vertexC.position;
								
				// out-of-frustum cull tests: if any triangle has all three vertices outside one of
				// the planes, we simply go back to the top of the loop and evaulate
				// the next triangle; this one does not need to be drawn.
				// clipping for the side planes will be done via raster clipping so nothing
				// is done here if they do not need to be culled.
				// (note that these tests are equivalent to testing e.g. X < 1 in NDC space)
				if (clipA.x > clipA.w && clipB.x > clipB.w && clipC.x > clipC.w) { continue; }
				if (clipA.x < -clipA.w && clipB.x < -clipB.w && clipC.x < -clipC.w) { continue; }
				if (clipA.y > clipA.w && clipB.y > clipB.w && clipC.y > clipC.w) { continue; }
				if (clipA.y < -clipA.w && clipB.y < -clipB.w && clipC.y < -clipC.w) { continue; }
				if (clipA.z > clipA.w && clipB.z > clipB.w && clipC.z > clipC.w) { continue; }							

				// here we play Clip, Cull, or Draw with the near plane.  Unlike the other planes,
				// we will do geometric clipping here if one or two vertices are on the wrong side
				// of the near plane.
				std::vector<VertexOutType> triangle;				
				triangle.push_back(gsOutTri.vertexA);
				triangle.push_back(gsOutTri.vertexB);
				triangle.push_back(gsOutTri.vertexC);
				std::vector<VertexOutType> v;
				
				int clipCount = 0;	// number of vertices that lie beyond the near clip plane
				if (clipA.z < 0.0f) {									
					v.push_back(gsOutTri.vertexA);
					triangle.erase(triangle.begin());
					clipCount++;
				}
				if (clipB.z < 0.0f) {										
					v.push_back(gsOutTri.vertexB);
					triangle.erase(triangle.begin() + (1 - clipCount));
					clipCount++;
				}
				if (clipC.z < 0.0f) {										
					v.push_back(gsOutTri.vertexC);
					triangle.erase(triangle.begin() + (2 - clipCount));
					clipCount++;
				};

				v.resize(3);

				// Based on the number of vertices that must be clipped, we make any adjustments
				// to the geometry that are necessary and make the appropriate rasterizer calls.

				if (clipCount == 3) { continue; }		// triangle is culled 
				else if (clipCount == 2) {	

					v[2] = triangle[0];

					float alphaV0V2 = -v[0].position.z / (v[2].position.z - v[0].position.z);
					float alphaV1V2 = -v[1].position.z / (v[2].position.z - v[1].position.z);

					v[0] = (v[2] * alphaV0V2) + v[0] * (1 - alphaV0V2);
					v[1] = (v[2] * alphaV1V2) + v[1] * (1 - alphaV1V2);

					if (rasterizer == Rasterizer::Scanline)
						threads.push_back(std::thread(&Pipeline::DrawTriangleSimple<VertexOutType, EffectType>, this, NDCtransform4(v[0], v[0].position), NDCtransform4(v[1], v[1].position), NDCtransform4(v[2], v[2].position), effect));					
				}
				else if (clipCount == 1) {						// triangle must be split/tesselated

					v[1] = triangle[0];
					v[2] = triangle[1];

					float alphaV0V1 = -v[0].position.z / (v[1].position.z - v[0].position.z);
					float alphaV0V2 = -v[0].position.z / (v[2].position.z - v[0].position.z);
					auto newVertex0 = (v[1] * alphaV0V1) + v[0] * (1 - alphaV0V1);
					auto newVertex1 = (v[2] * alphaV0V2) + v[0] * (1 - alphaV0V2);

					if (rasterizer == Rasterizer::Scanline) {
						threads.push_back(std::thread(&Pipeline::DrawTriangleSimple<VertexOutType, EffectType>, this, NDCtransform4(newVertex0, newVertex0.position), NDCtransform4(v[1], v[1].position), NDCtransform4(v[2], v[2].position), effect));
						threads.push_back(std::thread(&Pipeline::DrawTriangleSimple<VertexOutType, EffectType>, this, NDCtransform4(newVertex1, newVertex1.position), NDCtransform4(newVertex0, newVertex0.position), NDCtransform4(v[2], v[2].position), effect));
					}
				}
				else // no clipping, triangle is drawn in full
				{					
					threads.push_back(std::thread(&Pipeline::DrawTriangleSimple<VertexOutType, EffectType>, this, NDCtransform4(gsOutTri.vertexA, clipA), NDCtransform4(gsOutTri.vertexB, clipB), NDCtransform4(gsOutTri.vertexC, clipC), effect));
				}
			}
			for (int i = 0; i < threads.size(); i++) {
				threads[i].join();
			}
		}

		template<class VertexType, class EffectType>		
		void DrawTriangleSimple(const VertexType& a, const VertexType& b, const VertexType& c, EffectType effect) {

			const VertexType* sortA = &a;
			const VertexType* sortB = &b;
			const VertexType* sortC = &c;

			if (sortB->position.y < sortA->position.y) std::swap(sortA, sortB);
			if (sortC->position.y < sortB->position.y) std::swap(sortB, sortC);
			if (sortB->position.y < sortA->position.y) std::swap(sortA, sortB);


			if (sortA->position.y == sortB->position.y) {									// if natural flat top
				if (sortB->position.x < sortA->position.x) std::swap(sortA, sortB);		// sort top vertices by x
				DrawFlatTriangleSimple(TriangleType::FlatTop, *sortA, *sortB, *sortC, effect);
			}
			else if (sortB->position.y == sortC->position.y) {							// if natural flat bottom
				if (sortC->position.x < sortB->position.x) std::swap(sortC, sortB);		// sort top vertices by x
				DrawFlatTriangleSimple(TriangleType::FlatBottom, *sortA, *sortB, *sortC, effect);
			}
			else {	// we have a general triangle
				// find interpolated vertex for splitting the triangle into a flat bottom + a flat top
				float alpha = (sortB->position.y - sortA->position.y) / (sortC->position.y - sortA->position.y);
				const VertexType vertexInterpolated = *sortA + (*sortC - *sortA) * alpha;

				if (sortB->position.x < vertexInterpolated.position.x) {	// major right
					DrawFlatTriangleSimple(TriangleType::FlatBottom, *sortA, *sortB, vertexInterpolated, effect);
					DrawFlatTriangleSimple(TriangleType::FlatTop, *sortB, vertexInterpolated, *sortC, effect);

				}
				else {	// major left
					DrawFlatTriangleSimple(TriangleType::FlatBottom, *sortA, vertexInterpolated, *sortB, effect);
					DrawFlatTriangleSimple(TriangleType::FlatTop, vertexInterpolated, *sortB, *sortC, effect);
				}
			}
		}


		template<class VertexType, class EffectType>
		void DrawFlatTriangleSimple(TriangleType type, const VertexType& ipVertexA, const VertexType& ipVertexB, const VertexType& ipVertexC, EffectType effect) {

			float deltaY = ipVertexC.position.y - ipVertexA.position.y;
			VertexType ipDeltaLeft;
			VertexType ipDeltaRight;
			VertexType ipEdgeLeft = ipVertexA;
			VertexType ipEdgeRight;

			//TextureVertex4 corrected;

			// calculate slopes (run/rise) and attribute deltas, set baseline starting points
			if (type == TriangleType::FlatTop) {
				ipDeltaLeft = (ipVertexC - ipVertexA) / deltaY;			
				ipDeltaRight = (ipVertexC - ipVertexB) / deltaY;
				ipEdgeRight = ipVertexB;
			}
			else { // flat bottom 
				ipDeltaLeft = (ipVertexB - ipVertexA) / deltaY;
				ipDeltaRight = (ipVertexC - ipVertexA) / deltaY;
				ipEdgeRight = ipVertexA;									// left and right are the same vertex here
			}

			// calculate the start and end scanlines (integer y values)
			int yStart = std::max((int)ceil(ipVertexA.position.y - pixelCenter), 0);												// top part of top left rule
			int yEnd = std::min((int)ceil(ipVertexC.position.y - pixelCenter), (int)hardwarePipeline->ScreenHeight - 1);

			// prestep interpolants along both sides (slope, attributes)
			ipEdgeLeft += ipDeltaLeft * (float(yStart) + pixelCenter - ipVertexA.position.y);
			ipEdgeRight += ipDeltaRight * (float(yStart) + pixelCenter - ipVertexA.position.y);

			float textureWidth = float(Texture->GetWidth());
			float textureHeight = float(Texture->GetHeight());
			

			for (int y = yStart; y < yEnd; y++) {

				// calculate start and end pixels (integer x values)
				int xStart = std::max((int)ceil(ipEdgeLeft.position.x - pixelCenter), 0);										// left part of top-left rule
				int xEnd = std::min((int)ceil(ipEdgeRight.position.x - pixelCenter), (int)hardwarePipeline->ScreenWidth - 1);

				// create scanline interpolant starting point
				VertexType ipScanlinePosition = ipEdgeLeft;

				// calculate delta interpolant / delta x
				VertexType ipDeltaScanline = (ipEdgeRight - ipScanlinePosition) / (ipEdgeRight.position.x - ipEdgeLeft.position.x);

				// prestep scanline interpolant
				ipScanlinePosition += ipDeltaScanline * (float(xStart) + pixelCenter - ipEdgeLeft.position.x);

				for (int x = xStart; x < xEnd; x++) {					
					if (depthBuffer.TestAndSet(x, y, ipScanlinePosition.position.z)) {
						float w = 1.0f / ipScanlinePosition.position.w;	// recover z
						VertexType corrected = ipScanlinePosition * w;												
						hardwarePipeline->PutPixel(x, y, effect->PixelShaderFn(corrected));						
					}
					ipScanlinePosition += ipDeltaScanline; // update position within scanline
				}

				// update right and left edge interpolated values at the end of each scanline
				ipEdgeLeft += ipDeltaLeft;
				ipEdgeRight += ipDeltaRight;
			}
			
		} // end DrawFlatTriangle		
	}; //end Pipeline class
} // close namespace