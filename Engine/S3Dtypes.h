#pragma once
#include <vector>
#include "Math3D.h"
#include "Colors.h"


namespace Software3D {

	
	static constexpr float pixelCenter = 0.5f;
	typedef Color PixelValue;						// Color is currently the only supported pixel value

	struct BasicVertex {

		Vector4 position;
		
		BasicVertex() = default;
		BasicVertex(const Vector3& pos) { position = pos; }
		BasicVertex& operator+= (const BasicVertex& rhs) {
			position += rhs.position;
			return *this;
		}
		BasicVertex& operator-= (const BasicVertex& rhs) {
			position -= rhs.position;
			return *this;
		}
		BasicVertex& operator*= (float rhs) {
			position *= rhs;
			return *this;
		}
		BasicVertex& operator/= (float rhs) {
			position /= rhs;
			return *this;
		}
		BasicVertex operator+ (const BasicVertex& rhs) const { return BasicVertex(*this) += rhs; }
		BasicVertex operator- (const BasicVertex& rhs) const { return BasicVertex(*this) -= rhs; }
		BasicVertex operator* (float rhs) const { return BasicVertex(*this) *= rhs; }
		BasicVertex operator/ (float rhs) const { return BasicVertex(*this) /= rhs; }
	};

	struct BasicVertexChild : BasicVertex {
		Vector4 someOtherPosition;
	};

	
	struct FixedColorVertex {

		Vector4 position;
		Color color;

		FixedColorVertex() = default;
		FixedColorVertex(const Vector3& pos) { position = pos; }
		FixedColorVertex(const Vector3& pos, const FixedColorVertex& other) {
			position = pos;
			color = other.color;
		}
		FixedColorVertex(const Vector3& pos, const Color c) {
			position = pos;
			color = c;
		}
		FixedColorVertex& operator+= (const FixedColorVertex& rhs) {
			position += rhs.position;
			return *this;
		}
		FixedColorVertex& operator-= (const FixedColorVertex& rhs) {
			position -= rhs.position;
			return *this;
		}
		FixedColorVertex& operator*= (float rhs) {
			position *= rhs;
			return *this;
		}
		FixedColorVertex& operator/= (float rhs) {
			position /= rhs;
			return *this;
		}
		FixedColorVertex operator+ (const FixedColorVertex& rhs) const { return FixedColorVertex(*this) += rhs; }
		FixedColorVertex operator- (const FixedColorVertex& rhs) const { return FixedColorVertex(*this) -= rhs; }
		FixedColorVertex operator* (float rhs) const { return FixedColorVertex(*this) *= rhs; }
		FixedColorVertex operator/ (float rhs) const { return FixedColorVertex(*this) /= rhs; }
	};

	struct ColorVertex {

		Vector4 position;
		Vector3 colorRGBfloatVector;
		ColorVertex() = default;
		ColorVertex(const Vector3& pos) { position = pos; }
		ColorVertex(const Vector3& pos, const ColorVertex& other) {
			position = pos;
			colorRGBfloatVector = other.colorRGBfloatVector;
		}
		ColorVertex(const Vector3& pos, const Vector3& rgbVector) {
			position = pos;
			colorRGBfloatVector = rgbVector;
		}
		ColorVertex& operator+= (const ColorVertex& rhs) {
			position += rhs.position;
			colorRGBfloatVector += rhs.colorRGBfloatVector;
			return *this;
		}
		ColorVertex& operator-= (const ColorVertex& rhs) {
			position -= rhs.position;
			colorRGBfloatVector -= rhs.colorRGBfloatVector;
			return *this;
		}
		ColorVertex& operator*= (float rhs) {
			position *= rhs;
			colorRGBfloatVector *= rhs;
			return *this;
		}
		ColorVertex& operator/= (float rhs) {
			position /= rhs;
			colorRGBfloatVector /= rhs;
			return *this;
		}
		ColorVertex operator+ (const ColorVertex& rhs) const { return ColorVertex(*this) += rhs; }
		ColorVertex operator- (const ColorVertex& rhs) const { return ColorVertex(*this) -= rhs; }
		ColorVertex operator* (float rhs) const { return ColorVertex(*this) *= rhs; }
		ColorVertex operator/ (float rhs) const { return ColorVertex(*this) /= rhs; }
	};


	struct TextureVertex {

		Vector4 position;
		Vector2 texture;

		TextureVertex() = default;
		TextureVertex(const Vector3& pos, const Vector2& tex) {
			position = pos;
			texture = tex;
		}
		TextureVertex(const Vector3& pos, const TextureVertex& sourceVertex) {		
			position = pos;
			texture = sourceVertex.texture;
		}
		TextureVertex& operator+= (const TextureVertex& rhs) {
			position += rhs.position;
			texture += rhs.texture;
			return *this;
		}
		TextureVertex& operator-= (const TextureVertex& rhs) {
			position -= rhs.position;
			texture -= rhs.texture;
			return *this;
		}
		TextureVertex& operator*= (float rhs) {
			position *= rhs;
			texture *= rhs;
			return *this;
		}
		TextureVertex& operator/= (float rhs) {
			position /= rhs;
			texture /= rhs;
			return *this;
		}

		TextureVertex operator+ (const TextureVertex& rhs) const { return TextureVertex(*this) += rhs; }
		TextureVertex operator- (const TextureVertex& rhs) const { return TextureVertex(*this) -= rhs; }
		TextureVertex operator* (float rhs) const { return TextureVertex(*this) *= rhs; }
		TextureVertex operator/ (float rhs) const { return TextureVertex(*this) /= rhs; }
	};


	struct TextureVertex4 {

		Vector4 position;
		Vector2 texture;

		TextureVertex4() = default;
		TextureVertex4(const Vector4& pos, const Vector2& tex) {
			position = pos;
			texture = tex;
		}
		TextureVertex4(const Vector4& pos, const TextureVertex4& sourceVertex) {
			position = pos;
			texture = sourceVertex.texture;
		}
		TextureVertex4& operator+= (const TextureVertex4& rhs) {
			position += rhs.position;
			texture += rhs.texture;
			return *this;
		}
		TextureVertex4& operator-= (const TextureVertex4& rhs) {
			position -= rhs.position;
			texture -= rhs.texture;
			return *this;
		}
		TextureVertex4& operator*= (float rhs) {
			position *= rhs;
			texture *= rhs;
			return *this;
		}
		TextureVertex4& operator/= (float rhs) {
			position /= rhs;
			texture /= rhs;
			return *this;
		}

		TextureVertex4 operator+ (const TextureVertex4& rhs) const { return TextureVertex4(*this) += rhs; }
		TextureVertex4 operator- (const TextureVertex4& rhs) const { return TextureVertex4(*this) -= rhs; }
		TextureVertex4 operator* (float rhs) const { return TextureVertex4(*this) *= rhs; }
		TextureVertex4 operator/ (float rhs) const { return TextureVertex4(*this) /= rhs; }
	};


	struct WorldNormalVertex {

		Vector4 position;
		Vector3 worldPosition;
		Vector3 surfaceNormal;

		WorldNormalVertex() = default;
		WorldNormalVertex(const Vector3 & pos) { position = pos; }
		WorldNormalVertex(const Vector3& pos, const WorldNormalVertex& other) {
			position = other.position;
			worldPosition = other.worldPosition;
			surfaceNormal = other.surfaceNormal;
				
		}
		WorldNormalVertex(const Vector3& pos, const Vector3& worldpos, const Vector3& normal){
			position = pos;
			worldPosition = worldpos;
			surfaceNormal = normal;
		}
		WorldNormalVertex& operator+= (const WorldNormalVertex& rhs) {
			position += rhs.position;
			worldPosition += rhs.worldPosition;
			surfaceNormal += rhs.surfaceNormal;
			return *this;
		}
		WorldNormalVertex& operator-= (const WorldNormalVertex& rhs) {
			position -= rhs.position;
			worldPosition -= rhs.worldPosition;
			surfaceNormal -= rhs.surfaceNormal;
			return *this;
		}
		WorldNormalVertex& operator*= (float rhs) {
			position *= rhs;
			worldPosition *= rhs;
			surfaceNormal *= rhs;
			return *this;
		}
		WorldNormalVertex& operator/= (float rhs) {
			position /= rhs;
			worldPosition /= rhs;
			surfaceNormal /= rhs;
			return *this;
		}
		WorldNormalVertex operator+ (const WorldNormalVertex& rhs) const { return WorldNormalVertex(*this) += rhs; }
		WorldNormalVertex operator- (const WorldNormalVertex& rhs) const { return WorldNormalVertex(*this) -= rhs; }
		WorldNormalVertex operator* (float rhs) const { return WorldNormalVertex(*this) *= rhs; }
		WorldNormalVertex operator/ (float rhs) const { return WorldNormalVertex(*this) /= rhs; }
	};

	struct NormalVertex {

		Vector4 position;
		Vector3 surfaceNormal;

		NormalVertex() = default;
		NormalVertex(const Vector3& pos) { position = pos; }
		NormalVertex(const Vector3& pos, const NormalVertex& other) {
			surfaceNormal = other.surfaceNormal,
				position = other.position;
		}
		NormalVertex(const Vector3& pos, const Vector3& normal) {
			position = pos;
			surfaceNormal = normal;
		}
		NormalVertex& operator+= (const NormalVertex& rhs) {
			position += rhs.position;
			return *this;
		}
		NormalVertex& operator-= (const NormalVertex& rhs) {
			position -= rhs.position;
			return *this;
		}
		NormalVertex& operator*= (float rhs) {
			position *= rhs;
			return *this;
		}
		NormalVertex& operator/= (float rhs) {
			position /= rhs;
			return *this;
		}
		NormalVertex operator+ (const NormalVertex& rhs) const { return NormalVertex(*this) += rhs; }
		NormalVertex operator- (const NormalVertex& rhs) const { return NormalVertex(*this) -= rhs; }
		NormalVertex operator* (float rhs) const { return NormalVertex(*this) *= rhs; }
		NormalVertex operator/ (float rhs) const { return NormalVertex(*this) /= rhs; }
	};


	struct TextureNormalVertex4 {

		Vector4 position;
		Vector3 worldPosition;
		Vector3 surfaceNormal;
		Vector2 texture;

		TextureNormalVertex4() = default;
		TextureNormalVertex4(const Vector3& pos) { position = pos; }
		TextureNormalVertex4(const Vector3& pos, const WorldNormalVertex& other) {
			position = other.position;
			worldPosition = other.worldPosition;
			surfaceNormal = other.surfaceNormal;

		}
		TextureNormalVertex4(const Vector3& pos, const Vector3& worldpos, const Vector3& normal) {
			position = pos;
			worldPosition = worldpos;
			surfaceNormal = normal;
		}
		TextureNormalVertex4& operator+= (const TextureNormalVertex4& rhs) {
			position += rhs.position;
			worldPosition += rhs.worldPosition;
			surfaceNormal += rhs.surfaceNormal;
			return *this;
		}
		TextureNormalVertex4& operator-= (const TextureNormalVertex4& rhs) {
			position -= rhs.position;
			worldPosition -= rhs.worldPosition;
			surfaceNormal -= rhs.surfaceNormal;
			return *this;
		}
		TextureNormalVertex4& operator*= (float rhs) {
			position *= rhs;
			worldPosition *= rhs;
			surfaceNormal *= rhs;
			return *this;
		}
		TextureNormalVertex4& operator/= (float rhs) {
			position /= rhs;
			worldPosition /= rhs;
			surfaceNormal /= rhs;
			return *this;
		}
		TextureNormalVertex4 operator+ (const TextureNormalVertex4& rhs) const { return TextureNormalVertex4(*this) += rhs; }
		TextureNormalVertex4 operator- (const TextureNormalVertex4& rhs) const { return TextureNormalVertex4(*this) -= rhs; }
		TextureNormalVertex4 operator* (float rhs) const { return TextureNormalVertex4(*this) *= rhs; }
		TextureNormalVertex4 operator/ (float rhs) const { return TextureNormalVertex4(*this) /= rhs; }
	};




	
	template <class VertexType>
	struct Point {
		VertexType vertex;
		const int vertexCount = 1;
	};

	template <class VertexType>
	struct Line {
		VertexType vertexA, VertexB;
		int index;
		const int vertexCount = 2;
	};
	
	template <class VertexType>
	struct Triangle {
		VertexType vertexA, vertexB, vertexC;
		int index;
		const int vertexCount = 3;
	};
	

	template <class VertexType>
	struct IndexedList {
		enum class PrimitiveType { Point = 1, Line, Triangle };
		std::vector<VertexType> vertices;
		std::vector<unsigned int> indices;
		std::vector<bool> cullFlags;
		PrimitiveType primitiveType;  // so pipeline knows how to parse indices and assemble
	};

	

}