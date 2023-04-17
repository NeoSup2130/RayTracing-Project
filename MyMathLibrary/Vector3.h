#pragma once
namespace SlavMath
{
	class Matrix3;
	class Matrix4;
	class Vector3
	{
	public:
		union {
			struct { float x, y, z; };
			struct { float R, G, B; };
			struct { float cell[3]; };
		};
		Vector3() : x(0), y(0), z(0) {}
		Vector3(const float& value) : x(value), y(value), z(value) {}
		Vector3(float&& x, float&& y, float&& z) : x(x), y(y), z(z) {}
		Vector3(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {}

		Vector3 operator - () const;
		Vector3 operator - (const Vector3& other) const;
		Vector3 operator + (const Vector3& other) const;
		Vector3 operator * (const Vector3& other) const;
		Vector3 operator * (const float& scalar) const;
		Vector3 operator * (float&& scalar) const;
		Vector3 operator / (float&& scalar) const;
		Vector3 operator / (const Vector3& other) const;
		void operator -= (const Vector3& other);
		void operator += (const Vector3& other);
		void operator *= (const Vector3& other);
		void operator *= (const float& scalar);
		void operator *= (float&& scalar);
		void operator /= (float&& scalar);
		//No bounds check for index
		float& operator [] (const int& index);

		float magnitude() const;
		float magnitudeRoot() const;
		float getMaxComponent() const;
		float getMinComponent() const;
		void cleanNanInf();
		void normalize();
		Vector3 normalized() const;
		Vector3 perpendicular() const;
		Vector3 inversed() const;
		void clear();
		Vector3 Transformed(const Matrix3& matrix) const;
		void Transform(const Matrix3& matrix);
		void Transform(const Matrix4& matrix);
		Vector3 unit() const;

		
	};
	using Color = SlavMath::Vector3;
	using Point3D = SlavMath::Vector3;
};

SlavMath::Vector3 operator*(float val, const SlavMath::Vector3& rhs);

bool operator==(const SlavMath::Vector3& a, const SlavMath::Vector3& b);

bool operator!=(const SlavMath::Vector3& a, const SlavMath::Vector3& b);

float dot(const SlavMath::Vector3& a, const SlavMath::Vector3& b);

SlavMath::Vector3 crossProduct(const SlavMath::Vector3& a, const SlavMath::Vector3& b);

Pixel getVectorColor(const SlavMath::Vector3& vector);
