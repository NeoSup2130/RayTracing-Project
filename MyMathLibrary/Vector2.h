#pragma once
namespace SlavMath
{
	class Vector2
	{
	public:
		union {
			struct { float x, y; };
			struct { float cell[2]; };
		};
		Vector2() : x(0), y(0) {}
		Vector2(const float& value) : x(value), y(value) {}
		Vector2(float&& x, float&& y) : x(x), y(y) {}
		Vector2(const float& x, const float& y) : x(x), y(y) {}

		Vector2 operator - ();
		Vector2 operator - (const Vector2& other) const;
		Vector2 operator + (const Vector2& other) const;
		Vector2 operator * (const Vector2& other) const;
		Vector2 operator * (const float& scalar) const;
		Vector2 operator * (float&& scalar) const;
		Vector2 operator / (float&& scalar) const;
		Vector2 operator / (const float& scalar) const;
		Vector2 operator / (const Vector2& other) const;
		void operator -= (const Vector2& other);
		void operator += (const Vector2& other);
		void operator *= (const Vector2& other);
		void operator *= (const float& scalar);
		void operator *= (float&& scalar);
		//No bounds check for index
		float& operator [] (int&& index);

		float magnitude() const;
		float magnitudeRoot() const;
		void normalize();
		Vector2 unit() const;
		Vector2 perpendicular() const;
		void clear();
		float sqaureDistance(const Vector2& other) const;
		float dotProduct(const Vector2& other) const;
		void trim(float&& scalar);
	};
};
	float dot(const SlavMath::Vector2& a, const SlavMath::Vector2& b);

	/// Multiplication with Rhs Vector
	SlavMath::Vector2 operator*(float val, const SlavMath::Vector2& rhs);

	bool operator==(const SlavMath::Vector2& a, const SlavMath::Vector2& b);

	bool operator!=(const SlavMath::Vector2& a, const SlavMath::Vector2& b);

