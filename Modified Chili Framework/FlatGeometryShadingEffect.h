#pragma once
#include "S3Dtypes.h"
#include "S3Dpipeline.h"
#include "Math3D.h"
#include "Lights.h"


namespace Software3D{

struct FlatGeometryShadingEffect {

	Pipeline* pipeline;
	Matrix3x3 rotation;
	Vector3 translation;
	Vector3 materialColor;

	virtual void BindRotation(const Matrix3x3& rotationBuffer) { rotation = rotationBuffer; }
	virtual void BindTranslation(const Vector3& translationBuffer) { translation = translationBuffer; }

	
	DirectionalLight light;

	FlatGeometryShadingEffect(Pipeline* softwarePipeline) { pipeline = softwarePipeline; };

	

	BasicVertex VertexShaderFn(const BasicVertex& vertex) {
		BasicVertex vertexOut;
		vertexOut.position = vertex.position * rotation + translation;
		return vertexOut;
	}
	Triangle<FixedColorVertex> GeometryShaderFn(const Triangle<BasicVertex>& triangle) { 	
		Vector3 faceNormal = ((triangle.vertexB.position - triangle.vertexA.position).Cross (triangle.vertexC.position - triangle.vertexA.position)).GetNormalized();
		Vector3 intensity = light.diffuseColor * std::max(0.0f, -faceNormal * light.direction);
		Color litColor = Color(materialColor.Hadamard(intensity + light.ambientColor).Saturate() * 255.0f);
		
		return { {triangle.vertexA.position, litColor},
				 {triangle.vertexB.position, litColor},
				 {triangle.vertexC.position, litColor} };
		/*
		return { {triangle.vertexA.position, Colors::Green},
				 {triangle.vertexB.position, Colors::Green},
				 {triangle.vertexC.position, Colors::Green} };
				 */
	}
	PixelValue PixelShaderFn(const FixedColorVertex& vertex) { return vertex.color; }

	// techniques

	void RenderObjectWithFlatShading(const IndexedList<BasicVertex>& object) {
		pipeline->DrawSimple(object, this);
	}

};

}