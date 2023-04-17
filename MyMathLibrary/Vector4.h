#pragma once
namespace SlavMath
{
	class Vector4
	{
	public:
		union {
			struct { float x, y, z, w; };
			struct { float cell[4]; };
		};
		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(float&& x, float&& y, float&& z, float&& w) : x(x), y(y), z(z), w(w) {}
		Vector4(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {}

		Vector4 operator - ();
		Vector4 operator - (const Vector4& other) const;
		Vector4 operator + (const Vector4& other) const;
		Vector4 operator * (const Vector4& other) const;
		Vector4 operator * (const float& scalar) const;
		Vector4 operator * (float&& scalar) const;
		void operator -= (const Vector4& other);
		void operator += (const Vector4& other);
		void operator *= (const Vector4& other);
		void operator *= (float&& scalar);
		void operator *= (const float& scalar);
		//No bounds check for index
		float& operator [] (int&& index);

		float magnitude() const;
		float magnitudeRoot() const;
		void normalize();
	};
};
SlavMath::Vector4 crossProduct(const SlavMath::Vector4& a, const SlavMath::Vector4& b);
float dot(const SlavMath::Vector4& a, const SlavMath::Vector4& b);
