#pragma once
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <algorithm>

#include "vector3d.hpp"
#define CHECK_VALID( _v ) 0

#define M_PI 3.14159
inline float sqrt2(float sqr) {
	float root = 0;

	__asm {
		sqrtss xmm0, sqr
		movss root, xmm0
	}

	return root;
}

class vec2_t {
public:
	float x, y;

	vec2_t() {
		x = 0; y = 0;
	};
	vec2_t(float _x, float _y) {
		x = _x; y = _y;
	};
	vec2_t(vec3_t vec) {
		x = vec.x; y = vec.y;
	}

	inline vec2_t operator*(const float n) const {
		return vec2_t(x * n, y * n);
	}
	inline vec2_t operator+(const vec2_t& v) const {
		return vec2_t(x + v.x, y + v.y);
	}
	inline vec2_t operator-(const vec2_t & v) const {
		return vec2_t(x - v.x, y - v.y);
	}
	inline void operator+=(const vec2_t & v) {
		x += v.x;
		y += v.y;
	}
	inline void operator-=(const vec2_t & v) {
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const vec2_t & v) const {
		return (v.x == x && v.y == y);
	}
	bool operator!=(const vec2_t & v) const {
		return (v.x != x || v.y != y);
	}

	inline float length() {
		return sqrt((x * x) + (y * y));
	}
};

namespace sdk {
	namespace math {
		class vector_2d {
		public:
			// The only notation exception, for clarity of course.
			float x, y;

			constexpr explicit vector_2d(float flX = 0.f, float flY = 0.f) : x(flX), y(flY) { }
			constexpr explicit vector_2d(const float* flV) : x(flV[0]), y(flV[1]) { }

			~vector_2d() { }

			// Basic
			bool IsEmpty() const {
				return x == 0.f && y == 0.f;
			}

			void Clear() {
				x = y = 0.f;
			}

			// Operators
			bool operator==(const vector_2d& vOther) const {
				return (x == vOther.x && y == vOther.y);
			}

			bool operator!=(const vector_2d& vOther) const {
				return !(*this == vOther);
			}

			template <typename A>
			vector_2d operator+(A tOther) const {
				vector_2d vBuffer = *this;
				vBuffer += static_cast<float>(tOther);
				return vBuffer;
			}

			template <typename A>
			vector_2d operator-(A tOther) const {
				vector_2d vBuffer = *this;
				vBuffer -= static_cast<float>(tOther);
				return vBuffer;
			}

			template <typename A>
			vector_2d operator*(A tOther) const {
				vector_2d vBuffer = *this;
				vBuffer *= static_cast<float>(tOther);
				return vBuffer;
			}

			template <typename A>
			vector_2d operator/(A tOther) const {
				vector_2d vBuffer = *this;
				vBuffer /= static_cast<float>(tOther);
				return vBuffer;
			}

			template <typename A>
			vector_2d& operator+=(A tOther) {
				x += static_cast<float>(tOther);
				y += static_cast<float>(tOther);

				return *this;
			}

			template <typename A>
			vector_2d& operator-=(A tOther) {
				x -= static_cast<float>(tOther);
				y -= static_cast<float>(tOther);

				return *this;
			}

			template <typename A>
			vector_2d& operator*=(A tOther) {
				x *= static_cast<float>(tOther);
				y *= static_cast<float>(tOther);

				return *this;
			}

			template <typename A>
			vector_2d& operator/=(A tOther) {
				x /= static_cast<float>(tOther);
				y /= static_cast<float>(tOther);

				return *this;
			}

			vector_2d operator+(const vector_2d& tOther) const {
				vector_2d vBuffer = *this;

				vBuffer.x += tOther.x;
				vBuffer.y += tOther.y;

				return vBuffer;
			}

			vector_2d operator-(const vector_2d& tOther) const {
				vector_2d vBuffer = *this;

				vBuffer.x -= tOther.x;
				vBuffer.y -= tOther.y;

				return vBuffer;
			}

			vector_2d& operator+=(const vector_2d& tOther) {
				x += tOther.x;
				y += tOther.y;

				return (*this);
			}

			vector_2d& operator-=(const vector_2d& tOther)
			{
				x -= tOther.x;
				y -= tOther.y;

				return (*this);
			}

			// Math
			float Length() const {
				return sqrt(LengthSqr());
			}

			float LengthSqr() const {
				return x * x + y * y;
			}

			float Distance(const vector_2d& vOther) const {
				return (vOther - (*this)).Length();
			}

			void Normalize() {
				*this /= Length();
			}

			vector_2d Normalized() const {
				vector_2d vBase = *this;
				vBase.Normalize();
				return vBase;
			}
		};
	};
};




class Vector
{
public:
	Vector(void)
	{
		Invalidate();
	}
	Vector(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	Vector(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance);
	}

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	}
	bool IsValid() const
	{
		if (!x || !y || !z)
			return false;
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}
	void Invalidate()
	{
		x = y = z = std::numeric_limits<float>::infinity();
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	void Zero()
	{
		x = y = z = 0.0f;
	}

	bool operator==(const Vector& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}
	bool operator!=(const Vector& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	Vector& operator+=(const Vector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector operator+(float fl) const
	{
		return Vector(x + fl, y + fl, z + fl);
	}
	Vector operator-(float fl) const
	{
		return Vector(x - fl, y - fl, z - fl);
	}
	Vector& operator-=(const Vector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}
	Vector& operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector& operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	Vector& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}
	Vector& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vector& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	void NormalizeInPlace()
	{
		*this = Normalized();
	}
	void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result)
	{
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}
	Vector Cross(const Vector& vOther)
	{
		Vector res;
		VectorCrossProduct(*this, vOther, res);
		return res;
	}
	Vector Normalized() const
	{
		Vector res = *this;
		float l = res.Length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	float DistTo(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.Length();
	}
	float DistToSqr(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}
	float Dot(const Vector& vOther) const
	{
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}
	float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	float Size()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	Vector Angle()
	{
		if (!x && !y)
			return Vector(0, 0, 0);

		float roll = 0;
		return Vector(atan2f(-z, sqrt2(x * x + y * y)) * 180.0f / M_PI, atan2f(y, x) * 180.0f / M_PI, roll);
	}
	float LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}
	float Length2DSqr(void) const
	{
		return (x * x + y * y);
	}
	float Length2D() const
	{
		return sqrt(x * x + y * y);
	}

	void MulAdd(const Vector& a, const Vector& b, float scalar)
	{
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	Vector& operator=(const Vector& vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	Vector operator-(void) const
	{
		return Vector(-x, -y, -z);
	}
	Vector operator+(const Vector& v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	Vector operator*(float fl) const
	{
		return Vector(x * fl, y * fl, z * fl);
	}
	Vector operator*(const Vector& v) const
	{
		return Vector(x * v.x, y * v.y, z * v.z);
	}
	Vector operator/(float fl) const
	{
		return Vector(x / fl, y / fl, z / fl);
	}
	Vector operator/(const Vector& v) const
	{
		return Vector(x / v.x, y / v.y, z / v.z);
	}

	float x, y, z;
};



inline Vector operator*(float lhs, const Vector& rhs)
{
	return rhs * lhs;
}
inline Vector operator/(float lhs, const Vector& rhs)
{
	return rhs / lhs;
}