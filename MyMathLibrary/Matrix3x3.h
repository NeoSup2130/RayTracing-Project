#pragma once

namespace SlavMath
{
	class Matrix3
	{
	public:
		float cell[9];
	public:
		Matrix3() { for (int i = 0; i < sizeof(cell) / sizeof(float); i++) cell[i] = 0; }
		Matrix3(float&& a, float&& b, float&& c, float&& d, float&& e, float&& f, float&& g, float&& h, float&& i) :
			cell{ a, b, c, d, e, f, g, h, i } {}

		void setToIdentity();

		Matrix3 getIdentity();

		void setToScale(const float& scalar);

		void setToScale(const float& x, const float& y, const float& z);

		void setToTransform(const float& x, const float& y);

		void setToRotationX(const float& angle);

		void setToRotationY(const float& angle);

		void setToRotationZ(const float& angle);

		float& operator [] (const size_t& index);

		Matrix3 operator - () const;

		Matrix3 operator * (const float& scalar);

		Matrix3 operator + (const float& scalar);

		Matrix3 operator - (const float& scalar);

		Matrix3 operator * (const Matrix3& other);

		Matrix3 operator + (const Matrix3& other);

		Matrix3 operator - (const Matrix3& other);

		Matrix3 operator / (const float& scalar);

		void operator *= (const float& scalar);

		void operator += (const float& scalar);

		void operator -= (const float& scalar);

		void operator /= (const float& scalar);

		void operator *= (const Matrix3& other);

		void operator += (const Matrix3& other);

		void operator -= (const Matrix3& other);

		void Transpose();

		float Det() const;
		void Invert();
		Matrix3 Inverse() const;
		float GetX() const;
		float GetY() const;
		float GetZ() const;
	};
};
