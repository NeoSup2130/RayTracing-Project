#pragma once
namespace SlavMath
{
	class Matrix4
	{
	public:
		float cell[16];
	public:
		Matrix4(bool is_Identity = false);
		Matrix4
		(float&& a, float&& b, float&& c, float&& d,
			float&& e, float&& f, float&& g, float&& h,
			float&& i, float&& j, float&& k, float&& l,
			float&& m, float&& n, float&& o, float&& p) :
			cell{ a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p } {}
		Matrix4
		(const float& a, const float& b, float&& c, const float& d,
			const float& e, const float& f, const float& g, const float& h,
			const float& i, const float& j, const float& k, const float& l,
			const float& m, const float& n, const float& o, const float& p) :
			cell{ a, b, c, d, e, f, g, h, i, j, k, l, n, m, o, p } {}

		void setToIdentity();
		void setToScale(const float& scalar);
		void setToScale(const float& x, const float& y, const float& z);

		void setToTransform(const float& x, const float& y, const float& z);

		void setToRotationX(const float& angle);

		void setToRotationY(const float& angle);

		void setToRotationZ(const float& angle);

		float& operator [] (const size_t& index);

		Matrix4 operator * (const float& scalar);

		Matrix4 operator + (const float& scalar);

		Matrix4 operator - (const float& scalar);

		Matrix4 operator * (const Matrix4& other);

		Matrix4 operator + (const Matrix4& other);

		Matrix4 operator - (const Matrix4& other);

		Matrix4 operator / (const float& scalar);

		void operator *= (const float& scalar);

		void operator += (const float& scalar);

		void operator -= (const float& scalar);

		void operator /= (const float& scalar);

		void operator *= (const Matrix4& other);

		void operator += (const Matrix4& other);

		void operator -= (const Matrix4& other);

		void Transpose();

		float Det() const;

		Matrix4 Inverse() const;
	};
};