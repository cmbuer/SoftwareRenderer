#pragma once
#include <algorithm>
#include <cmath>
#include "Surface.h"
#include "S3Dtypes.h"

#define Floor(x)	(int) floor(x)
#define Ceil(x)		(int) ceil(x)

namespace Software3D {

	// forward declaration
	class Pipeline;


	struct TextureEffect {

		Pipeline* pipeline;
		Matrix3x3 rotation;
		Vector3 translation;

		virtual void BindRotation(const Matrix3x3& rotationBuffer) { rotation = rotationBuffer; }
		virtual void BindTranslation(const Vector3& translationBuffer) { translation = translationBuffer; }

		std::unique_ptr<Surface> texture;
		float textureWidth, textureHeight;
		
		
		float minU = 9999, maxU = 0, minV = 9999, maxV = 0;
		int *uVals, *vVals;
		
		TextureEffect() {};
		TextureEffect(Pipeline* softwarePipeline) { pipeline = softwarePipeline; minU = minV = 10000; maxU = maxV = -100; };

		void BindTexture(const std::wstring& filename);

		TextureVertex VertexShaderFn(const TextureVertex& vertex) {
			TextureVertex vertexOut;
			vertexOut.position = vertex.position * rotation + translation;  
			vertexOut.texture = vertex.texture;
			return vertexOut;
		}
		
		Triangle<TextureVertex> GeometryShaderFn(const Triangle<TextureVertex>& t) { return t; }

		PixelValue PixelShaderFn(const TextureVertex& vertex) {

			int ui = (int)std::min((std::max(vertex.texture.x, 0.0f)) * textureWidth, textureWidth - 1);
			int vi = (int)std::min((std::max(vertex.texture.y, 0.0f)) * textureHeight, textureHeight - 1);
			
			// BEGIN BILINEAR FILTERING //
			
			float u = std::min(std::max(vertex.texture.x * textureWidth - pixelCenter, 0.0f) , textureWidth - 1);
			float v = std::min(std::max((vertex.texture.y * textureHeight - pixelCenter), 0.0f), textureHeight - 1);

			static bool uMax = false;
			static bool uMin = false; 
			static bool vMax = false;
			static bool vMin = false;

			if (u == textureWidth - 1) {
				uMax = true;
				float q = u;
			}				
			if (u == 0) {
				uMin = true;
				float q = u;
			}				
			if (v == textureHeight - 1) {
				vMax = true;
				float q = v;
			}				
			if (v == 0) {
				vMin = true;
				float q = v;
			}
				
			if (u < minU) minU = u;
			if (u > maxU) maxU = u;
			if (v < minV) minV = v;
			if (v > maxV) maxV = v;
						

			Vector3 pixelRGBufvf, pixelRGBucvf, pixelRGBufvc, pixelRGBucvc;			
			Vector3 interp1, interp2, interpFinal;			// RGB vectors	

			pixelRGBufvf.x = (texture->GetPixel(Floor(u), Floor(v))).GetR();
			pixelRGBufvf.y = (texture->GetPixel(Floor(u), Floor(v))).GetG();
			pixelRGBufvf.z = (texture->GetPixel(Floor(u), Floor(v))).GetB();

			pixelRGBucvf.x = (texture->GetPixel(Ceil(u), Floor(v))).GetR();
			pixelRGBucvf.y = (texture->GetPixel(Ceil(u), Floor(v))).GetG();
			pixelRGBucvf.z = (texture->GetPixel(Ceil(u), Floor(v))).GetB();
			
			Vector3 test1 = pixelRGBufvf * (Ceil(u) - u);
			Vector3 test2 = pixelRGBucvf * (u - Floor(u));
			interp1 = test1 + test2;

			pixelRGBufvc.x = (texture->GetPixel(Floor(u), Ceil(v))).GetR();
			pixelRGBufvc.y = (texture->GetPixel(Floor(u), Ceil(v))).GetG();
			pixelRGBufvc.z = (texture->GetPixel(Floor(u), Ceil(v))).GetB();

			pixelRGBucvc.x = (texture->GetPixel(Ceil(u), Ceil(v))).GetR();
			pixelRGBucvc.y = (texture->GetPixel(Ceil(u), Ceil(v))).GetG();
			pixelRGBucvc.z = (texture->GetPixel(Ceil(u), Ceil(v))).GetB();

			Vector3 test3 = pixelRGBufvc * (Ceil(u) - u);
			Vector3 test4 = pixelRGBucvc * (u - Floor(u));
			interp2 = test3 + test4;

			Vector3 test5 = interp1 * ((Ceil(v) - v));
			Vector3 test6 = interp2 * (v - Floor(v));
			interpFinal = test5 + test6;
			
			Color color = Color((unsigned int) round(interpFinal.x), (unsigned int) round(interpFinal.y), (unsigned int) round(interpFinal.z));

			if (color.dword == Colors::Black.dword)
				exit(666);

			return color;

		}

		//techniques
		void RenderObjectWithTexture(const IndexedList<TextureVertex>& object);

	};


	

}