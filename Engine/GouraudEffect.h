#pragma once
#include "S3Dtypes.h"
#include "S3Dpipeline.h"
#include "Math3D.h"
#include "Lights.h"


namespace Software3D {
	
	struct GouraudEffect {

		Pipeline* pipeline;
		Matrix3x3 rotation;
		Vector3 translation;
		Vector3 materialColor;
		DirectionalLight light;

		GouraudEffect() {};
		GouraudEffect(Pipeline* softwarePipeline) { pipeline = softwarePipeline; };

		virtual void BindRotation(const Matrix3x3& rotationBuffer) { rotation = rotationBuffer; }
		virtual void BindTranslation(const Vector3& translationBuffer) { translation = translationBuffer; }


		ColorVertex VertexShaderFn(const NormalVertex& vertex) { 
			Vector3 v = light.diffuseColor * std::max(0.0f, -(vertex.surfaceNormal * rotation) * light.direction);			
			Vector3 litColor = materialColor.Hadamard(v + light.ambientColor).Saturate() * 255.0f;
			return{ vertex.position * rotation + translation, litColor };
		}
		

		Triangle<ColorVertex> GeometryShaderFn(const Triangle<ColorVertex>& triangle) { return triangle; }
		PixelValue PixelShaderFn(const ColorVertex& vertex) { return Color(Vector3(vertex.colorRGBfloatVector)); }

		// techniques

		void RenderObjectWithGouraudShading(IndexedList<NormalVertex> object) {
			pipeline->DrawSimple(object, this);
		}
	};


}
