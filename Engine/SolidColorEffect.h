#pragma once
#include "S3Dtypes.h"
#include "S3Dpipeline.h"
#include "Math3D.h"
#include "Lights.h"


namespace Software3D {
	struct SolidColorEffect {

		Pipeline* pipeline;
		Matrix3x3 rotation;
		Vector3 translation;
		Vector3 materialColor;

		SolidColorEffect() {};
		SolidColorEffect(Pipeline* softwarePipeline) { pipeline = softwarePipeline; };

		virtual void BindRotation(const Matrix3x3& rotationBuffer) { rotation = rotationBuffer; }
		virtual void BindTranslation(const Vector3& translationBuffer) { translation = translationBuffer; }

		FixedColorVertex VertexShaderFn(const FixedColorVertex& vertex) { return { vertex.position * rotation + translation, vertex.color }; }
		Triangle<FixedColorVertex> GeometryShaderFn(const Triangle<FixedColorVertex>& triangle) { return triangle; }
		PixelValue PixelShaderFn(const FixedColorVertex& vertex) { return vertex.color; }

		// techniques

		void RenderObjectWithSolidColor(const IndexedList<FixedColorVertex>& object) { pipeline->DrawSimple(object, this); }
	};
}