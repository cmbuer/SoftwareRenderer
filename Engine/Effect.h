#pragma once
#include <memory>
#include "Software3D.h"

struct TextureEffect2 {

	Matrix3x3& rotation;
	Vector3& translation;
    std::unique_ptr<Surface> texture;
    float textureWidth, textureHeight;

    void BindRotation(const Matrix3x3& rotationBuffer) { rotation = rotationBuffer; }
    void BindTranslation(const Vector3& translationBuffer) { translation = translationBuffer; }

    void BindTexture(const std::wstring& filename) {
        texture = std::make_unique<Surface>(Surface::FromFile(filename));
        textureWidth = float(texture->GetWidth());
        textureHeight = float(texture->GetHeight());
    }

    template<typename VertexTypeIn, typename VertexTypeOut>
    VertexTypeOut VertexShader(VertexTypeIn vertex) {}

    template<typename PrimitiveTypeIn, typename PrimtiveTypeOut>
    PrimtiveTypeOut GeometryShader(PrimitiveTypeIn vertex) {}

    template<typename VertexTypeIn, typename PixelValueTypeOut>
    PixelValueTypeOut PixelShader(VertexTypeIn vertex) {}
};


/*

struct EffectType {

	EffectType() : VertexShader(this), GeometryShader(this), PixelShader(this) {}

	template<class VertexTypeIn, class VertexTypeOut>
	struct VertexShaderClass {

		EffectType* effect;
		VertexShaderClass(EffectType* parent) { effect = parent; }
		VertexTypeOut operator() (VertexTypeIn v) { return effect->vertexShader(v); }
		virtual VertexTypeOut vertexShader(VertexTypeIn v) { return v; }
	};
	
	template<typename VertexTypeIn, typename VertexTypeOut>
	VertexTypeOut VertexShader(VertexTypeIn vertex) { VertexShaderClass }
	

	struct GeometryShaderClass {

		EffectType* effect;
		GeometryShaderClass(EffectType* parent) { effect = parent; }
		PrimitiveType operator() (PrimitiveType p) { return effect->geometryShader(p); }
	};
	GeometryShaderClass GeometryShader;
	virtual PrimitiveType geometryShader(PrimitiveType p) { return p; }

	struct PixelShaderClass {

		EffectType* effect;
		PixelShaderClass(EffectType* parent) { effect = parent; }
		PixelValueType operator() (VertexType v) { return effect->pixelShader(v); }
	};
	PixelShaderClass PixelShader;
	virtual PixelValueType pixelShader(VertexType v) { return Colors::Black; }	// test values
};

*/
/*
template<class VSin, class VSout, class GSin, class GSout, class PSin, class PSout>
struct NewEffectType {

	virtual VSout VertexShader (VSin) {}
	virtual GSout GeometryShader(GSin) {}
	virtual PSout PixelShader(PSin) {}

};

template<class VSin, class VSout, class GSin, class GSout, class PSin, class PSout>
struct NewDefaultEffect : NewEffectType {

	VSout VertexShader(VSin v) { return v; }
	GSout GeometryShader(GSin g) { return g; }
	PSout PixelShader(PSin p) { return Colors::Red; }
};
*/