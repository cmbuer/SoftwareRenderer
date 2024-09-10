#pragma once
#include <math.h>

namespace Math3D {


	constexpr float PI = 3.14159265f;
	constexpr double PI_D = 3.1415926535897932;

	template <typename T>
	inline auto square(const T& x) { return x * x; }

	template<typename T>
	inline T wrap_angle(T theta)				// mods rotation angle values
	{
		const T modded = fmod(theta, (T)2.0 * (T)PI_D);
		return (modded > (T)PI_D) ?
			(modded - (T)2.0 * (T)PI_D) :
			modded;
	}



	template <typename T>
	class _vector2
	{
	public:
		_vector2() {}
		_vector2(T x, T y)
			:
			x(x),
			y(y)
		{}
		_vector2(const _vector2& vect)
			:
			_vector2(vect.x, vect.y)
		{}

		template <typename T2>
		explicit operator _vector2<T2>() const { return { (T2)x,(T2)y };}
		
		T		SquareMagnitude() const { return square(*this); }
		T		Magnitude() const { return sqrt(SquareMagnitude()); }
		
		_vector2& Normalize()
		{
			const T length = Magnitude();
			x /= length;
			y /= length;
			return *this;
		}
		_vector2	GetNormalized() const
		{
			_vector2 norm = *this;
			norm.Normalize();
			return norm;
		}
		_vector2	operator-() const
		{
			return _vector2(-x, -y);
		}
		_vector2& operator=(const _vector2& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}
		_vector2& operator+=(const _vector2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		_vector2& operator-=(const _vector2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		T		operator*(const _vector2& rhs) const
		{
			return x * rhs.x + y * rhs.y;
		}
		_vector2	operator+(const _vector2& rhs) const
		{
			return _vector2(*this) += rhs;
		}
		_vector2	operator-(const _vector2& rhs) const
		{
			return _vector2(*this) -= rhs;
		}
		_vector2& operator*=(const T& rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		_vector2	operator*(const T& rhs) const
		{
			return _vector2(*this) *= rhs;
		}
		_vector2& operator/=(const T& rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}
		_vector2	operator/(const T& rhs) const
		{
			return _vector2(*this) /= rhs;
		}
		bool	operator==(const _vector2& rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}
		bool	operator!=(const _vector2& rhs) const
		{
			return !(*this == rhs);
		}
	public:
		T x;
		T y;
	};

	typedef _vector2<float> Vector2;
	typedef _vector2<double> Vector2Double;
	typedef _vector2<int> Vector2Int;


	template <typename T>
	class _vector3 : public _vector2<T>
	{
	public:
		_vector3() {}
		_vector3(T x, T y, T z)
			:
			_vector2(x, y),
			z(z)
		{}
		_vector3(const _vector3& vect)
			:
			_vector3(vect.x, vect.y, vect.z)
		{}
		template <typename T2>
		explicit operator _vector3<T2>() const
		{
			return{ (T2)x,(T2)y,(T2)z };
		}
		T		SquareMagnitude() const
		{
			return square(*this);
		}
		T		Magnitude() const
		{
			return sqrt(SquareMagnitude());
		}
		_vector3& Normalize()
		{
			const T length = Magnitude();
			x /= length;
			y /= length;
			z /= length;
			return *this;
		}
		_vector3	GetNormalized() const
		{
			_vector3 norm = *this;
			norm.Normalize();
			return norm;
		}
		_vector3	Cross(const _vector3 rhs) {
			return _vector3(
				y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
		}
		_vector3	Hadamard(const _vector3& rhs) {
			return _vector3(x * rhs.x, y * rhs.y, z * rhs.z);
		}
		_vector3 Saturate()
		{
			return {
			std::min(1.0f, std::max(0.0f, x)),
			std::min(1.0f, std::max(0.0f, y)),
			std::min(1.0f, std::max(0.0f, z)),
			};
		}

		_vector3	operator-() const
		{
			return _vector3(-x, -y, -z);
		}
		_vector3& operator=(const _vector3& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}
		_vector3& operator+=(const _vector3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		_vector3& operator-=(const _vector3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		T		operator*(const _vector3& rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}
		_vector3	operator+(const _vector3& rhs) const
		{
			return _vector3(*this) += rhs;
		}
		_vector3	operator-(const _vector3& rhs) const
		{
			return _vector3(*this) -= rhs;
		}
		_vector3& operator*=(const T& rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		_vector3&	operator*(const T& rhs) const
		{
			return _vector3(*this) *= rhs;
		}
		_vector3& operator/=(const T& rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}
		_vector3	operator/(const T& rhs) const
		{
			return _vector3(*this) /= rhs;
		}
		bool	operator==(const _vector3& rhs) const
		{
			return x == rhs.x && y == rhs.y && rhs.z = z;
		}
		bool	operator!=(const _vector3& rhs) const
		{
			return !(*this == rhs);
		}
	public:
		T z;
	};

	typedef _vector3<float> Vector3;
	typedef _vector3<double> Vector3Double;
	typedef _vector3<int> Vector3Int;


	template <typename T>
	class _vector4 : public _vector3<T>
	{
	public:
		_vector4() = default;
		_vector4(T x, T y, T z, T w)
			:
			_vector3(x, y, z),
			w(w)
		{}
		_vector4(const _vector3& v3, float w = 1.0f)
			:
			_vector3(v3),
			w(w)
		{}
		template <typename T2>
		explicit operator _vector4<T2>() const
		{
			return{ (T2)x,(T2)y,(T2)z,(T2)w };
		}
		T		SquareMagnitude() const
		{
			return square( *this );
		}
		T		Magnitude() const
		{
			return sqrt( SquareMagnitude() );
		}
		_vector4&	Normalize()
		{
			const T length = Magnitude();
			w /= length;
			x /= length;
			y /= length;
			z /= length;
			return *this;
		}
		_vector4	GetNormalized() const
		{
			_vector4 norm = *this;
			norm.Normalize();
			return norm;
		}
		_vector4	operator-() const
		{
			return _vector4(-x, -y, -z, -w);
		}
		_vector4& operator=(const _vector4& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}
		_vector4& operator+=(const _vector4& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}
		_vector4& operator-=(const _vector4& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}
		T		operator*( const _vector4 &rhs ) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
		}
		_vector4	operator+(const _vector4& rhs) const
		{
			return _vector4(*this) += rhs;
		}
		_vector4	operator-(const _vector4& rhs) const
		{
			return _vector4(*this) -= rhs;
		}
		_vector4& operator*=(const T& rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
			return *this;
		}
		_vector4	operator*(const T& rhs) const
		{
			return _vector4(*this) *= rhs;
		}
		_vector4& operator/=(const T& rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;
			return *this;
		}
		_vector4	operator/(const T& rhs) const
		{
			return _vector4(*this) /= rhs;
		}
		bool	operator==(const _vector4& rhs) const
		{
			return x == rhs.x && y == rhs.y && rhs.z == z && rhs.w == w;
		}
		bool	operator!=(const _vector4& rhs) const
		{
			return !(*this == rhs);
		}
		// clamp to between 0.0 ~ 1.0
		_vector4& Saturate()
		{
			x = std::min(1.0f, std::max(0.0f, x));
			y = std::min(1.0f, std::max(0.0f, y));
			z = std::min(1.0f, std::max(0.0f, z));
			w = std::min(1.0f, std::max(0.0f, w));
			return *this;
		}
		// clamp to between 0.0 ~ 1.0
		_vector4	GetSaturated() const
		{
			_vector4 temp(*this);
			temp.Saturate();
			return temp;
		}
		// x3 = x1 * x2 etc.
		_vector4& Hadamard(const _vector4& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			w *= rhs.w;
			return *this;
		}
		// x3 = x1 * x2 etc.
		_vector4	GetHadamard(const _vector4& rhs) const
		{
			_vector4 temp(*this);
			temp.Hadamard(rhs);
			return temp;
		}
	public:
		T w;
	};

	typedef _vector4<float> Vector4;
	typedef _vector4<double> Vector4Double;
	typedef _vector4<int> Vector4Int;


	

	template <typename T, size_t S>
	class _matrix
	{
	public:
		_matrix& operator=(const _matrix& rhs)
		{
			memcpy(elements, rhs.elements, sizeof(elements));
			return *this;
		}
		_matrix& operator*=(T rhs)
		{
			for (auto& row : elements)
			{
				for (T& e : row)
				{
					e *= rhs;
				}
			}
			return *this;
		}
		_matrix operator*(T rhs) const
		{
			_matrix result = *this;
			return result *= rhs;
		}
		_matrix& operator*=(const _matrix& rhs)
		{
			return *this = *this * rhs;
		}
		_matrix operator*(const _matrix& rhs) const
		{
			_matrix result;
			for (size_t j = 0; j < S; j++)
			{
				for (size_t k = 0; k < S; k++)
				{
					T sum = (T)0.0;
					for (size_t i = 0; i < S; i++)
					{
						sum += elements[j][i] * rhs.elements[i][k];
					}
					result.elements[j][k] = sum;
				}
			}
			return result;
		}
		_matrix operator!() const
		{
			_matrix xp;
			for (size_t j = 0; j < S; j++)
			{
				for (size_t k = 0; k < S; k++)
				{
					xp.elements[j][k] = elements[k][j];
				}
			}
			return xp;
		}
		constexpr static _matrix Identity()
		{
			if constexpr (S == 3)
			{
				return {
					(T)1.0,(T)0.0,(T)0.0,
					(T)0.0,(T)1.0,(T)0.0,
					(T)0.0,(T)0.0,(T)1.0,
				};
			}
			else if constexpr (S == 4)
			{
				return {
					(T)1.0,(T)0.0,(T)0.0,(T)0.0,
					(T)0.0,(T)1.0,(T)0.0,(T)0.0,
					(T)0.0,(T)0.0,(T)1.0,(T)0.0,
					(T)0.0,(T)0.0,(T)0.0,(T)1.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}
		constexpr static _matrix Scaling(T factor)
		{
			if constexpr (S == 3)
			{
				return{
					factor,(T)0.0,(T)0.0,
					(T)0.0,factor,(T)0.0,
					(T)0.0,(T)0.0,factor,
				};
			}
			else if constexpr (S == 4)
			{
				return {
					factor,(T)0.0,(T)0.0,(T)0.0,
					(T)0.0,factor,(T)0.0,(T)0.0,
					(T)0.0,(T)0.0,factor,(T)0.0,
					(T)0.0,(T)0.0,(T)0.0,(T)1.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}

		}
		static _matrix RotationZ(T theta)
		{
			const T sinTheta = sin(theta);
			const T cosTheta = cos(theta);
			if constexpr (S == 3)
			{
				return{
					 cosTheta, sinTheta, (T)0.0,
					-sinTheta, cosTheta, (T)0.0,
					(T)0.0,    (T)0.0,   (T)1.0,
				};
			}
			else if constexpr (S == 4)
			{
				return {
					 cosTheta, sinTheta, (T)0.0,(T)0.0,
					-sinTheta, cosTheta, (T)0.0,(T)0.0,
					(T)0.0,    (T)0.0,   (T)1.0,(T)0.0,
					(T)0.0,	   (T)0.0,   (T)0.0,(T)1.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}
		static _matrix RotationY(T theta)
		{
			const T sinTheta = sin(theta);
			const T cosTheta = cos(theta);
			if constexpr (S == 3)
			{
				return{
					 cosTheta, (T)0.0,-sinTheta,
					 (T)0.0,   (T)1.0, (T)0.0,
					 sinTheta, (T)0.0, cosTheta
				};
			}
			else if constexpr (S == 4)
			{
				return {
					cosTheta, (T)0.0, -sinTheta,(T)0.0,
					(T)0.0,   (T)1.0, (T)0.0,   (T)0.0,
					sinTheta, (T)0.0, cosTheta, (T)0.0,
					(T)0.0,   (T)0.0, (T)0.0,   (T)1.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}
		static _matrix RotationX(T theta)
		{
			const T sinTheta = sin(theta);
			const T cosTheta = cos(theta);
			if constexpr (S == 3)
			{
				return{
					(T)1.0, (T)0.0,   (T)0.0,
					(T)0.0, cosTheta, sinTheta,
					(T)0.0,-sinTheta, cosTheta,
				};
			}
			else if constexpr (S == 4)
			{
				return {
					(T)1.0, (T)0.0,   (T)0.0,  (T)0.0,
					(T)0.0, cosTheta, sinTheta,(T)0.0,
					(T)0.0,-sinTheta, cosTheta,(T)0.0,
					(T)0.0, (T)0.0,   (T)0.0,  (T)1.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}
		template<class V>
		constexpr static _matrix Translation(const V& tl)
		{
			return Translation(tl.x, tl.y, tl.z);
		}
		constexpr static _matrix Translation(T x, T y, T z)
		{
			if constexpr (S == 4)
			{
				return {
					(T)1.0,(T)0.0,(T)0.0,(T)0.0,
					(T)0.0,(T)1.0,(T)0.0,(T)0.0,
					(T)0.0,(T)0.0,(T)1.0,(T)0.0,
					x,     y,      z,    (T)1.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}
		constexpr static _matrix Projection(T w, T h, T n, T f)
		{
			if constexpr (S == 4)
			{
				return {
					(T)2.0 * n / w,	(T)0.0,			(T)0.0,				(T)0.0,
					(T)0.0,			(T)2.0 * n / h,	(T)0.0,				(T)0.0,
					(T)0.0,			(T)0.0,			f / (f - n),		(T)1.0,
					(T)0.0,			(T)0.0,			-n * f / (f - n),	(T)0.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}
		constexpr static _matrix ProjectionHFOV(T fov, T aspect, T n, T f)
		{
			if constexpr (S == 4)
			{
				const auto fov_rad = fov * (T)PI / (T)180.0;
				//const auto w = (T)1.0f / std::tan(fov_rad / (T)2.0);
				//const auto h = w * aspect;
				
				const auto h = (T)1.0f / std::tan(fov_rad / (T)2.0);
				const auto w = h / aspect;


				return {
					w,		(T)0.0,	(T)0.0,				(T)0.0,
					(T)0.0,	h,		(T)0.0,				(T)0.0,
					(T)0.0,	(T)0.0,	f / (f - n),		(T)1.0,
					(T)0.0,	(T)0.0,	-n * f / (f - n),	(T)0.0,
				};
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}
		constexpr static _matrix OpenGLProjectionHFOV(T fov, T aspect, T n, T f)
		{
			if constexpr (S == 4)
			{
				const auto fov_rad = fov * (T)PI / (T)180.0;
				//const auto w = (T)1.0f / std::tan(fov_rad / (T)2.0);
				//const auto h = w * aspect;

				const auto h = (T)1.0f / std::tan(fov_rad / (T)2.0);
				const auto w = h / aspect;

				return {
					w,		(T)0.0,	(T)0.0,				(T)0.0,
					(T)0.0,	h,		(T)0.0,				(T)0.0,
					(T)0.0,	(T)0.0,	(-1 - f) / (n - f),		(T)1.0,
					(T)0.0,	(T)0.0,	-1 - ((-n - f * n) / (n - f)),	(T)0.0,
				};

			
			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}

		constexpr static _matrix OpenGLProjectionOrtho(T w, T h, T n, T f)
		{
			if constexpr (S == 4)
			{								
				//const auto w = h / aspect;

				return {
					1 / (w/2),	(T)0.0,		(T)0.0,				(T)0.0,
					(T)0.0,		1 / (h/2),	(T)0.0,				(T)0.0,
					(T)0.0,		(T)0.0,		-2 / (f-n),			-((f+n)/(f-n)),
					(T)0.0,		(T)0.0,		(T)0.0,				(T)1.0,
				};


			}
			else
			{
				static_assert(false, "Bad dimensionality");
			}
		}

	public:
		// [ row ][ col ]
		T elements[S][S];
	};



	template<typename T>
	_vector3<T>& operator*=(_vector3<T>& lhs, const _matrix<T, 3>& rhs)
	{
		return lhs = lhs * rhs;
	}

	template<typename T>
	_vector3<T> operator*(const _vector3<T>& lhs, const _matrix<T, 3>& rhs)
	{
		return{
			lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
			lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
			lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
		};
	}

	template<typename T>
	_vector4<T>& operator*=(_vector4<T>& lhs, const _matrix<T, 4>& rhs)
	{
		return lhs = lhs * rhs;
	}

	template<typename T>
	_vector4<T> operator*(const _vector4<T>& lhs, const _matrix<T, 4>& rhs)
	{
		return{
			lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0] + lhs.w * rhs.elements[3][0],
			lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1] + lhs.w * rhs.elements[3][1],
			lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2] + lhs.w * rhs.elements[3][2],
			lhs.x * rhs.elements[0][3] + lhs.y * rhs.elements[1][3] + lhs.z * rhs.elements[2][3] + lhs.w * rhs.elements[3][3]
		};
	}

	typedef _matrix<float, 3> Matrix3x3;
	typedef _matrix<double, 3> Matrix3x3Double;
	typedef _matrix<float, 4> Matrix4x4;
	typedef _matrix<double, 4> Matrix4x4Double;





	struct Quaternion {

	};

}

using namespace Math3D;
